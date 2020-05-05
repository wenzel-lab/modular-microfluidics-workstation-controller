import time
from threading import Thread
from flask import Flask, flash, render_template, request, redirect
from flask_socketio import SocketIO, join_room, emit, send
import eventlet 
import picommon
from pistrobe import PiStrobe

class strobe_data_class:
  hold = 0
  num = 0

strobe_data = strobe_data_class()
picommon.spi_init( 0, 2, 30000 )
strobe = PiStrobe( picommon.PORT_STROBE, 0.1 )
#valid = strobe.set_enable( True )

eventlet.monkey_patch()
app = Flask( __name__ )
socketio = SocketIO(app, async_mode = 'eventlet')
thread = None

def background_stuff():
  while True:
    time.sleep( 1 )
    #t = str(time.perf_counter())
    #socketio.emit('message', {'data': 'This is data', 'time': t})
    socketio.emit( 'test', { 'hello': 'world' } )

@app.route('/', methods=['GET', 'POST'])
def index():
  global thread
  if thread is None:
    thread = Thread( target=background_stuff )
    thread.start()
  
  if request.method == 'POST':
    if request.form['action'] == 'On':
      valid = strobe.set_hold( True )
      strobe_data.hold = 1
    elif request.form['action'] == 'Off':
      valid = strobe.set_hold( False )
      strobe_data.hold = 0
    return redirect( '/' )
  
  strobe_data.num = strobe_data.num + 1
  return render_template( 'index.html', strobe=strobe_data )

@socketio.on('create')
def on_create(data):
  socketio.emit( 'test' )

@socketio.on('connect')
def on_connect():
  socketio.emit( 'test' )

if __name__ == '__main__':
#    app.run( debug=True, host='0.0.0.0' )
  socketio.run( app, host='0.0.0.0', debug=True )
  