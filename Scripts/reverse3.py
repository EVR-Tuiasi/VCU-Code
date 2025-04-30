import serial
import sys

print("This is the name of the program:", sys.argv[0])

if len(sys.argv) > 1:
    lungime = int(sys.argv[1])
    print("Argument List:", str(sys.argv))
    print(sys.argv[1])
else:
    lungime = 1

ser = serial.Serial('COM15', 9600, timeout=1)
print("Listening on COM22...")
stiva=[]

def parse_signed_24bit(b):
    val = b[0] << 16 | b[1] << 8 | b[2]
    if val & 0x800000:
        val -= 0x1000000  # convert to signed
    return val

try:
    
    while True:
        if ser.in_waiting >= 7:
            buffer = ser.read(7)
            if buffer[0] != 12:
                print(f"Warning: First byte is {buffer[0]}, expected 12. Dropping packet.")
                continue

            if(len(stiva)==lungime):
                stiva.pop(0)
                
            v1 = parse_signed_24bit(buffer[1:4])
            v1=v1/1000.0

            i1 = parse_signed_24bit(buffer[4:7])
            i1=i1/1000.0
            stiva.append(i1)
            i1 = sum(stiva) / len(stiva)

            print(f"Voltage (v1): {v1:.3f}, Current (i1): {i1:.3f}")
except KeyboardInterrupt:
    print("Stopped by user.")
finally:
    ser.close()
