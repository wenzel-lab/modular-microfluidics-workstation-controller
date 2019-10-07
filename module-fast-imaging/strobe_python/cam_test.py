import spidev
from pistrobe import PiStrobe
from picamera import PiCamera
from time import sleep
from fractions import Fraction


def spi_init( bus, device, mode, speed_hz ):
  spi = spidev.SpiDev()
  spi.open( bus, device )
  spi.mode = mode
  spi.max_speed_hz = speed_hz
  #spi.no_cs = False
  #spi.cshigh = False
  return spi

spi = spi_init( 0, 0, 2, 500000 )
strobe = PiStrobe( spi )
strobe.set_timing( 100000, 4000000 )
strobe.set_enable( True )
#strobe.set_enable( False )
#strobe.set_hold( True )

camera = PiCamera(
    #resolution=
    framerate=Fraction(1, 1),
#    sensor_mode=3
    )

camera.awb_gains = ( Fraction( 1, 1 ), Fraction ( 1, 1 ) )
#camera.analog_gain = 100
camera.exposure_mode = 'off'
camera.awb_mode = 'auto'
#camera.resolution = ( 0, 0 )
camera.framerate = 13
#camera.annotate_text = 'Test'
#camera.brightness = 70
#camera.contrast = 50
camera.shutter_speed = 64000
camera.iso = 800
camera.exposure_mode = 'off'

camera.start_preview()
#sleep(2)
input()
#camera.capture('capture.jpg')
#camera.start_recording('')
camera.stop_preview()

camera.close()
spi.close()
