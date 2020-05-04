import picommon
from pistrobe import PiStrobe
from flask import Flask

app = Flask( __name__ )

@app.route('/')
def index():
    picommon.spi_init( 0, 2, 30000 )

    strobe = PiStrobe( picommon.PORT_STROBE, 0.1 )
    valid = strobe.set_enable( True )
    valid = strobe.set_hold( True )
    
    return 'Valid: {}'.format( valid )

if __name__ == '__main__':
    app.run( debug=True, host='0.0.0.0' )
