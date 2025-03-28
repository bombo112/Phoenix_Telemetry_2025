import serial
import time
import queue
import json

#--------------------------------PC-test-relevant-imports------------------------
import keyboard
import msvcrt

def tøm_buffer():
    while msvcrt.kbhit():
        msvcrt.getch()


def Send_a_Message(): #Function for sending a message
    time.sleep(0.0001) #Waiting for 1 ms
    tøm_buffer()
    can_id = input("CanID:")
    message = input("The message you want to send(Launch_Rocket)(Terminate Flight):")
    complete_message = commandToCan_Convertion(can_id + ":" + message)
    ser.write(complete_message.encode("utf-8"))     # write a message thru the serial 
    

def commandToCan_Convertion(message):
    can_id, command = message.split(":")
    mapping = {
        "Launch_Rocket": [8, 7, 6, 5, 4, 3, 2, 1],
        "Terminate Flight": [255, 0, 255, 0, 255, 0, 255, 0]
    }
    bytes_list = mapping.get(command)
    if bytes_list is None:
        raise ValueError(f"Ukjent kommando: {command}")
    return f"{can_id}:{':'.join(map(str, bytes_list))}\n"


def canTocommand_convertion(text):
    bitListe = list()    

    can_id, content = text.split(":") #Splits string
    intCan_id = int(can_id) 
    intContent = int(content)

    result = {"id": intCan_id}

    #------------Check-type-----------------------
    if intCan_id == 200: message_type = "status"
    else: message_type = "float"
    
    #------------Result-based-on-type-------------
    #1x 8/64 Byte/bit ---- float value
    if message_type == 'float': 
        result["value"] = intContent

    #1x 8/64 Byte/bit ---- status bits 
    elif message_type == 'status': 
        for i in range(63):
            bitListe.append((intContent >> (63 - i)) & 1)
        result["value"] = bitListe  
    else:
        raise ValueError("Ukjent message_type. Bruk 'float' eller 'status'.")
    return result


def on_key_event(event): #Function for sending a message when "esc" is pressed
    if event.name == "esc":
        Send_a_Message() 
        print("Sendte a message over seriel port.")


#---------------Trykk på esc for å sende en melding--------------------------
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
    Reminder_counter = 0 #counters used in reminding ther person to use "Ctrl+C" to exit
    Sending_counter = 0 #counter used to send data from FIFO register

    while Program_on: 
        if ser.in_waiting > 0: 

            #Mode that categorises data based on can_id
            if Easy_reception is False:
                raw_data = ser.readline() 
                raw_data_text = raw_data.decode("utf-8") #Turns to sting, ("Canid:n1n2n3n4n5n6n7n8")
                Data_to_send = canTocommand_convertion(raw_data_text) #function that gives the right current message
                Queue.put(Data_to_send) #Puts data into FIFO Queue

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

#-------------------Only for pc testing----------------------------------------
        #A reminder for button to stop the program
        if Reminder_counter > 3000: #Not important, just a reminder
            print("For å avslutte lyttingen trykk CTRL+C i kommando-vinduet")
            Reminder_counter = 0
        Reminder_counter += 1
#------------------------------------------------------------------------------

        time.sleep(0.000001) #Waiting for 1 ns
except KeyboardInterrupt:
    print("Stoppkommando mottatt. Avslutter mottak") #For å avslutte lyttingen trykk CTRL+C i kommando-vinduet     


