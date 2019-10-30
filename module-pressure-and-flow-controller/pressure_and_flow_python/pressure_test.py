import time
import spidev
import pistrobe

def strobe_packet_test():
  msg = [7, 7, 7]
  spi.xfer2( msg )

  time.sleep( 1 )

  msg = [7, 7, 7, 2, 5, 3, 1, 245]
  msg = [2, 7, 7, 7, 7, 2, 5, 3, 1, 245]
  spi.xfer2( msg )

  time.sleep( 1 )

  msg = [2, 5, 3, 0, 246]
  spi.xfer2( msg )

def spi_init( bus, device, mode, speed_hz ):
  spi = spidev.SpiDev()
  spi.open( bus, device )
  spi.mode = mode
  spi.max_speed_hz = speed_hz
  #spi.no_cs = False
  #spi.cshigh = False
  return spi

spi = spi_init( 0, 1, 2, 50000 )
strobe = pistrobe.PiStrobe( spi, 0.1 )
#strobe.set_timing( 2000000, 2000000 )
#strobe.set_hold( True )
#time.sleep( 1 )
#strobe.set_hold( False )

strobe.packet_write( 3, [5] )
time.sleep( 0.1 )
valid = False
while not valid:
    valid, data = strobe.packet_read()
print( valid, data )

spi.close()

#spi.readbytes(n)
