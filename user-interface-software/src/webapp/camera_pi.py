import io
import threading
import picommon
from pistrobecam import PiStrobeCam
from PIL import Image
from datetime import datetime
import os
import shutil
import time # For initial sleep

class Camera(object):
    thread = None # For background image capture thread
    frame = None # Last captured frame
    
    def __init__( self, exit_event, socketio ):
        
      print( "Init Camera ----------------------" )
      self.exit_event = exit_event
      self.socketio = socketio
      
      # Pre-initialize attributes
      self.cam_read_time_us = 0
      self.cam_data = {
          'camera': 'none',
          'status': '',
          'recording': False,
          'error': ''
      }
      self.recording  = False
      self.video_file = None
      
      # Initialize strobe and camera
      self.strobe_cam = PiStrobeCam(picommon.PORT_STROBE, 0.1)
      self.camera     = self.strobe_cam.camera # Get the PiCamera instance
      
      # 'strobe_data' holds the state for the UI, reflecting actual hardware acknowledged values.
      self.strobe_data = {
        'hold':0,
        'enable':0,
        'wait_ns':0, # Will be updated by set_timing and update_strobe_data
        'period_ns':100000, # Initial desired, will be updated
        'framerate':0, # Actual camera framerate
        'cam_shutter_speed_us':0 # Actual camera shutter speed in µs
        } 
      
      # default strobe settings
      self.strobe_period_ns = int( self.strobe_data['period_ns'] )
      print(f"[Camera.__init__] Default desired strobe_period_ns: {self.strobe_period_ns} ns")
      
      # Initialize strobe hardware to known state (defaults from strobe_data)
      print(f"[Camera.__init__] Setting initial strobe hold: {self.strobe_data['hold']}")
      self.strobe_cam.strobe.set_hold( self.strobe_data['hold'] )
      print(f"[Camera.__init__] Setting initial strobe enable: {self.strobe_data['enable']}")
      self.strobe_cam.strobe.set_enable( self.strobe_data['enable'] )
      
      self.strobe_cam.strobe.set_timing( self.strobe_data['wait_ns'], self.strobe_period_ns ) 
 
      # Apply initial timing settings based on defaults
      print(f"[Camera.__init__] Applying initial timing settings...")
      self.set_timing()
      #self.enabled = valid 
      
      # Populate self.strobe_data with actual values after initial configuration
      print(f"[Camera.__init__] Updating strobe_data with actual values for initial UI.")
      self.update_strobe_data() # Ensures UI gets actual hardware-acknowledged values initially
      print(f"[Camera.__init__] Initialization complete. Initial strobe_data: {self.strobe_data}")
      
      #camera state
      self.cam_read_time_us = 0
      self.cam_data = {
          'camera': 'none',
          'status': '',
          'recording': False,
          'error': ''
      }
      self.recording  = False
      self.video_file = None
      
      # socket handlers
      @self.socketio.on( 'cam' )
      def on_cam( data ):
        self.on_cam( data )
        
      @self.socketio.on( 'strobe' )
      def on_strobe( data ):
        self.on_strobe( data )

    def __del__(self):
        # finalize recording if still open
        if hasattr(self, 'recording') and self.recording:
            try:
                self.camera.stop_recording()
            except:
                pass

    def initialize( self ):
        if self.thread is None:
            # start background frame thread
            self.thread = threading.Thread( target=self._capture_loop )
            self.thread.daemon = True
            self.thread.start()
            while self.frame is None and not self.exit_event.is_set():
                pass

    def get_frame( self ):
      self.initialize()
      return self.frame

    def set_timing(self):
        self.strobe_cam.set_timing(32, self.strobe_period_ns, 20_000_000)
    
    def save( self ):
      img = Image.open(io.BytesIO(self.frame))
      ts  = datetime.now().strftime('%Y-%m-%d_%H-%M-%S')
      folder = "/home/pi/webapp/snapshots"
      os.makedirs(folder, exist_ok=True)
      fn = f"snapshot_{ts}.jpg"
      try:
          img.save(f"{folder}/{fn}", "JPEG")
          self.cam_data.update({'status': f"Saved {fn}", 'error': ''})
      except Exception as e:
          self.cam_data['error'] = f"Save failed: {e}"
      finally:
          self.socketio.emit('cam', self.cam_data)

    def start_record(self):
        if self.recording:
            return

        folder = "/home/pi/webapp/videos"
        os.makedirs(folder, exist_ok=True)
        free_mb = shutil.disk_usage(folder).free / 1024 / 1024
        if free_mb < 100:
            self.cam_data['error'] = "Insufficient disk space"
            return self.socketio.emit('cam', self.cam_data)

        ts = datetime.now().strftime('%Y-%m-%d_%H-%M-%S')
        fn = f"video_{ts}.h264"
        path = os.path.join(folder, fn)

        try:
            # overlay strobe period on each frame
            txt = f"REC {self.strobe_period_ns//1000} us"
            self.camera.annotate_text_size = 32
            self.camera.annotate_text      = txt

            self.camera.start_recording(path, format='h264')
            self.recording  = True
            self.video_file = path
            self.cam_data.update({
                'recording': True,
                'status':    f"Recording → {fn}",
                'error':     ''
            })
        except Exception as e:
            self.cam_data['error'] = f"Record start failed: {e}"
        finally:
            self.socketio.emit('cam', self.cam_data)

    def stop_record(self):
        if not self.recording:
            return
        try:
            self.camera.stop_recording()
            self.recording = False
            self.camera.annotate_text = ''
            self.cam_data.update({
                'recording': False,
                'status':    "Recording stopped",
                'error':     ''
            })
        except Exception as e:
            self.cam_data['error'] = f"Record stop failed: {e}"
        finally:
            self.socketio.emit('cam', self.cam_data)

#    @classmethod
    def _capture_loop(self):
        # continuous JPEG preview
        stream = io.BytesIO()
        for _ in self.camera.capture_continuous(stream, format='jpeg', use_video_port=True):
            if self.exit_event.is_set():
                break
            stream.seek(0)
            self.frame = stream.read()
            stream.seek(0)
            stream.truncate()
    
    def emit( self ):
      self.socketio.emit( 'cam', self.cam_data )
      self.socketio.emit( 'strobe', self.strobe_data )
    
    def set_timing( self ):
      try:
        self.strobe_period_ns = min( self.strobe_period_ns, 16000000 )
        valid = self.strobe_cam.set_timing( 32, self.strobe_period_ns, 20000000 )
        self.optimize_fps_btn_enabled = True
        if valid:
          self.enabled = True
      except:
        pass
    
    def optimize_fps( self ):
      get_cam_read_time_us = 10000
      get_cam_read_time_us_prev = 0
      strobe_post_padding_ns = 1000000
      tries = 10
      while ( abs( get_cam_read_time_us - get_cam_read_time_us_prev ) > 1000 ):
        get_cam_read_time_us_prev = get_cam_read_time_us
        self.strobe_cam.set_timing( 32, self.strobe_period_ns, strobe_post_padding_ns )
        valid, get_cam_read_time_us = self.strobe_cam.strobe.get_cam_read_time();
        strobe_post_padding_ns = ( get_cam_read_time_us + 100 ) * 1000
        
        tries = tries - 1
        if tries <= 0:
          break
    
    def update( self ):
      valid, cam_read_time_us = self.strobe_cam.strobe.get_cam_read_time()
      if valid:
        self.cam_read_time_us = cam_read_time_us
      
      self.strobe_framerate = int( self.strobe_cam.camera.framerate )
    
    def update_strobe_data( self ):
      self.update()
      self.strobe_data['cam_read_time_us'] = self.cam_read_time_us
      self.strobe_data['period_ns'] = self.strobe_period_ns
      self.strobe_data['framerate'] = self.strobe_framerate
    
    def on_strobe( self, data ):
      if ( data['cmd'] == 'hold' ):
        hold_on = 1 if ( data['parameters']['on'] != 0 ) else 0
        valid = self.strobe_cam.strobe.set_hold( hold_on )
        self.strobe_data['hold'] = hold_on
      elif ( data['cmd'] == 'enable' ):
        enabled = data['parameters']['on'] != 0
        valid = self.strobe_cam.strobe.set_enable( enabled )
        self.strobe_data['enable'] = enabled
      elif ( data['cmd'] == 'timing' ):
        wait_ns = int(data['parameters']['wait_ns'])
        period_ns = int(data['parameters']['period_ns'])
        self.strobe_data['wait_ns'] = wait_ns  # <- Track for frontend display
        self.strobe_period_ns = period_ns
        self.set_timing()    
      
      self.update_strobe_data()
      self.socketio.emit( 'strobe', self.strobe_data )
    
    def on_cam( self, data ):
      cmd = data['cmd']
      if cmd == 'snapshot':
          self.save()
      elif cmd == 'optimize':
          self.optimize_fps()
      elif cmd == 'start_record':
          self.start_record()
      elif cmd == 'stop_record':
          self.stop_record()
      
      # always emit updated strobe + cam state
      self.update_strobe_data()
      self.socketio.emit( 'strobe', self.strobe_data )
