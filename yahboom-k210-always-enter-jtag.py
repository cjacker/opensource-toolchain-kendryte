import sys
import serial
import time
import subprocess

if len(sys.argv) == 1:
    print("This script is only for Yahboom K210 Developer Kit")
    print("to demo how to activate jtag debugging mode by reset")
    print("even IO 0 - 3 were re-mapped.\n")
    print("Usage: python yahboom-k210-always-enter-jtag <serial device of uart>")
    exit() 

serial_device = sys.argv[1]

ctrl_port = serial.Serial(serial_device, baudrate=2000000)

# reset the device
ctrl_port.dtr = 1
ctrl_port.dtr = 0
ctrl_port.close()

# sleep a proper time
time.sleep(0.1)

# After reset and before IO re-mapped, IO 0 - 4 of K210 is JTAG pins.
# launch kendryte-openocd.
# no matter IO re-mapped or not, it could be able to always activate jtag debugging mode.
subprocess.run(["kendryte-openocd", "-f", "interface-ft2232d.cfg", "-f", "target-k210.cfg"])
