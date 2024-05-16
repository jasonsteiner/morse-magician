# -*- coding: utf-8 -*-
"""
Python Example to read from FRDM-KL25Z Debug Cable Serial Port

You will need to change the COM Port. If you change the baudrate
be sure to change it in the setup code for the UART0 too.

In windows you can find it under "Device Manager" -> "Ports (COM & LTP)"

In linux it will look like /dev/ttyUSB0 or something similar. You can find it
by plugging in the board and then running 'sudo dmesg' and looking at most 
recent output for something like /dev/tty????

The board will send one string "Hello There Again!" 
And then keep sending single bytes 0-255

To send larger values than 255 you need to send muliple bytes from the MCU 
and then decode them appropriatly using struct.unpack()
"""


import serial  #from pyserial package 
import struct  #import to decode the byte strings returned form the serial read



with serial.Serial('COM15',115200) as ser:

    print(ser.readline()) #This should read to the first '\n' character
                          #If you don't see anything on the output 
                          #it could be because the board did not send a line
                          #try pressing the reset button to resend the string
    while True:
        #Read one character
        some_bytes = ser.read() 
        
        #Returns a list (of length 1) of decoded python types
        decoded_bytes=struct.unpack('B',some_bytes) 
        #NOTE: This code will throw an error if you send to fast because
        #some_bytes is too long and cant be decoded into a single 8-bit int
        
        print(f'{some_bytes} decodes to the number: {decoded_bytes[0]}' )