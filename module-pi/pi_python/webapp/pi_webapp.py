import time
from threading import Thread
from flask import Flask, flash, render_template, request, redirect
from flask_socketio import SocketIO, join_room, emit, send
import eventlet 
import picommon
from pistrobe import PiStrobe
from piholder_web import heater_web

debug_data = { "update_count": 0 }

strobe_data = { "hold": 0 }
picommon.spi_init( 0, 2, 30000 )
strobe = PiStrobe( picommon.PORT_STROBE, 0.1 )
#valid = strobe.set_enable( True )

heaters_data = [ { "status": "", "temp_actual": "" } for i in range(4) ]
heater1 = heater_web( 1, picommon.PORT_HEATER1 )

eventlet.monkey_patch()
app = Flask( __name__ )
socketio = SocketIO(app, async_mode = 'eventlet')
thread = None

def update_heater_data( index, heater ):
    heaters_data[index]["temp_actual"] = "{}".format( round( heater.get_temp(), 2 ) )
    heaters_data[index]["status"] = heater.status_text

def update_heaters_data():
    heater1.update()
    update_heater_data( 0, heater1 )

def background_stuff():
  while True:
    time.sleep( 1 )
    
    socketio.emit( 'debug', debug_data )
    debug_data["update_count"] = debug_data["update_count"] + 1
    
    update_heaters_data()
    #socketio.emit( 'heaters', { 'data': heaters_data } )
    socketio.emit( 'heaters', heaters_data )
    socketio.emit( 'heaters', heaters_data )

@app.route('/', methods=['GET', 'POST'])
def index():
  global thread
  if thread is None:
    thread = socketio.start_background_task( target=background_stuff )
    #thread = Thread( target=background_stuff )
    #thread.start()
  
  if request.method == 'POST':
    if request.form['action'] == 'On':
      valid = strobe.set_hold( True )
      strobe_data["hold"] = 1
    elif request.form['action'] == 'Off':
      valid = strobe.set_hold( False )
      strobe_data["hold"] = 0
    return redirect( '/' )
  
  debug_data["update_count"] = debug_data["update_count"] + 1

  return render_template( 'index.html', debug=debug_data, strobe=strobe_data, heaters=heaters_data )

@socketio.on('create')
def on_create(data):
  #socketio.emit( 'test' )
  pass

@socketio.on('connect')
def on_connect():
  #socketio.emit( 'test' )
  pass

if __name__ == '__main__':
  update_heaters_data()
#    app.run( debug=True, host='0.0.0.0' )
  socketio.run( app, host='0.0.0.0', debug=True )
  