import os
import time
from threading import Thread, Event
from flask import Flask, flash, render_template, request, redirect, Response
from flask_socketio import SocketIO, join_room, emit, send
import eventlet 
import picommon
from pistrobe import PiStrobe
from piholder_web import heater_web
from camera_pi import Camera
from piflow_web import flow_web

# patch eventlet for GPIO and SPI
eventlet.monkey_patch( os=True, select=True, socket=True, thread=False, time=True, psycopg=True )

exit_event = Event()

# initialize SPI for sensors and actuators
picommon.spi_init( 0, 2, 30000 )

debug_data = { 'update_count': 0 }

# set up heaters
heaters_data = [
    { 'status': '',
      'temp_text': '',
      'temp_c_actual': 0.0,
      'temp_c_target': 0.0,
      'pid_enabled': False,
      'power_limit': 0, 
      'autotune_status': '',
      'autotune_target_temp': 0.0,
      'autotuning': False,
      'stir_speed_text': '',
      'stir_speed_target': 0,
      'stir_enabled': False } for i in range(4) 
]
heater1 = heater_web( 1, picommon.PORT_HEATER1 )
heater2 = heater_web( 2, picommon.PORT_HEATER2 )
heater3 = heater_web( 3, picommon.PORT_HEATER3 )
heater4 = heater_web( 4, picommon.PORT_HEATER4 )
heaters = [heater1, heater2, heater3, heater4]

# set up flows
flows_data = [
    { 'status': '',
      'pressure_mbar_text': '',
      'pressure_mbar_target': 0.0,
      'flow_ul_hr_text':'',
      'control_modes': [],
      'control_mode': '',
      'fluid_type':'water'}
    for i in range(4)
]
flow = flow_web( picommon.PORT_FLOW )

app = Flask( __name__ )
socketio = SocketIO( app, async_mode = 'eventlet' )
thread = None

# camera (Raspberry Pi Camera v2 + strobe)
cam = Camera( exit_event, socketio )

def update_heater_data( index, heater ):
  heaters_data[index]['status'] = heater.status_text
  heaters_data[index]['temp_text'] = heater.temp_text
  heaters_data[index]['temp_c_target'] = heater.temp_c_target
  heaters_data[index]['pid_enabled'] = heater.pid_enabled
  heaters_data[index]['power_limit'] = heater.heat_power_limit_pc
  heaters_data[index]['autotune_status'] = heater.autotune_status_text
  heaters_data[index]['autotune_target_temp'] = heater.autotune_target_temp
  heaters_data[index]['autotuning'] = heater.autotuning
  heaters_data[index]['stir_speed_text'] = heater.stir_speed_text
  heaters_data[index]['stir_speed_target'] = heater.stir_target_speed
  heaters_data[index]['stir_enabled'] = heater.stir_enabled

def update_heaters_data():
  heater1.update(); heater2.update(); heater3.update(); heater4.update()
  for i, h in enumerate(heaters):
      update_heater_data(i, h)

def update_flow_data( index ):
  flows_data[index]['status'] = flow.status_text[index]
  flows_data[index]['pressure_mbar_text'] = flow.pressure_mbar_text[index]
  flows_data[index]['pressure_mbar_target'] = flow.pressure_mbar_targets[index]
  flows_data[index]['flow_ul_hr_text'] = flow.flow_ul_hr_text[index]
  flows_data[index]['control_modes'] = flow.ctrl_mode_str
  flows_data[index]['control_mode'] = flow.control_modes[index]
  flows_data[index]['fluid_type'] = flow.fluid_types[index]

def update_flows_data():
  flow.update()
  for i in range(4):
      update_flow_data(i)

def update_all_data():
  cam.update_strobe_data()
  update_heaters_data()
  update_flows_data()

def background_stuff():
  while True:
    time.sleep( 1 )    
    debug_data['update_count'] = debug_data['update_count'] + 1
    update_all_data()
    socketio.emit( 'debug', debug_data )
    socketio.emit( 'heaters', heaters_data )
    socketio.emit( 'flows', flows_data )
    cam.emit()

def start_server():
  global thread
  if thread is None:
    thread = socketio.start_background_task( target=background_stuff )

@socketio.on( 'create' )
def on_create( data ):
  pass

@socketio.on( 'connect' )
def on_connect():
  print( "Connected" )
  start_server()
  pass

@socketio.on( 'cam_select' )
def on_cam( data ):
  if ( data['cmd'] == 'select' ):
    cam.cam_data['camera'] = data['parameters']['camera']
    socketio.emit( 'reload' )

@socketio.on( 'heater' )
def on_heater( data ):
  index = -1
  
  if ( data['cmd'] == 'temp_c_target' ):
    index = data['parameters']['index']
    temp_c_target = data['parameters']['temp_c_target']
    valid = heaters[index].set_temp( temp_c_target )
  elif ( data['cmd'] == 'pid_enable' ):
    index = data['parameters']['index']
    enabled = data['parameters']['on']
    valid = heaters[index].set_pid_running( enabled )
  elif ( data['cmd'] == 'power_limit_pc' ):
    index = data['parameters']['index']
    power_limit_pc = data['parameters']['power_limit_pc']
    valid = heaters[index].set_heat_power_limit_pc( power_limit_pc )
  elif ( data['cmd'] == 'autotune' ):
    index = data['parameters']['index']
    enabled = data['parameters']['on']
    temp = data['parameters']['temp']
    heaters[index].autotune_target_temp = temp
    valid = heaters[index].set_autotune( enabled )
  elif ( data['cmd'] == 'stir' ):
    index = data['parameters']['index']
    enabled = data['parameters']['on']
    speed = data['parameters']['speed']
    heaters[index].stir_target_speed = speed
    valid = heaters[index].set_stir_running( enabled )
  
  if index >= 0:
    heaters[index].update()
    update_heater_data( index, heaters[index] )
    socketio.emit( 'heaters', heaters_data )

@socketio.on( 'flow' )
def on_flow( data ):
  index = -1
  
  if ( data['cmd'] == 'pressure_mbar_target' ):
    index = data['parameters']['index']
    pressure_mbar_target = data['parameters']['pressure_mbar_target']
    valid = flow.set_pressure( index, pressure_mbar_target )
  if ( data['cmd'] == 'control_mode' ):
    index = data['parameters']['index']
    control_mode = data['parameters']['control_mode']
    valid = flow.set_control_mode( index, int( control_mode ) )
    
    update_flows_data()
    socketio.emit( 'flows', flows_data )
    
# --- new handler for fluid selection ---
@socketio.on('flow_fluid')
def on_flow_fluid(data):
    idx = data['index']
    ft  = data['fluid']
    flow.fluid_types[idx]      = ft
    flows_data[idx]['fluid_type'] = ft
    update_flows_data()
    socketio.emit('flows', flows_data)

# --- new handler for flow target ---
@socketio.on('flow_target')
def on_flow_target(data):
    try:
        index = int(data['index'])
        flow_ul_hr = int(data['flow_ul_hr'])
        flow.set_flow_target(index, flow_ul_hr)
        update_flows_data()
        socketio.emit('flows', flows_data)
    except Exception as e:
        print("Error in flow_target handler:", e)

def gen( camera ):
  while True:
    frame = camera.get_frame()
    yield ( b'--frame\r\n'
            b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n' )

@app.route( '/' )
def index():
  debug_data['update_count'] = debug_data['update_count'] + 1
  return render_template(
        'index.html',
        debug=debug_data,
        strobe=cam.strobe_data,
        heaters=heaters_data,
        flows=flows_data,
        cam=cam.cam_data )

@app.route( '/video' )
def video():
  return Response( gen( cam ), mimetype='multipart/x-mixed-replace; boundary=frame' )

def before_first_request():
  print( "Before First Request -----------------------------" )


if __name__ == '__main__':
    update_all_data()
    app.before_first_request( before_first_request )
    socketio.run( app, host='0.0.0.0', debug=True, use_reloader=False )
    
    # clean up SPI on exit
    picommon.spi_close()
    exit_event.set()
    
