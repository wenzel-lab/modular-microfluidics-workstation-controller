import time
from threading import Thread, Event
from flask import Flask, flash, render_template, request, redirect
from flask_socketio import SocketIO, join_room, emit, send
import eventlet 
import picommon
from pistrobe import PiStrobe
from piholder_web import heater_web

debug_data = { 'update_count': 0 }

strobe_data = { 'hold': 0 }
picommon.spi_init( 0, 2, 30000 )
strobe = PiStrobe( picommon.PORT_STROBE, 0.1 )
#valid = strobe.set_enable( True )

heaters_data = [ { 'status': '', 'temp_actual': '' } for i in range(4) ]
heater1 = heater_web( 1, picommon.PORT_HEATER1 )

eventlet.monkey_patch()
app = Flask( __name__ )
socketio = SocketIO( app, async_mode = 'eventlet' )
thread = None
event = Event()

def update_heater_data( index, heater ):
  heaters_data[index]['temp_actual'] = '{}'.format( round( heater.get_temp(), 2 ) )
  heaters_data[index]['status'] = heater.status_text

def update_heaters_data():
  heater1.update()
  update_heater_data( 0, heater1 )
  update_heater_data( 1, heater1 )
  update_heater_data( 2, heater1 )
  update_heater_data( 3, heater1 )

def background_stuff():
  while True:
    time.sleep( 1 )
#    event.wait( 1 )
#    pi_wait_s( 1 )
    
    update_heaters_data()
    
    debug_data['update_count'] = debug_data['update_count'] + 1
    
    socketio.emit( 'debug', debug_data )
    socketio.emit( 'strobe', strobe_data )
    socketio.emit( 'heaters', heaters_data )

#@app.route('/', methods=['GET', 'POST'])
@app.route( '/' )
def index():
  global thread
  if thread is None:
#    thread = socketio.start_background_task( target=background_stuff, args=(picommon.pi_lock,) )
#    thread = socketio.start_background_task( target=background_stuff )
    thread = Thread( target=background_stuff )
    thread.start()
  
  debug_data['update_count'] = debug_data['update_count'] + 1

  return render_template( 'index.html', debug=debug_data, strobe=strobe_data, heaters=heaters_data )

@socketio.on( 'create' )
def on_create( data ):
  pass

@socketio.on( 'connect' )
def on_connect():
  pass

@socketio.on( 'strobe' )
def on_strobe( data ):
  if ( data['cmd'] == 'hold' ):
    hold_on = 1 if ( data['parameters']['on'] != 0 ) else 0
    valid = strobe.set_hold( hold_on )
    strobe_data['hold'] = hold_on
  socketio.emit( 'strobe', strobe_data )

if __name__ == '__main__':
  update_heaters_data()
#    app.run( debug=True, host='0.0.0.0' )
  socketio.run( app, host='0.0.0.0', debug=True )
  