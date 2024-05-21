import sys
import cv2

# Use recv.py on separate PC

port = 7123
host_ip = "192.168.100.11"

cap = cv2.VideoCapture(f'http://{host_ip}:{port}/stream.mjpg')

while True:
    _, frame = cap.read()
    cv2.imshow('Video', frame)

    if cv2.waitKey(1) == 27: # Press ESC --> close window
        sys.exit(0)

