import time
import io
import threading
import picamera
from PIL import Image

class Camera(object):
    thread = None  # background thread that reads frames from camera
    frame = None  # current frame is stored here by background thread
    last_access = 0  # time of last client access to the camera
    
    def __init__( self, exit_event ):
      self.exit_event = exit_event
    
    def initialize( self ):
        if self.thread is None:
            # start background frame thread
            self.thread = threading.Thread(target=self._thread)
#            self.thread.daemon = True
            self.thread.start()
            
            # wait until frames start to be available
            while self.frame is None:
#                time.sleep(0)
              pass

    def get_frame(self):
        self.last_access = time.time()
        self.initialize()
        return self.frame
    
    def save( self ):
      img=Image.open( io.BytesIO( self.frame ) )
      img.save( "snapshot.jpg", "JPEG" )
#      self.strobe_cam.camera.capture( "snapshot.jpg" )
    
    @classmethod
    def _thread(self):
        with picamera.PiCamera() as camera:
            # camera setup
            camera.resolution = (320, 240)
            camera.hflip = True
            camera.vflip = True

            # let camera warm up
#            camera.start_preview()
#            time.sleep(2)

            stream = io.BytesIO()
            for foo in camera.capture_continuous(stream, 'jpeg',
                                                 use_video_port=True):
                # store frame
                stream.seek(0)
                self.frame = stream.read()

                # reset stream for next frame
                stream.seek(0)
                stream.truncate()

                # if there hasn't been any clients asking for frames in
                # the last 10 seconds stop the thread
                if time.time() - self.last_access > 10:
                  break
                if self.exit_event.isSet():
                  print( "Pi Camera thread exiting" )
                  break
        self.thread = None
