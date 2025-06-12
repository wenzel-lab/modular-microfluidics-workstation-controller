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
    self.status_text            = [ ("Init")  for i in range(self.flow.NUM_CONTROLLERS) ]
    self.pressure_mbar_text     = [ ("")      for i in range(self.flow.NUM_CONTROLLERS) ]
    self.pressure_mbar_targets  = [ (0.00)    for i in range(self.flow.NUM_CONTROLLERS) ]
    self.flow_ul_hr_text        = [ ("")      for i in range(self.flow.NUM_CONTROLLERS) ]
    self.control_modes          = [ (0)       for i in range(self.flow.NUM_CONTROLLERS) ]
    self.control_modes_text     = [ ("")      for i in range(self.flow.NUM_CONTROLLERS) ]
    
    # --- track fluid type per channel ---
    # Options: 'water', 'fluorinated', 'mineral'
    self.fluid_types = ['water'] * self.flow.NUM_CONTROLLERS
    
    valid, id, id_valid = self.flow.get_id()
    print( "ID OK:{}, ID={}".format( id_valid, id ) )
    self.enabled = valid and id_valid
    self.get_pressure_targets()
    self.get_control_modes()

  # --- for setting flow rate targets from the backend ---
  def set_flow_target(self, index, flow_ul_hr):
      try:
          flows = [0] * self.flow.NUM_CONTROLLERS
          flows[index] = int(flow_ul_hr)

          # 1. Set flow target via SPI
          result = self.flow.set_flow_target([index], flows)

          # 2. Set control mode to Flow Closed Loop (3)
          self.set_control_mode(index, 3)

          return result
      except Exception as e:
          print("Error setting flow target:", e)
          return False

  def get_pressure_targets( self ):
    valid, pressures_mbar_targets = self.flow.get_pressure_target()
    if valid:
      self.pressure_mbar_targets = pressures_mbar_targets

  def get_control_modes( self ):
    valid, control_modes = self.flow.get_control_modes()
    if valid:
      self.control_modes = control_modes
      self.control_modes_text = [ (self.ctrl_mode_str[control_mode]) for control_mode in control_modes ]

  def set_pressure( self, index, pressure_mbar ):
    try:
      pressure = int( pressure_mbar )
      self.flow.set_pressure( [index], [pressure] )
      self.get_pressure_targets()
    except:
      pass
  
  def set_control_mode( self, index, control_mode ):
    try:
      self.flow.set_control_mode( [index], [control_mode] )
      self.get_control_modes()
    except:
      pass
  
  def set_pid_running( self, run ):
    try:
      self.holder.set_pid_running( run )
    except:
      pass
  
  def update( self ):
        if not self.enabled:
            self.status_text = ["Offline"] * self.flow.NUM_CONTROLLERS
            return
        # Read actual pressures and flows
        ok1, pressures = self.flow.get_pressure_actual()
        ok2, flows_raw = self.flow.get_flow_actual()
        if not (ok1 and ok2):
            self.status_text = ["Connection Error"] * self.flow.NUM_CONTROLLERS
            return
        self.pressures_actual = pressures
        self.flows_actual     = flows_raw
        self.status_text      = ["Connected"] * self.flow.NUM_CONTROLLERS    
    

        
        # Prepare calibrated flow readings
        calibrated = []
        for i in range(self.flow.NUM_CONTROLLERS):
            raw = self.flows_actual[i]
            ft  = self.fluid_types[i]
            if   ft=='water':
                 cal = raw
            elif ft=='fluorinated':
                 # Cubic calibration for Novec oil
                 cal = (5.92715985e-06 * raw**3
                       + 2.26989221e-03 * raw**2
                       + 9.84302695e-01 * raw
                       + 7.92965050e+00)
            elif ft=='mineral':
                 # Cubic calibration for mineral oil
                 cal = (-2.71749525e-07 * raw**3
                       + 2.36655640e-03 * raw**2
                       + 8.39461407e-01 * raw
                       + 3.46965708e+01)
            else:
                 cal = raw
            calibrated.append(cal)
        
        # Format display text
        self.pressure_mbar_text = [f"{p:.2f} / {t:.2f}" \
                                   for p, t in zip(self.pressures_actual, self.pressure_mbar_targets)]
        self.flow_ul_hr_text    = [f"{c:.2f}" for c in calibrated]
