from time import sleep
from guizero import App, Text, Slider, PushButton, MenuBar, TextBox
import spidev
import picommon
import piholder

WINDOW_X = 1024
WINDOW_Y = 600
PREVIEW_X = 500
PREVIEW_Y = 375

running = True
temp_c_target = 0

def update_gui():
  global temp_c_target
  
  valid, pid_status, pid_error = holder.get_pid_status()
  valid, temp_c = holder.get_temp_actual()
  
  if ( pid_status == 4 ):
    if ( pid_error == 2 ):
      holder_status_text.value = "No Sensor"
    else:
      holder_status_text.value = "Error ".format( pid_error )
  else:
    holder_status_text.value = "{}".format( pid_status )
  
  holder_temp.value = "{} / {}".format( round( temp_c, 2 ), round( temp_c_target, 2 ) )

def exit():
  global running
  running = False

def get_temp():
  valid, temp_c_target = holder.get_temp_target()
  if valid:
    temp_c_target = round( temp_c_target, 2 )
    strobe_time_box.value = temp_c_target
  return temp_c_target

def set_temp():
  global temp_c_target
  try:
    temp = round( float( strobe_time_box.value ), 2 )
    holder.set_pid_running( 1, temp )
    temp_c_target = get_temp()
  except:
    pass  

# GUIZERO
app = App( title="Microfluidics Station", width=WINDOW_X, height=WINDOW_Y, layout="grid", visible=False, bg="white" )
menubar = MenuBar( app,
                   toplevel=["File"],
                   options=[
                       [ ["Exit", exit] ],
                   ])

locx = 0;
locy = 0;
heater_num = 1;

Text( app, grid=[locx+0, locy+0], text="Heater {}".format(heater_num), align="left" )

Text( app, grid=[locx+0, locy+1], text="Status", align="left" )
holder_status_text = Text( app, grid=[locx+1, locy+1], align="left" )

Text( app, grid=[locx+0, locy+2], text="Temp", align="left" )
holder_temp = Text( app, grid=[locx+1, locy+2], align="left" )

strobe_time_box = TextBox( app, grid=[locx+2, locy+2], align="left" )
set_temp_btn = PushButton( app, command=set_temp, text="Set Temp", grid=[locx+3, locy+2], align="left" )

#app.set_full_screen('')
app.full_screen = True
app.show()
app.update()

picommon.spi_init( 0, 2, 30000 )
holder = piholder.PiHolder( picommon.PORT_HEATER1, 0.2 )

valid = False
while not valid:
  print( "Reading ID..." )
  valid, id, id_valid = holder.get_id()
  sleep( 0.5 )
print( valid, id, id_valid )

temp_c_target = get_temp()

while running:
  update_gui()
  app.update()

picommon.spi_close()
app.hide()

