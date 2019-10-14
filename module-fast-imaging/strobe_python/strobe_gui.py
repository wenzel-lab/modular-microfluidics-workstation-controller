import spidev
#from pistrobe import PiStrobe
#from picamera import PiCamera
from pistrobecam import PiStrobeCam
from time import sleep
from guizero import App, Text, Slider, PushButton, MenuBar, TextBox

WINDOW_X = 1024
WINDOW_Y = 768
PREVIEW_X = 500
PREVIEW_Y = 375

running = True
strobe_period_ns = 10000000

def spi_init( bus, device, mode, speed_hz ):
  spi = spidev.SpiDev()
  spi.open( bus, device )
  spi.mode = mode
  spi.max_speed_hz = speed_hz
  #spi.no_cs = False
  #spi.cshigh = False
  return spi

def update_strobe():
  global strobe_period_ns
  global strobe_cam
  strobe_time_slider.value = strobe_period_ns / 1000
  strobe_time_box.value = strobe_time_slider.value
#  strobe_cam.set_timing( 32, strobe_period_ns, 24000000 )
  
  get_cam_read_time_us = 10000
  get_cam_read_time_us_prev = 0
  strobe_post_padding_ns = 1000000
  while ( abs( get_cam_read_time_us - get_cam_read_time_us_prev ) > 1000 ):
    get_cam_read_time_us_prev = get_cam_read_time_us
    strobe_cam.set_timing( 32, strobe_period_ns, strobe_post_padding_ns )
    print( "strobe wait={}ns, strobe period={}ns, strobe padding={}ns".format( strobe_cam.strobe_wait_ns, strobe_cam.strobe_period_ns, strobe_post_padding_ns ) )
    valid, get_cam_read_time_us = strobe_cam.strobe.get_cam_read_time();
    print( "get_cam_read_time_us={}".format( get_cam_read_time_us ) )
    strobe_post_padding_ns = ( get_cam_read_time_us + 100 ) * 1000

def strobe_time_slider_cmd( value ):
  # Writes slider value to strobe
  global strobe_period_ns
  global strobe_time_slider
  strobe_period_ns = int( value ) * 1000
#  strobe_time_box.value = strobe_time_slider.value
  try:
    update_strobe()
  except:
    pass

def strobe_time_box_cmd( key ):
  global strobe_period_ns
  try:
    strobe_period_ns = int( strobe_time_box.value ) * 1000
    update_strobe()
  except:
    strobe_time_box.value = int( strobe_period_ns / 1000 )

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

#exit_btn = PushButton( app, command=exit, text="Exit", grid=[3,0], align="left" )
Text( app, text="Strobe Period (us)", grid=[0, 1], align="left" )
strobe_time_box = TextBox( app, grid=[0, 2], align="left", command=strobe_time_box_cmd )
#strobe_time_box = TextBox( app, grid=[0, 2], align="left" )
#strobe_time_box.after( 1000, strobe_time_box_cmd )
strobe_time_slider = Slider( app, grid=[0, 3], align="left", command=strobe_time_slider_cmd, start=1, end=16000 )
strobe_time_slider.value=strobe_period_ns / 1000

#app.set_full_screen('')
app.full_screen = True
app.show()
app.update()

spi = spi_init( 0, 0, 2, 125000 )

strobe_cam = PiStrobeCam( spi, 0.1 )
#strobe_cam.camera.resolution = ( 640, 400 )
#strobe_cam.camera.resolution = ( 320, 200 )
#strobe_cam.camera.sensor_mode = 4
#print( 'Resolution: {}'.format( strobe_cam.camera.resolution ) )
update_strobe()

#strobe_cam.camera.zoom = ( 0.4, 0.4, 0.6, 0.6 )
strobe_cam.camera.start_preview( fullscreen=False, window=(WINDOW_X-PREVIEW_X+1,WINDOW_Y-PREVIEW_Y,PREVIEW_X,PREVIEW_Y) )

while running:
  app.update()

#input()
#camera.capture('capture.jpg')
#camera.start_recording('')
strobe_cam.camera.stop_preview()
#strobe_cam.close()
#sleep(2)

strobe_cam.strobe.set_enable( False )
strobe_cam.close()
spi.close()
app.hide()

