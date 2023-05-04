import cv2, serial, time, glob

# Load the cascade
face_cascade = cv2.CascadeClassifier('haarcascade_frontalface_default.xml')

# To capture video from webcam. 
# cap = cv2.VideoCapture(0)
for i in glob.glob("/dev/video?"):
    cap = cv2.VideoCapture(i)
    if cap is None or not cap.isOpened():
      continue
    else:
      break

arduino = serial.Serial(port="/dev/ttyACM0", baudrate=9600, timeout=.1)
# To use a video file as input 
# cap = cv2.VideoCapture('filename.mp4')

width = cap.get(cv2.CAP_PROP_FRAME_WIDTH)
height = cap.get(cv2.CAP_PROP_FRAME_HEIGHT)

def map_range(x, in_min, in_max, out_min, out_max):
  return (x - in_min) * (out_max - out_min) // (in_max - in_min) + out_min

while True:
    # Read the frame
    _, img = cap.read()
    # Convert to grayscale
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    # Detect the faces
    faces = face_cascade.detectMultiScale(gray, 1.1, 4)
    # Draw the rectangle around each face
    if len(faces) > 0:
        (x, y, w, h) = faces[0]
        cv2.rectangle(img, (x, y), (x+w, y+h), (255, 0, 0), 2)
        center = ((x+x+w)/2, (y+y+h)/2)
        centerAngles = [map_range(center[0], 0, width, 0, 180), map_range(center[1], 0, height, 0, 180)];
        # print(height, width, center, [map_range(center[0], 0, width, 0, 180), map_range(center[1], 0, height, 0, 180)])
        arduino.write(bytes(f"{centerAngles[0]},{centerAngles[1]},90,60\r\n", 'utf-8'))
    else:
        arduino.write(bytes(f"0,0,0,120\r\n", 'utf-8'))

    time.sleep(0.1)
    print(arduino.readline())
    # Display
    # cv2.imshow('img', img)
    # Stop if escape key is pressed
    k = cv2.waitKey(30) & 0xff
    if k==27:
        break
# Release the VideoCapture object
cap.release()

# arduino = serial.Serial(port="/dev/ttyACM0", baudrate=9600, timeout=.1)
#
# while True:
#     arduino.write(bytes("60,30,40,80\r\n", 'utf-8'))
#     time.sleep(0.1)
#     print(arduino.readline())
