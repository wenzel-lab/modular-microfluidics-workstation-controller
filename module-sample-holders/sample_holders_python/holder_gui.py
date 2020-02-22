from time import sleep
from guizero import App, Text, Slider, PushButton, MenuBar, TextBox, Box, Window
import spidev
import picommon
import piholder

WINDOW_X = 1024
WINDOW_Y = 600
PREVIEW_X = 500
PREVIEW_Y = 375

col_lightgray1 = "#A0A0A0"
col_lightgray2 = "#C0C0C0"

global running
running = True

app = App( title="Microfluidics Station", width=WINDOW_X, height=WINDOW_Y, layout="grid", visible=False, bg="white" )
menubar = MenuBar( app,
                   toplevel=["File"],
                   options=[
                       [ ["Exit", exit] ],
                   ])

pid_status_str = [ "Unconfigured",
                   "Idle",
                   "Heating",
                   "Suspended",
                   "Error"
                 ]

autotune_status_str = [ "None",
                        "Running",
                        "Aborted",
                        "Finished",
                        "Failed"
                 ]

def update_gui():
  heater1.update()
  heater2.update()
  heater3.update()
  heater4.update()

def exit():
  global running
  running = False

class heater:

  def __init__( self, heater_num, port, locx, locy ):
    self.locx = locx
    self.locy = locy
    self.holder = piholder.PiHolder( port, 0.2 )
    self.autotuning = False
    self.pid_enabled = False
    
    valid, id, id_valid = self.holder.get_id()
    self.enabled = valid and id_valid
    
    box=Box( app, layout="grid", grid=[locx, locy] )
    box.set_border( 1, "black" )
    box.bg = col_lightgray1 if ( ( locx + locy ) % 2 == 0 ) else col_lightgray2
    
    Text( box, grid=[0, 0], text="Heater {}".format(heater_num), align="left" )

    Text( box, grid=[0, 1], text="Status:", align="left" )
    self.holder_status_text = Text( box, grid=[1, 1], align="left" )
    Box( box, grid=[1, 0], width=120, height=1 )
    Box( box, grid=[2, 0], width=80, height=1 )
    Box( box, grid=[3, 0], width=80, height=1 )

    Text( box, grid=[0, 3], text="Autotune:", align="left" )
    self.autotune_status_text = Text( box, grid=[1, 3], align="left" )

    Text( box, grid=[0, 2], text="Temp:", align="left" )
    self.holder_temp = Text( box, grid=[1, 2], align="left" )

    self.temp_target_box = TextBox( box, grid=[2, 2], align="left", width=6 )
    self.set_temp_btn = PushButton( box, command=self.set_temp, text="Set Temp", grid=[3, 2], width=12, align="left", pady=1 )
    self.set_pid_enable_btn = PushButton( box, command=self.set_pid_running, text="", grid=[3, 1], width=12, align="left", pady=1 )
    
    self.autotune_target_box = TextBox( box, grid=[2, 3], align="left", width=6 )
    self.autotune_target_box.value = "50.00"
    self.autotune_btn = PushButton( box, command=self.set_autotune, text="", grid=[3, 3], width=12, align="left", pady=1 )
    
#    spacer=Box( box, grid=[0, 4], width=100, height=10 )
#    spacer.bg="black"
    Box( box, grid=[4, 4], width=10, height=10 )
    Box( box, grid=[0, 4], width=80, height=1 )
    
    self.temp_c_target = self.get_temp()

  def get_temp( self ):
    valid, temp_c_target = self.holder.get_temp_target()
    if valid:
      temp_c_target = round( temp_c_target, 2 )
      self.temp_target_box.value = temp_c_target
    self.temp_c_target = temp_c_target
    return temp_c_target

  def set_temp( self ):
    try:
      temp = round( float( self.temp_target_box.value ), 2 )
      self.holder.set_pid_temp( temp )
      self.temp_c_target = self.get_temp()
    except:
      pass
  
  def set_autotune( self ):
    try:
      temp = round( float( self.autotune_target_box.value ), 2 )
      autotuning = 0 if self.autotuning else 1
      self.holder.set_autotune_running( autotuning, temp )
    except:
      pass
    
  def set_pid_running( self ):
    try:
      run = 0 if self.pid_enabled else 1
      temp = round( float( self.temp_target_box.value ), 2 )
      self.holder.set_pid_running( run, temp )
      self.temp_c_target = self.get_temp()
    except:
      pass
  
  def update( self ):
    if not self.enabled:
      self.holder_status_text.value = "Offline"
    else:
      valid, pid_status, pid_error = self.holder.get_pid_status()
      valid, temp_c = self.holder.get_temp_actual()
      valid, autotune_status, autotune_fail = self.holder.get_autotune_status()
      
      self.autotuning = ( autotune_status == 1 )
      self.autotune_status = autotune_status
      
      if self.autotuning:
        self.holder_status_text.value = "Autotuning"
      else:
        if ( pid_status == 4 ):
          if ( pid_error == 2 ):
            self.holder_status_text.value = "No Sensor"
          else:
            self.holder_status_text.value = "Error ".format( pid_error )
        else:
          try:
            self.holder_status_text.value = "{}".format( pid_status_str[pid_status] )
          except:
            pass
      
      self.holder_temp.value = "{} / {}".format( round( temp_c, 2 ), round( self.temp_c_target, 2 ) )
      
      self.pid_enabled = ( pid_status == 2 )
      
    if ( not self.pid_enabled ):
      self.set_pid_enable_btn.text = "Enable PID"
    else:
      self.set_pid_enable_btn.text = "Disable PID"
    
    if ( not self.autotuning ):
      self.autotune_btn.text = "Start Autotune"
    else:
      self.autotune_btn.text = "Abort Autotune"
    
    try:
      self.autotune_status_text.value = "{}".format( autotune_status_str[autotune_status] )
    except:
      pass

picommon.spi_init( 0, 2, 30000 )

heater1 = heater( 1, picommon.PORT_HEATER1, 0, 0 )
heater2 = heater( 2, picommon.PORT_HEATER2, 0, 5 )
heater3 = heater( 3, picommon.PORT_HEATER3, 5, 0 )
heater4 = heater( 4, picommon.PORT_HEATER4, 5, 5 )

#app.set_full_screen('')
app.full_screen = True
app.when_closed = exit
app.show()
app.update()

while running:
  try:
    update_gui()
    app.update()
  except:
    running = False
    pass

try:
  picommon.spi_close()
  app.destroy()
except:
  pass
