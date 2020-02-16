import spidev
import RPi.GPIO as GPIO

PORT_NONE         = 0
PORT_HEATER1      = 31
PORT_HEATER2      = 33
PORT_HEATER3      = 32
PORT_HEATER4      = 36

current_device    = PORT_NONE
global spi

def spi_init( bus, mode, speed_hz ):
  global spi
  spi = spidev.SpiDev()
  spi.open( bus, 0 )
  spi.mode = mode
  spi.max_speed_hz = speed_hz
  spi.no_cs = True
  
  GPIO.setwarnings( False )
  GPIO.setmode( GPIO.BOARD )
  GPIO.setup( PORT_HEATER1, GPIO.OUT, initial=GPIO.HIGH )
  GPIO.setup( PORT_HEATER2, GPIO.OUT, initial=GPIO.HIGH )
  GPIO.setup( PORT_HEATER3, GPIO.OUT, initial=GPIO.HIGH )
  GPIO.setup( PORT_HEATER4, GPIO.OUT, initial=GPIO.HIGH )
  
  current_device = PORT_NONE
  
  return spi

def spi_close():
  spi.close()

def spi_select_device( device ):
  spi_deselect_current()
  
  if ( device != PORT_NONE ):
    GPIO.output( device, GPIO.LOW )

def spi_deselect_current():
  if ( current_device != PORT_NONE ):
    GPIO.output( current_device, GPIO.HIGH )
    