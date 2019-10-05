import time
import spidev

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

def strobe_packet_write( spi, type, data ):
  msg = [2, len(data)+4, type] + data
  msg = bytearray( msg )
  checksum = ( -( sum( msg ) & 0xFF ) ) & 0xFF
  msg.append( checksum )
  spi.xfer2( msg )

def spi_init( spi, bus, device, mode, speed_hz ):
  spi.open( bus, device )
  spi.mode = mode
  spi.max_speed_hz = speed_hz
  #spi.no_cs = False
  #spi.cshigh = False

spi = spidev.SpiDev()
spi_init( spi, 0, 0, 2, 500000 )

strobe_packet_write( spi, 3, [1] )
time.sleep(1)
strobe_packet_write( spi, 3, [0] )

#spi.readbytes(n)

spi.close()
