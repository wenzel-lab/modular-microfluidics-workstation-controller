import time
import spidev
import piflow

def spi_init( bus, device, mode, speed_hz ):
  spi = spidev.SpiDev()
  spi.open( bus, device )
  spi.mode = mode
  spi.max_speed_hz = speed_hz
  #spi.no_cs = False
  #spi.cshigh = False
  return spi

spi = spi_init( 0, 1, 2, 50000 )
flow = piflow.PiFlow( spi, 0.1 )

valid, id, id_valid = flow.get_id()
print( valid, id, id_valid )

valid = flow.set_pressure( [1000, 200, 300, 4000] )
print( valid )

valid, pressures = flow.get_pressure_actual()
print( valid, pressures )

spi.close()

#spi.readbytes(n)
