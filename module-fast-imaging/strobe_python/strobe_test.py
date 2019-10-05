import time
import spidev

# We only have SPI bus 0 available to us on the Pi
bus = 0

#Device is the chip select pin. Set to 0 or 1, depending on the connections
device = 0

# Enable SPI
spi = spidev.SpiDev()

# Open a connection to a specific bus and device (chip select pin)
spi.open(bus, device)

# Set SPI speed and mode
spi.max_speed_hz = 500000
spi.mode = 2

msg = [7, 7, 7]
spi.xfer2(msg)

time.sleep(1)

#msg = [7, 7, 7, 2, 5, 3, 1, 245]
msg = [2, 7, 7, 7, 7, 2, 5, 3, 1, 245]
spi.xfer2(msg)

time.sleep(1)

msg = [2, 5, 3, 0, 246]
spi.xfer2(msg)

