import keyboard
import time
import os
import serial
arduino = serial.Serial(port='/dev/ttyUSB0', baudrate=115200, timeout=.1)
angle = 0
mode_dict = {"0000":"00", "1000":"FD", "1010":"FD", "1001":"FD", "0100":"BK", "0110":"BK", "0101":"BK", "1100":"S0", "1110":"SL", "1101":"SR"}

def send_data(m, a):
    command = "<R{}{}&>".format(m,a)
    print(command)
    arduino.write(bytes(command, 'utf-8'))
    # response = arduino.readline()
    time.sleep(0.1/5)
    # while response.decode('utf-8')!="end\r\n":
    #     response = arduino.readline()
    #     # print("waiting for the end to come", response)
    # print("end received")
    # while response.decode('utf-8')!="ok\r\n":
    # response = arduino.readline()
    # print(response.decode( 'utf-8'))

def command_to_data(command, angle):
    # print(command)
    if command[3]=="1":
        if angle<45:
            angle+=2
            # print("increment 1...\n")
    if command[2]=="1":
        if angle>-45:
            angle-=2
            # print("decrement 1...\n")
    # time.sleep(0.05/angle)
    if command in mode_dict:
        if mode_dict[command][0]=="S":
            angle=0
        return mode_dict[command], angle
    else:
        return "00", angle

while True:
    command_array = [0,0,0,0] # [forward, backward, left, right]
    if keyboard.is_pressed('0'):
        print("0")
        break
    else:
        if keyboard.is_pressed('w'):
            # print("w")
            command_array[0]=1
        if keyboard.is_pressed('s'):
            # print("s")
            command_array[1]=1
        if keyboard.is_pressed('a'):
            # print("a")
            command_array[2]=1
        if keyboard.is_pressed('d'):
            # print("d")
            command_array[3]=1

    # print(command_array)
    command_string = ''
    for i in command_array:
        command_string+=str(i)


    # f = open("/home/aiden/stuff/thisFile.txt", "r")
    # i = len(f.readlines())
    # f.close()
    # if i<5:
    #     f = open("/home/aiden/stuff/thisFile.txt", "a")
    #     # print(command_string)
    #     f.write(command_string+"\n")
    #     f.close()
    # else:
    #     # print("file buffer full")
    #     time.sleep(0.5)
    #     os.system('clear')

    mode, angle = command_to_data(command_string, angle)
    print("Sending Mode:{}, Angle:{}".format(mode,angle))
    send_data(mode, angle)

