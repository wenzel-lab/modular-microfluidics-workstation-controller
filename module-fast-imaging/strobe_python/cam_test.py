import spidev
#from pistrobe import PiStrobe
#from picamera import PiCamera
from pistrobecam import PiStrobeCam
from time import sleep
#from fractions import Fraction


def spi_init( bus, device, mode, speed_hz ):
  spi = spidev.SpiDev()
  spi.open( bus, device )
  spi.mode = mode
  spi.max_speed_hz = speed_hz
  #spi.no_cs = False
  #spi.cshigh = False
  return spi

spi = spi_init( 0, 0, 2, 125000 )
strobe_cam = PiStrobeCam( spi )
strobe_cam.set_timing( 10000, 10000000 )

#strobe_cam.camera.zoom = 1
strobe_cam.camera.start_preview()
sleep(2)
#input()
#camera.capture('capture.jpg')
#camera.start_recording('')
strobe_cam.camera.stop_preview()
#strobe_cam.close()
#sleep(2)

strobe_cam.strobe.set_enable( False )
strobe_cam.close()
spi.close()
