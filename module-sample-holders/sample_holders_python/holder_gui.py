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

def update_gui():
  valid, pid_status = holder.get_pid_status()
  valid, temp_c = holder.get_temp_actual()
  
  holder_status_text.value = "{}".format( pid_status )
  holder_temp.value = round( temp_c, 2 )

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

Text( app, grid=[0, 0], text="Status", align="left" )
holder_status_text = Text( app, grid=[1, 0], align="left" )

Text( app, grid=[0, 1], text="Temp", align="left" )
holder_temp = Text( app, grid=[1, 1], align="left" )

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

while running:
  update_gui()
  app.update()

picommon.spi_close()
app.hide()

