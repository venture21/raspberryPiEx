# -*- coding: utf-8 -*-
import RPi.GPIO as GPIO
import time
import sys
import signal

#GPIO ��
TRIG = 23 # Ʈ����
ECHO = 24 # ����

#�Ÿ� Ÿ�� �ƿ� ��
MAX_DISTANCE_CM = 300
MAX_DURATION_TIMEOUT = (MAX_DISTANCE_CM * 2 * 29.1) #17460 # 17460us = 300cm

# Ű���� CTRL + C ������ ���� �ǰ� ó��
def signal_handler(signal, frame):
        print('You pressed Ctrl+C!')
        GPIO.cleanup()
        sys.exit(0)
signal.signal(signal.SIGINT, signal_handler)

# cm ȯ�� �Լ�
# �Ƶ��̳� UltraDistSensor �ڵ忡�� ������
def distanceInCm(duration):
    # ��ü�� ������ ���ƿ��� �ð� ���
    # �ð� = cm / ���� * �պ�
    # t   = 0.01/340 * 2= 0.000058824�� (58.824us)

    # �νı����� �ð�
    # t = 0.01/340 = 0.000029412�� (29.412us)

    # duration�� �պ� �ð��̴� �νı����� �ð����� 2�� ����
    return (duration/2)/29.1


# �Ÿ� ǥ��
def print_distance(distance):
    if distance == 0:
        distanceMsg = 'Distance : out of range                   \r'
    else:
        distanceMsg = 'Distance : ' + str(distance) + 'cm' + '        \r'
    sys.stdout.write(distanceMsg)
    sys.stdout.flush()


def main():
    # ���̽� GPIO ���
    GPIO.setmode(GPIO.BCM)

    # �� ����
    GPIO.setup(TRIG, GPIO.OUT) # Ʈ���� ���
    GPIO.setup(ECHO, GPIO.IN)  # ���� �Է�

    print('To Exit, Press the CTRL+C Keys')

    # HC-SR04 ���� �� ��� ���
    GPIO.output(TRIG, False)
    print('Waiting For Sensor To Ready')
    time.sleep(1) # 1��

    #����
    print('Start!!')
    while True:
        #171206 �߰��� ��� �ȵǴ� ���� ������      
        fail = False
        time.sleep(0.1)
        # Ʈ���Ÿ� 10us ���� High �ߴٰ� Low�� ��.
        # sleep 0.00001 = 10us
        GPIO.output(TRIG, True)
        time.sleep(0.00001)
        GPIO.output(TRIG, False)

        # ECHO�� ��ȣ�� ��� �ö����� ���
        timeout = time.time()
        while GPIO.input(ECHO) == 0:
            #�������� ���� �ð��� ������ ����
            pulse_start = time.time()
            if ((pulse_start - timeout)*1000000) >= MAX_DURATION_TIMEOUT:
                #171206 �߰��� ��� �ȵǴ� ���� ������        
                #continue
                fail = True
                break
                
        #171206 �߰��� ��� �ȵǴ� ���� ������        
        if fail:
            continue
        #ECHO�� �ν� ���� �������� ���
        timeout = time.time()
        while GPIO.input(ECHO) == 1:
            #���� �ð� ������ ����
            pulse_end = time.time()
            if ((pulse_end - pulse_start)*1000000) >= MAX_DURATION_TIMEOUT:
                print_distance(0) 
                #171206 �߰��� ��� �ȵǴ� ���� ������        
                #continue
                fail = True
                break

        #171206 �߰��� ��� �ȵǴ� ���� ������        
        if fail:
            continue

        #�ν� ���ۺ��� ��������� ���� �ٷ� �Ÿ� �ν� �ð�
        pulse_duration = (pulse_end - pulse_start) * 1000000

        # �ð��� cm�� ȯ��
        distance = distanceInCm(pulse_duration)
        #print(pulse_duration)
        #print('')
        # �ڸ��� �ݿø�
        distance = round(distance, 2)

        #ǥ��
        print_distance(distance)

    GPIO.cleanup()



if __name__ == '__main__':
    main()