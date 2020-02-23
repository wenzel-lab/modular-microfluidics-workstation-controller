from time import sleep
from guizero import App, Text, Slider, PushButton, MenuBar, TextBox, Box, Window
import spidev
import picommon
import piholder
from pistrobecam import PiStrobeCam

WINDOW_X = 1024
WINDOW_Y = 600
PREVIEW_X = 470
PREVIEW_Y = 275

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

def update_gui():
  strobe_module.update()

def exit():
  global running
  running = False

class strobe_gui:
  set_timing_done = False

  def __init__( self, port, locx, locy ):
    self.screen_width = app.tk.winfo_screenwidth()
    self.screen_height = app.tk.winfo_screenheight()
#    print( "Width {}, Height {}".format( self.screen_width, self.screen_height ) )
    
    self.locx = locx
    self.locy = locy
    self.strobe_cam = PiStrobeCam( port, 0.2 )
    
    valid = self.strobe_cam.strobe.set_enable( False )
    self.enabled = valid
    
    box=Box( app, layout="grid", grid=[locx, locy] )
    box.set_border( 1, "black" )
    box.bg = col_lightgray1 if ( ( locx + locy ) % 2 == 0 ) else col_lightgray2
    
    Text( box, grid=[0, 0], text="Strobe", align="left" )
    Box( box, grid=[1, 0], width=120, height=1 )
    Box( box, grid=[2, 0], width=80, height=1 )
    Box( box, grid=[3, 0], width=80, height=1 )

    Text( box, grid=[0, 1], text="Status:", align="left" )
    self.strobe_status_text = Text( box, grid=[1, 1], align="left" )
    
    Text( box, grid=[0, 2], text="Period:", align="left" )
    self.strobe_time_text = Text( box, grid=[1, 2], align="left" )
    self.strobe_time_text.value = "N/A"
    self.strobe_time_box = TextBox( box, grid=[2, 2], align="left" )
    self.strobe_time_box.value = "1000"
    self.set_timing_btn = PushButton( box, command=self.set_timing, text="Set", grid=[3, 2], width=12, align="left", pady=1 )
    
    Text( box, grid=[0, 3], text="Framerate:", align="left" )
    self.strobe_framerate_text = Text( box, grid=[1, 3], align="left" )
    self.optimize_fps_btn = PushButton( box, command=self.optimize_fps, text="Optimize", grid=[3, 3], width=12, align="left", pady=1, visible=False )
    
    self.snapshot_btn = PushButton( box, command=self.save_snapshot, text="Snapshot", grid=[3, 4], width=12, align="left", pady=1 )
    
#    Text( box, grid=[0, 4], text="Red Back Time:", align="left" )
#    self.strobe_readtime_text = Text( box, grid=[1, 4], align="left" )
    
#    Box( box, grid=[4, 4], width=10, height=10 )
#    Box( box, grid=[0, 4], width=80, height=1 )
    
    self.strobe_cam.camera.start_preview( fullscreen=False, window=(self.screen_width-PREVIEW_X+1,self.screen_height-PREVIEW_Y,PREVIEW_X,PREVIEW_Y) )
    
  def update( self ):
    if not self.enabled:
      self.strobe_status_text.value = "Offline"
    else:
      self.strobe_status_text.value = "Online"
    
    self.strobe_framerate_text.value = "{} fps".format( int( self.strobe_cam.camera.framerate ) )
#    self.strobe_framerate_text.value = "{} fps".format( int( self.strobe_cam.framerate_set ) )
  
  def set_timing( self ):
    try:
      self.strobe_period_ns = int( round( float( self.strobe_time_box.value ) * 1000, 0 ) )
      self.strobe_period_ns = min( self.strobe_period_ns, 16000000 )
      self.strobe_cam.set_timing( 32, self.strobe_period_ns, 20000000 )
      self.strobe_time_text.value = "{} us".format( round( float( self.strobe_cam.strobe_period_ns ) / 1000, 3 ) )
      self.set_timing_done = True
      self.optimize_fps_btn.visible = True
    except:
      pass
  
  def optimize_fps( self ):
    get_cam_read_time_us = 10000
    get_cam_read_time_us_prev = 0
    strobe_post_padding_ns = 1000000
    while ( abs( get_cam_read_time_us - get_cam_read_time_us_prev ) > 1000 ):
      self.strobe_cam.camera.stop_preview()
      get_cam_read_time_us_prev = get_cam_read_time_us
      self.strobe_cam.set_timing( 32, self.strobe_period_ns, strobe_post_padding_ns )
#      print( "strobe wait={}ns, strobe period={}ns, strobe padding={}ns".format( self.strobe_cam.strobe_wait_ns, self.strobe_cam.strobe_period_ns, strobe_post_padding_ns ) )
      valid, get_cam_read_time_us = self.strobe_cam.strobe.get_cam_read_time();
#      print( "get_cam_read_time_us={}".format( get_cam_read_time_us ) )
      strobe_post_padding_ns = ( get_cam_read_time_us + 100 ) * 1000
      self.strobe_cam.camera.start_preview( fullscreen=False, window=(self.screen_width-PREVIEW_X+1,self.screen_height-PREVIEW_Y,PREVIEW_X,PREVIEW_Y) )
  
  def save_snapshot( self ):
    self.strobe_cam.camera.capture( "snapshot.jpg" )

picommon.spi_init( 0, 2, 30000 )

strobe_module = strobe_gui( picommon.PORT_STROBE, 0, 0 )

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
  strobe_module.strobe_cam.camera.stop_preview()
  strobe_module.strobe_cam.strobe.set_enable( False )
  strobe_module.strobe_cam.close()
  picommon.spi_close()
  app.destroy()
except:
  pass
