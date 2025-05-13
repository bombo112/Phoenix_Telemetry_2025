import serial
import time

# Set your serial port and baud rate
SERIAL_PORT = '/dev/tty.usbmodem1101'  # Replace with your actual port
BAUD_RATE = 115200
OUTPUT_FILE = 'serial_log.txt'

def main():
    try:
        with serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1) as ser, open(OUTPUT_FILE, 'a') as f:
            print(f"Logging from {SERIAL_PORT} to {OUTPUT_FILE}...")
            while True:
                if ser.in_waiting:
                    line = ser.readline().decode(errors='replace').strip()
                    timestamp = time.strftime("%Y-%m-%d %H:%M:%S")
                    log_line = f"[{timestamp}] {line}"
                    print(log_line)
                    f.write(log_line + '\n')
                    f.flush()
    except serial.SerialException as e:
        print(f"Error: {e}")
    except KeyboardInterrupt:
        print("Logging stopped.")

if __name__ == '__main__':
    main()
