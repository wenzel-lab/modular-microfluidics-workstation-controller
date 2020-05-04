import picommon
from pistrobe import PiStrobe
from flask import Flask, flash, render_template, request, redirect

class strobe_data_class:
  hold = 0
  num = 0

strobe_data = strobe_data_class()
picommon.spi_init( 0, 2, 30000 )
strobe = PiStrobe( picommon.PORT_STROBE, 0.1 )
#valid = strobe.set_enable( True )

app = Flask( __name__ )

@app.route('/', methods=['GET', 'POST'])
def index():
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

if __name__ == '__main__':
    app.run( debug=True, host='0.0.0.0' )
    