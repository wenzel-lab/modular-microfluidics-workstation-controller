import spidev
import RPi.GPIO as GPIO

PORT_NONE         = 0
PORT_HEATER1      = 31
PORT_HEATER2      = 33
PORT_HEATER3      = 32
PORT_HEATER4      = 36
PORT_STROBE       = 24
PORT_FLOW         = 26

global current_device
global spi

current_device    = PORT_NONE

def spi_init( bus, mode, speed_hz ):
  global spi
  global current_device
  
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
  GPIO.setup( PORT_STROBE, GPIO.OUT, initial=GPIO.HIGH )
  GPIO.setup( PORT_FLOW, GPIO.OUT, initial=GPIO.HIGH )
  
  current_device = PORT_NONE
  
  return spi

def spi_close():
  spi.close()

def spi_select_device( device ):
  global current_device
  
  if current_device != PORT_NONE and device != current_device:
    GPIO.output( current_device, GPIO.HIGH )
  
  if ( device != PORT_NONE ):
    GPIO.output( device, GPIO.LOW )
    current_device = device
#    print( "Selected {}".format( current_device ) )

def spi_deselect_current():
  global current_device
  
  if ( current_device != PORT_NONE ):
    GPIO.output( current_device, GPIO.HIGH )
#    print( "Deselected {}".format( current_device ) )
    