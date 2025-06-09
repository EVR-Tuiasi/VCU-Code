import serial

ser = serial.Serial('COM15', 9600, timeout=1)
print("Listening on COM22...")

def parse_signed_24bit(b):
    val = b[0] << 16 | b[1] << 8 | b[2]
    if val & 0x800000:
        val -= 0x1000000  # convert to signed
    return val

try:
    while True:
        if ser.in_waiting >= 10:
            buffer = ser.read(10)
            if buffer[0] < 13 :
                print(f"Warning: First byte is {buffer[0]}, expected 13. Dropping packet.")
                continue

            v1 = parse_signed_24bit(buffer[1:4])
            v1=v1/100000.0
            v2 = parse_signed_24bit(buffer[4:7])
            v2=v2/100000.0
            v3 = parse_signed_24bit(buffer[7:10])
            v3=v3/100000.0
            
            #if buffer[0] == 18:
            #    print(f"C {buffer[0]%13+1}: {v1:.3f}")
            
            print(f"C {buffer[0]%13*3+1}: {v1:.3f}, C {buffer[0]%13*3+2}: {v2:.3f}, C {buffer[0]%13*3+3}: {v2:.3f}")
except KeyboardInterrupt:
    print("Stopped by user.")
finally:
    ser.close()
