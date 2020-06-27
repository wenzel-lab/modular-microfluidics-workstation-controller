import time
import spidev
import picommon

class PiFlow:
  DEVICE_ID                       = 'MICROFLOW'
  STX                             = 2
  PRESSURE_SHIFT                  = 3
  PRESSURE_SCALE                  = ( 1 << PRESSURE_SHIFT )
  
  PACKET_TYPE_GET_ID              = 1
  PACKET_TYPE_SET_PRESSURE_TARGET = 2
  PACKET_TYPE_GET_PRESSURE_TARGET = 3
  PACKET_TYPE_GET_PRESSURE_ACTUAL = 4
  PACKET_TYPE_SET_FLOW_TARGET     = 5
  PACKET_TYPE_GET_FLOW_TARGET     = 6
  PACKET_TYPE_GET_FLOW_ACTUAL     = 7
  PACKET_TYPE_SET_CONTROL_MODE    = 8
  PACKET_TYPE_GET_CONTROL_MODE    = 9
  
  NUM_CONTROLLERS                 = 4
  
  def __init__( self, device_port, reply_pause_s ):
    self.device_port = device_port
    self.reply_pause_s = reply_pause_s
  
  def read_bytes( self, bytes ):
    data = []
    for x in range( bytes ):
      data.extend( picommon.spi.xfer2( [0] ) )
    return data
  
  def packet_read( self ):
    valid = False
    picommon.spi_select_device( self.device_port )
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
    return valid, type, data
  
  def packet_write( self, type, data ):
    msg = [2, len(data)+4, type] + data
    checksum = ( -( sum( msg ) & 0xFF ) ) & 0xFF
    msg.append( checksum )
    picommon.spi_select_device( self.device_port )
    picommon.spi.xfer2( msg )
    
  def packet_query( self, type, data ):
    try:
      picommon.spi_lock()
      self.packet_write( type, data )
#      time.sleep( self.reply_pause_s )
      picommon.pi_wait_s( self.reply_pause_s )
      valid = True
      data_read = []
      type_read = 0x100
      try:
        while valid and ( type_read != type ) and ( type_read != 0 ):
          valid, type_read, data_read = self.packet_read()
      except:
        valid = False
      picommon.spi_deselect_current()
    except:
      pass
    finally:
      picommon.spi_release()
    return valid, data_read
    
  def get_id( self ):
    valid, data = self.packet_query( self.PACKET_TYPE_GET_ID, [] )
    if valid:
      id = bytes(data[1:-1]).decode("ascii")
      id_valid = ( id == self.DEVICE_ID )
      try:
        checksum_okay = data[0] == 0
      except:
        checksum_okay = False
    else:
      id = 0
      id_valid = False
    return ( valid and checksum_okay, id, id_valid )

  def set_pressure( self, pressures_mbar ):
    pressures_mbar_bytes = []
    for i in range(self.NUM_CONTROLLERS):
      mask = 1 << i
      pressure_fp = int( pressures_mbar[i] * self.PRESSURE_SCALE );
      pressures_mbar_bytes.extend( [mask] + list( pressure_fp.to_bytes( 2, 'little', signed=False ) ) )
    valid, data = self.packet_query( self.PACKET_TYPE_SET_PRESSURE_TARGET, pressures_mbar_bytes )
#    actual_wait_ns = int.from_bytes( data[1:5], byteorder='little', signed=False )
#    print( "data={}, wait={}, period={}, wait_bytes={}".format( data, actual_wait_ns, actual_period_ns, data[1:5] ) )
    return ( ( valid and ( data[0] == 0 ) ) )

  def get_pressure_actual( self ):
    valid, data = self.packet_query( self.PACKET_TYPE_GET_PRESSURE_ACTUAL, [] )
    count = int( ( len(data) - 1 ) / 2 )
    pressures=[]
    for i in range(count):
      index = 1 + ( i << 1 )
      pressure = int.from_bytes( data[index:index+2], byteorder='little', signed=False ) / self.PRESSURE_SCALE
      pressures.extend( [pressure] )
    return ( valid and ( data[0] == 0 ), pressures )

