import threading
import os

buffer_size = 10000000
buffer1 = []
buffer2 = []
buffer1TOBeread = True
buffer2TOBeread = True
file = open("ifile.txt", "r")
eof = False;
allowPrint = False

def fillBuffer():
    global eof
    global buffer1
    global buffer2
    global buffer1TOBeread
    global buffer2TOBeread
    global buffer_size
    global allowPrint
    while(not eof):
        if(buffer1TOBeread):
            buffer1 = file.read(buffer_size)
            buffer1TOBeread = False
            if(len(buffer1) < buffer_size):
                eof = True
        if(buffer2TOBeread and not eof):
            buffer2 = file.read(buffer_size)
            buffer2TOBeread = False
            if(len(buffer2) < buffer_size):
                eof = True
        if(not allowPrint):
            allowPrint = True

def printOutBuffer():
    global allowPrint
    global buffer1TOBeread
    global buffer2TOBeread
    global eof
    
    while(True):
        if(allowPrint):
            if(not buffer1TOBeread):
                print("Printing Buffer1")
                buffer1TOBeread = True
            if(not buffer2TOBeread):
                print("Printing Buffer2")
                buffer2TOBeread = True
            if(buffer1TOBeread and buffer2TOBeread and eof):
                break
#Main Program
t1 = threading.Thread(target=fillBuffer)
t2 = threading.Thread(target=printOutBuffer)
t1.start()
t2.start()
t1.join()
t2.join()

file.close()
