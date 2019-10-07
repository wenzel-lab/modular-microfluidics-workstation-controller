import time
import spidev

class PiStrobe:
  def __init__( self, spi ):
    self.spi = spi
    
  def packet_write( self, type, data ):
    msg = [2, len(data)+4, type] + data
#    msg = bytearray( msg )
    checksum = ( -( sum( msg ) & 0xFF ) ) & 0xFF
    msg.append( checksum )
    self.spi.xfer2( msg )
    
  def set_enable( self, enable ):
    self.packet_write( 1, [1 if enable else 0] )

  def set_timing( self, wait_ns, period_ns ):
    wait_ns_bytes = list( wait_ns.to_bytes( 4, 'little', signed=False ) )
    period_ns_bytes = list( period_ns.to_bytes( 4, 'little', signed=False ) )
    self.packet_write( 2, wait_ns_bytes + period_ns_bytes )

  def set_hold( self, hold ):
    self.packet_write( 3, [1 if hold else 0] )

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

#spi = spi_init( 0, 0, 2, 500000 )
#strobe = PiStrobe( spi )
#strobe.set_timing( 2000000, 2000000 )
#strobe.set_hold( True )
#time.sleep( 1 )
#strobe.set_hold( False )
#spi.close()

#spi.readbytes(n)
