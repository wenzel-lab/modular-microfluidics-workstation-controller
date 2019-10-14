import time
import spidev

class PiStrobe:
  STX = 2
  
  def __init__( self, spi, reply_pause_s ):
    self.spi = spi
    self.reply_pause_s = reply_pause_s
  
  def read_bytes( self, bytes ):
    data = []
    for x in range( bytes ):
      data.extend( self.spi.xfer2( [0] ) )
    return data
  
  def packet_read( self ):
    valid = False
    data = self.read_bytes( 1 )
    if ( data[0] == self.STX ):
      data.extend( self.read_bytes( 2 ) )
      size = data[1]
      type = data[2]
      data.extend( self.read_bytes( size - 3 ) )
      checksum = sum( data ) & 0xFF
      if ( checksum == 0 ):
        data = data[3:(size-1)]
        valid = True
    if not valid:
      data = []
    return valid, data
  
  def packet_write( self, type, data ):
    msg = [2, len(data)+4, type] + data
    checksum = ( -( sum( msg ) & 0xFF ) ) & 0xFF
    msg.append( checksum )
    self.spi.xfer2( msg )
    
  def set_enable( self, enable ):
    self.packet_write( 1, [1 if enable else 0] )
    time.sleep( self.reply_pause_s )
    valid, data = self.packet_read()
    return ( valid and ( data[0] == 0 ) )

  def set_timing( self, wait_ns, period_ns ):
    wait_ns_bytes = list( wait_ns.to_bytes( 4, 'little', signed=False ) )
    period_ns_bytes = list( period_ns.to_bytes( 4, 'little', signed=False ) )
    self.packet_write( 2, wait_ns_bytes + period_ns_bytes )
    time.sleep( self.reply_pause_s )
    valid, data = self.packet_read()
    actual_wait_ns = int.from_bytes( data[1:5], byteorder='little', signed=False )
    actual_period_ns = int.from_bytes( data[5:9], byteorder='little', signed=False )
#    print( "data={}, wait={}, period={}, wait_bytes={}".format( data, actual_wait_ns, actual_period_ns, data[1:5] ) )
    return ( ( valid and ( data[0] == 0 ) ), actual_wait_ns, actual_period_ns )

  def set_hold( self, hold ):
    self.packet_write( 3, [1 if hold else 0] )
    time.sleep( self.reply_pause_s )
    valid, data = self.packet_read()
    return ( valid and ( data[0] == 0 ) )

  def get_cam_read_time( self ):
    self.packet_write( 4, [] )
    time.sleep( self.reply_pause_s )
    valid, data = self.packet_read()
    cam_read_time_us = int.from_bytes( data[1:3], byteorder='little', signed=False )
    return ( valid and ( data[0] == 0 ), cam_read_time_us )

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
