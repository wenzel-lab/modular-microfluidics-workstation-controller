import time
import spidev
import piholder

def spi_init( bus, device, mode, speed_hz ):
  spi = spidev.SpiDev()
  spi.open( bus, device )
  spi.mode = mode
  spi.max_speed_hz = speed_hz
  #spi.no_cs = False
  #spi.cshigh = False
  return spi

spi = spi_init( 0, 1, 2, 50000 )
holder = piholder.PiHolder( spi, 0.1 )

valid, id, id_valid = holder.get_id()
print( valid, id, id_valid )

#valid = holder.set_pressure( [1000, 200, 300, 4000] )
#print( valid )

valid, temp_c = holder.get_temp_actual()
print( valid, temp_c )

spi.close()
