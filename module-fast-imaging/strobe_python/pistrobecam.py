from pistrobe import PiStrobe
from picamera import PiCamera

class PiStrobeCam:
    def __init__( self, spi ):
        self.strobe = PiStrobe( spi )
        self.camera = PiCamera(
            #resolution=
            #framerate=Fraction(1, 1),
            #framerate = 50,
            #sensor_mode=3
        )
        self.camera.awb_mode = 'auto'
        self.camera.exposure_mode = 'off'
        #self.camera.resolution = ( 0, 0 )
        self.camera.iso = 800
    
    def set_timing( self, strobe_wait_padding_ns, strobe_period_ns ):
        camera_read_time_us = 25000
        shutter_padding_us = 0
        shutter_speed_us = int( ( ( strobe_period_ns + strobe_wait_padding_ns ) / 1000 ) + camera_read_time_us + shutter_padding_us )
        framerate = 1000000 / shutter_speed_us
        if ( framerate > 60 ):
            framerate = 60
        self.camera.framerate = framerate
        self.camera.shutter_speed = shutter_speed_us;
#        self.camera.shutter_speed += camera_read_time_us
#        framerate = 1000000 / ( self.camera.shutter_speed )
        
        # Inter-frame period in microseconds
        frame_rate_period_us = int( 1000000 / float( self.camera.framerate ) )
        
        # Dead time after frame, before sampling next frame
        strobe_pre_wait_us = frame_rate_period_us - self.camera.shutter_speed
        
        # How long the strobe is set to wait before triggering
        strobe_wait_ns = strobe_wait_padding_ns + ( 1000 * strobe_pre_wait_us )
        
        #strobe_wait_ns = strobe_wait_padding_ns
        #strobe_period_ns += ( strobe_pre_wait_us * 1000 ) + 3000000
        
        print( 'wait {}, strobe {}, framerate {}, frametime {}, shutter {}={}'.format( strobe_wait_ns, strobe_period_ns, int( self.camera.framerate ), frame_rate_period_us, int( shutter_speed_us ), int( self.camera.shutter_speed ) ) )
        
        self.strobe.set_timing( strobe_wait_ns, strobe_period_ns )
        self.strobe.set_enable( True )
        
    def close( self ):
        #self.strobe.set_enable( False )
        #self.strobe.set_hold( False )
        self.camera.close()
        