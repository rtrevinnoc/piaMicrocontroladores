import cv2, serial, time, glob

arduino = serial.Serial(port="/dev/ttyACM0", baudrate=9600, timeout=.1)


# door1: 0-90
# door2: 120-60

while True:
    arduino.write(bytes("60,30,0,120\r\n", 'utf-8'))
    time.sleep(0.1)
    print(arduino.readline())
