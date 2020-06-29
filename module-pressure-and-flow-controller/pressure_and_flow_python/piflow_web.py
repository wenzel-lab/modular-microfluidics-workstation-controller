import picommon
import piflow

class flow_web:
  press_status_str = [  "Unconfigured",
                        "Idle",
                        "Active",
                        "Error"
                     ]

  flow_status_str  = [  "Unconfigured",
                        "Idle",
                        "Active",
                        "Error"
                     ]

  ctrl_mode_str    = [  "Off",
                        "Pressure Open Loop",
                        "Pressure Closed Loop",
                        "Flow Closed Loop"
                     ]

  def __init__( self, port ):
    self.flow = piflow.PiFlow( port, 0.1 )
    
#    self.pid_enabled = False
    self.status_text = [ ("Init") for i in range (self.flow.NUM_CONTROLLERS) ]
    self.pressure_mbar_text = [ ("") for i in range (self.flow.NUM_CONTROLLERS) ]
    self.pressures_target = [ (0.00) for i in range (self.flow.NUM_CONTROLLERS) ]
    
    valid, id, id_valid = self.flow.get_id()
    print( "ID OK:{}, ID={}".format( id_valid, id ) )
    self.enabled = valid and id_valid
    
    self.get_pressures_target()

  def get_pressures_target( self ):
    valid, pressures_mbar_target = self.flow.get_pressure_target()
    if valid:
      self.pressures_target = pressures_mbar_target

  def set_pressure( self, index, pressure_mbar ):
    try:
#      pressure = round( float( pressure_mbar ), 2 )
      pressure = int( pressure_mbar )
      self.flow.set_pressure( [index], [pressure] )
#      self.pressures_target[index] = self.get_temp_target()
#      self.pressures_target[index] = pressure
    except:
      pass
  
  def set_pid_running( self, run ):
    try:
      #run = 0 if self.pid_enabled else 1
      #temp = round( float( self.temp_target_box.value ), 2 )
      #self.holder.set_pid_running( run, temp )
      self.holder.set_pid_running( run )
      #self.temp_c_target = self.get_temp_target()
    except:
      pass
  
  def update( self ):
    if not self.enabled:
      self.status_text = [ ( "Offline" ) for i in range (self.flow.NUM_CONTROLLERS) ]
#      self.status_text = "Offline"
    else:
#      valid, pid_status, pid_error = self.holder.get_pid_status()
      valid = True
      
      okay = valid
      valid, pressures_actual = self.flow.get_pressure_actual();
      if valid:
        self.pressures_actual = pressures_actual
#        self.temp_c_actual = round( temp_c, 2 )
      okay = okay and valid
      
      if not okay:
        self.status_text = [ ( "Connection Error" ) for i in range (self.flow.NUM_CONTROLLERS) ]
#        self.status_text = "Connection Error"
      else:
#        if ( pid_status == 4 ):
#          if ( pid_error == 2 ):
#            self.status_text = "No Sensor"
#          else:
#            self.status_text = "Error ".format( pid_error )
#        else:
          try:
#            self.status_text = "{}".format( self.pid_status_str[pid_status] )
            self.status_text = [ ( "Connected" ) for i in range (self.flow.NUM_CONTROLLERS) ]
          except:
            pass
      
      try:
        self.pressure_mbar_text = [ ( "{} / {}".format( round( self.pressures_actual[i], 2 ), round( self.pressures_target[i], 2 ) ) ) for i in range (self.flow.NUM_CONTROLLERS) ]
#        self.pressure_mbar_text = [ ( "{} / {}".format( 0.00, 1.00 ) ) for i in range (self.flow.NUM_CONTROLLERS) ]
      except:
        pass
      
#      self.pid_enabled = ( pid_status == 2 )
      
#    if ( not self.pid_enabled ):
#      self.set_pid_enable_btn.text = "Enable PID"
#    else:
#      self.set_pid_enable_btn.text = "Disable PID"
    
    try:
#      self.autotune_status_text = "{}".format( self.autotune_status_str[autotune_status] )
      pass
    except:
      pass
