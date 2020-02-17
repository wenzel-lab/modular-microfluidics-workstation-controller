from time import sleep
from guizero import App, Text, Slider, PushButton, MenuBar, TextBox
import spidev
import picommon
import piholder

WINDOW_X = 1024
WINDOW_Y = 600
PREVIEW_X = 500
PREVIEW_Y = 375

global running

running = True

def update_gui():
  heater1.update()
  heater2.update()

def exit():
  global running
  running = False

# GUIZERO
app = App( title="Microfluidics Station", width=WINDOW_X, height=WINDOW_Y, layout="grid", visible=False, bg="white" )
menubar = MenuBar( app,
                   toplevel=["File"],
                   options=[
                       [ ["Exit", exit] ],
                   ])

heater_num = 1;

class heater:

  def __init__( self, heater_num, port, locx, locy ):
    self.locx = locx
    self.locy = locy
    self.holder = piholder.PiHolder( port, 0.2 )
    
    valid, id, id_valid = self.holder.get_id()
    self.enabled = valid and id_valid
    
    Text( app, grid=[self.locx+0, self.locy+0], text="Heater {}".format(heater_num), align="left" )

    Text( app, grid=[self.locx+0, self.locy+1], text="Status", align="left" )
    self.holder_status_text = Text( app, grid=[self.locx+1, self.locy+1], align="left" )

    Text( app, grid=[self.locx+0, self.locy+2], text="Temp", align="left" )
    self.holder_temp = Text( app, grid=[self.locx+1, self.locy+2], align="left" )

    self.temp_target_box = TextBox( app, grid=[self.locx+2, self.locy+2], align="left" )
    self.set_temp_btn = PushButton( app, command=self.set_temp, text="Set Temp", grid=[self.locx+3, self.locy+2], align="left" )
    
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
      self.holder.set_pid_running( 1, temp )
      self.temp_c_target = self.get_temp()
    except:
      pass
  
  def update( self ):
    if not self.enabled:
      self.holder_status_text.value = "Offline"
    else:
      valid, pid_status, pid_error = self.holder.get_pid_status()
      valid, temp_c = self.holder.get_temp_actual()
      
      if ( pid_status == 4 ):
        if ( pid_error == 2 ):
          self.holder_status_text.value = "No Sensor"
        else:
          self.holder_status_text.value = "Error ".format( pid_error )
      else:
        self.holder_status_text.value = "{}".format( pid_status )
      
      self.holder_temp.value = "{} / {}".format( round( temp_c, 2 ), round( self.temp_c_target, 2 ) )

picommon.spi_init( 0, 2, 30000 )

heater1 = heater( 1, picommon.PORT_HEATER1, 0, 0 )
heater2 = heater( 2, picommon.PORT_HEATER2, 0, 5 )

#app.set_full_screen('')
app.full_screen = True
app.when_closed = exit
app.show()
app.update()

while running:
  update_gui()
  app.update()

picommon.spi_close()
app.destroy()
