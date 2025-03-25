import serial
import time
import keyboard
import queue
import json
import msvcrt

def tøm_buffer():
    while msvcrt.kbhit():
        msvcrt.getch()

def Send_a_Message(): #Function for sending a message
    time.sleep(0.0001) #Waiting for 1 ms
    tøm_buffer()
    can_id = input("CanID:")
    message = input("The message you want to send:")
    Complete_message = can_id + message + "\n"
    ser.write(Complete_message.encode("utf-8"))     # write a message thru the serial port

def on_key_event(event): #Function for sending a message when "esc" is pressed
    if event.name == "esc":
        Send_a_Message() 
        print("Sendte a message over seriel port.")


#---------------Trykk på esc for å sende en melding--------------- 
Port = "COM5" #Port to use for serial communciations
Max_length = 10 #number for how many ascii signs max to read
Baudrate = 115200 #Baudrate for the serial communications
Timeout = 2 #how many seconds the port will wait for a message before closing

Easy_reception = True #If False is makes json, If True it just prints as text
Print_interval = 100 #Number to simulate a periodic sendRequest to server, number repressents the wait in ms between sending

Program_on = True
ser = serial.Serial(port=Port, baudrate=Baudrate, timeout=Timeout) #Opens the port for sending and reciving
print(ser.name)         # checks which port that was really used

keyboard.add_hotkey('b', Send_a_Message)
keyboard.on_press(on_key_event) #Checks if keys are pressed
Queue = queue.Queue()

try:
    in_waiting_before = 0 #Previus value
    Reminder_counter = 0 #Reminder
    Sending_counter = 0
    while Program_on: 
        if ser.in_waiting > 0:
            #Mode that turns data into Json and uses a queue
            if Easy_reception is False:
                raw_data = ser.readline() 
                raw_data_text = raw_data.decode("utf-8") #Turns to sting, ("Canid:n1:n2:n3:n4:n5:n6:n7:n8")
                list_str = raw_data_text.split(":") #Splits string on ":", ["Canid","n1","n2","n3","n4","n5","n6","n7","n8")]
                list = [int(x) for x in list_str] #Convert all number to int [Canid,n1,n2,n3,n4,n5,n6,n7,n8)]
                objekt = {"Canid": list[0], "Message": [list[1], list[2], list[3], list[4], list[5], list[6], list[7], list[8]]} #sorts
                Json_data = json.dumps(objekt)  #Turns into json: {"Canid": 200, "Message": [n1,n2,n3,n4,n5,n6,n7,n8]}
                Queue.put(Json_data) #Puts Json data into FIFO Queue

                #If statment to simulate periodic sending to server
                if Sending_counter >= Print_interval:
                    while not Queue.empty(): 
                        print(Queue.get()) #Emptyes the Queue until its empty
                    Sending_counter = 0
                Sending_counter +=1
            #Mode that uses basic binary to text only
            else:
                if in_waiting_before == ser.in_waiting: 
                    data = ser.read_all()
                    text = data.decode("utf-8")
                    print(text)
                in_waiting_before = ser.in_waiting

        #A reminder for button to stop the program
        if Reminder_counter > 3000: #Not important, just a reminder
            print("For å avslutte lyttingen trykk CTRL+C i kommando-vinduet")
            Reminder_counter = 0
        Reminder_counter += 1

        time.sleep(0.000001) #Waiting for 1 ns
except KeyboardInterrupt:
    print("Stoppkommando mottatt. Avslutter mottak") #For å avslutte lyttingen trykk CTRL+C i kommando-vinduet     


