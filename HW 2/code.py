import board
import pwmio # get access to PWM
import time

# control GP14 with PWM
motor = pwmio.PWMOut(board.GP15, variable_frequency=True)
motor.frequency = 500 # in hz
motor.duty_cycle = 0 # initially off, at 16bit number so max on is 65535

while True:
    # start duty cycle at 0, every loop increase by 100 
    # until getting to the max of 65535
    for i in range(0, 65535, 100):
        motor.duty_cycle = i
        time.sleep(0.01)
    for i in range(0, 65535, 100):
        motor.duty_cycle = 65535 - i
        time.sleep(0.01)