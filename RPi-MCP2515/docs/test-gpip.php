import RPi.GPIO as GPIO
import time

GPIO.setmode(GPIO.BCM)

# LED red
GPIO.setup(4, GPIO.OUT)
# LED green
GPIO.setup(27, GPIO.OUT)
# Push button
GPIO.setup(22, GPIO.IN)

for i in range(5):
    GPIO.output(4, GPIO.HIGH)
    GPIO.output(27, GPIO.LOW)
    time.sleep(0.5)
    GPIO.output(4, GPIO.LOW)
    GPIO.output(27, GPIO.HIGH)
    time.sleep(0.5)

# Endlosschleife
while True:
    if GPIO.input(22) == 0:
        # Einschalten
        GPIO.output(4, GPIO.LOW)
        GPIO.output(27, GPIO.LOW)
    else:
        # Ausschalten
        GPIO.output(4, GPIO.HIGH)
        GPIO.output(27, GPIO.HIGH)

