import time
import spidev
import picommon

class PiHolder:
  DEVICE_ID                         = 'SAMPLE_HOLDER'
  STX                               = 2
  PRESSURE_SHIFT                    = 3
  TEMP_SCALE                        = 100
  PRESSURE_SCALE                    = ( 1 << PRESSURE_SHIFT )
  PACKET_TYPE_GET_ID                = 1
  PACKET_TYPE_TEMP_SET_TARGET       = 2
  PACKET_TYPE_TEMP_GET_TARGET       = 3
  PACKET_TYPE_TEMP_GET_ACTUAL       = 4
  PACKET_TYPE_PID_SET_COEFFS        = 5
  PACKET_TYPE_PID_GET_COEFFS        = 6
  PACKET_TYPE_PID_SET_RUNNING       = 7
  PACKET_TYPE_PID_GET_STATUS        = 8
  PACKET_TYPE_AUTOTUNE_SET_RUNNING  = 9
  PACKET_TYPE_AUTOTUNE_GET_RUNNING  = 10
  PACKET_TYPE_AUTOTUNE_GET_STATUS   = 11
  
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
    picommon.spi_deselect_current()
    return valid, type, data
  
  def packet_write( self, type, data ):
    msg = [2, len(data)+4, type] + data
    checksum = ( -( sum( msg ) & 0xFF ) ) & 0xFF
    msg.append( checksum )
    picommon.spi_select_device( self.device_port )
    picommon.spi.xfer2( msg )
    picommon.spi_deselect_current()
    
  def packet_query( self, type, data ):
    self.packet_write( type, data )
    time.sleep( self.reply_pause_s )
    valid = True
    data_read = []
    type_read = 0x100
    try:
      while valid and ( type_read != type ) and ( type_read != 0 ):
        valid, type_read, data_read = self.packet_read()
    except:
      valid = False
    return valid, data_read
    
  def get_id( self ):
#    self.packet_write( self.PACKET_TYPE_GET_ID, [] )
#    time.sleep( self.reply_pause_s )
#    valid, data = self.packet_read()
    valid, data = self.packet_query( self.PACKET_TYPE_GET_ID, [] )
    id = bytes(data[1:-1]).decode("ascii")
    id_valid = ( id == self.DEVICE_ID )
    try:
      checksum_okay = data[0] == 0
    except:
      checksum_okay = False
    return ( valid and checksum_okay, id, id_valid )

  def get_temp_target( self ):
#    self.packet_write( self.PACKET_TYPE_TEMP_GET_TARGET, [] )
#    time.sleep( self.reply_pause_s )
#    valid, data = self.packet_read()
    valid, data = self.packet_query( self.PACKET_TYPE_TEMP_GET_TARGET, [] )
    temp_c = int.from_bytes( data[1:3], byteorder='big', signed=True ) / self.TEMP_SCALE
    return ( valid and ( data[0] == 0 ), temp_c )

  def set_pid_coeffs( self, pid_p, pid_i, pid_d ):
    send_bytes = list( pid_p.to_bytes( 2, 'little', signed=False ) )
    send_bytes.extend( list( pid_i.to_bytes( 2, 'little', signed=False ) ) )
    send_bytes.extend( list( pid_d.to_bytes( 2, 'little', signed=False ) ) )
    valid, data = self.packet_query( self.PACKET_TYPE_PID_SET_COEFFS, send_bytes )
    return ( ( valid and ( data[0] == 0 ) ) )

  def get_pid_coeffs( self ):
    valid, data = self.packet_query( self.PACKET_TYPE_PID_GET_COEFFS, [] )
    pid_p = int.from_bytes( data[1:3], byteorder='big', signed=False )
    pid_i = int.from_bytes( data[3:5], byteorder='big', signed=False )
    pid_d = int.from_bytes( data[5:7], byteorder='big', signed=False )
    return ( valid and ( data[0] == 0 ), pid_p, pid_i, pid_d )

  def set_pid_running( self, running, temp_c ):
    temp_c_scaled = round( temp_c * 100 )
    send_bytes = list( running.to_bytes( 1, 'little', signed=False ) )
    send_bytes.extend( list( temp_c_scaled.to_bytes( 2, 'little', signed=True ) ) )
    valid, data = self.packet_query( self.PACKET_TYPE_PID_SET_RUNNING, send_bytes )
    return ( ( valid and ( data[0] == 0 ) ) )

  def get_pid_status( self ):
    valid, data = self.packet_query( self.PACKET_TYPE_PID_GET_STATUS, [] )
    pid_status = data[1]
    return ( valid and ( data[0] == 0 ), pid_status )

  def get_temp_actual( self ):
#    self.packet_write( self.PACKET_TYPE_TEMP_GET_ACTUAL, [] )
#    time.sleep( self.reply_pause_s )
#    valid, data = self.packet_read()
    valid, data = self.packet_query( self.PACKET_TYPE_TEMP_GET_ACTUAL, [] )
    temp_c = int.from_bytes( data[1:3], byteorder='big', signed=True ) / self.TEMP_SCALE
    return ( valid and ( data[0] == 0 ), temp_c )

  def set_autotune_running( self, running, temp_c ):
    temp_c_scaled = round( temp_c * 100 )
    send_bytes = list( running.to_bytes( 1, 'little', signed=False ) )
    send_bytes.extend( list( temp_c_scaled.to_bytes( 2, 'little', signed=True ) ) )
    valid, data = self.packet_query( self.PACKET_TYPE_AUTOTUNE_SET_RUNNING, send_bytes )
    return ( ( valid and ( data[0] == 0 ) ) )

  def get_autotune_running( self ):
#    self.packet_write( self.PACKET_TYPE_AUTOTUNE_GET_RUNNING, [] )
#    time.sleep( self.reply_pause_s )
#    valid, data = self.packet_read()
    valid, data = self.packet_query( self.PACKET_TYPE_AUTOTUNE_GET_RUNNING, [] )
    autotune_running = data[1]
    return ( valid and ( data[0] == 0 ), autotune_running )
  
  def get_autotune_status( self ):
#    self.packet_write( self.PACKET_TYPE_AUTOTUNE_GET_STATUS, [] )
#    time.sleep( self.reply_pause_s )
#    valid, data = self.packet_read()
    valid, data = self.packet_query( self.PACKET_TYPE_AUTOTUNE_GET_STATUS, [] )
    autotune_status = data[1]
    autotune_fail = data[2]
    return ( valid and ( data[0] == 0 ), autotune_status, autotune_fail )
