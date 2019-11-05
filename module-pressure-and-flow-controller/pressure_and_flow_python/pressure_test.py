import time
import spidev
import piflow

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
flow = piflow.PiFlow( spi, 0.1 )

valid, id, id_valid = flow.get_id()
print( valid, bytes(id).decode("ascii"), id_valid )

valid = flow.set_pressure( [100, 200, 300, 4000 << 4] )
print( valid )

spi.close()

#spi.readbytes(n)
