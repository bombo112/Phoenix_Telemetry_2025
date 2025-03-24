import serial
import time
import keyboard

def Send_a_Message():
    time.sleep(0.0001) #Waiting
    can_id = input("CanID:")
    message = input("The message you want to send:")
    Complete_message = can_id + message
    ser.write(Complete_message.encode("utf-8"))     # write a message thru the serial port

def on_key_event(event): #Gets called when "ESC" is pressed
    if event.name == "esc":
        Send_a_Message() 
        print("Sendte a message over seriel port.")


#---------------Trykk på esc for å sende en melding--------------- 
Port = "COM5" 
Max_length = 10 #number for how many ascii signs max to read
Baudrate = 115200 
Timeout = 2 #how many seconds the port will wait for a message before closing

Program_on = True
ser = serial.Serial(port=Port, baudrate=Baudrate, timeout=Timeout) #Opens the port for sending and reciving
print(ser.name)         # checks which port that was really used

keyboard.add_hotkey('b', Send_a_Message)
keyboard.on_press(on_key_event) #Checks if keys are pressed

try:
    in_waiting_before = 0 #Previus value
    counter = 0 #Reminder
    while Program_on: 
        if ser.in_waiting > 0:
            if in_waiting_before == ser.in_waiting:
                data = ser.read_all()
                text = data.decode("utf-8")
                print(text)
        in_waiting_before = ser.in_waiting

        if counter > 3000: #Not important, just a reminder
            print("For å avslutte lyttingen trykk CTRL+C i kommando-vinduet")
            counter = 0
        counter += 1

        time.sleep(0.000001) #Waiting
except KeyboardInterrupt:
    print("Stoppkommando mottatt. Avslutter mottak") #For å avslutte lyttingen trykk CTRL+C i kommando-vinduet     


