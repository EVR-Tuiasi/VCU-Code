import serial
import threading
import tkinter as tk
from tkinter import Canvas
from datetime import datetime

# Serial setup
ser = serial.Serial('COM15', 9600, timeout=1)

def parse_signed_24bit(b):
    val = b[0] << 16 | b[1] << 8 | b[2]
    if val & 0x800000:
        val -= 0x1000000
    return val

def voltage_to_color(v):
    v = max(2.5, min(4.2, v))
    norm = (v - 2.5) / (4.2 - 2.5)
    r = int(255 * (1 - norm))
    g = int(255 * norm)
    b = 0
    return f'#{r:02x}{g:02x}{b:02x}'

root = tk.Tk()
root.title("15S LiPo Monitor")

labels = []

# Voltage labels
for i in range(16):
    lbl = tk.Label(root, text=f"C{i+1}: --- V", font=("Arial", 14), width=20, anchor="w", bg="#eeeeee")
    lbl.grid(row=i // 3, column=i % 3, padx=10, pady=5)
    labels.append(lbl)

# Total voltage
total_label = tk.Label(root, text="Total: --- V", font=("Arial", 16, "bold"), fg="blue")
total_label.grid(row=5, column=0, columnspan=3, pady=10)

# Color legend
legend_frame = tk.Frame(root)
legend_frame.grid(row=6, column=0, columnspan=3, pady=10)

legend_label = tk.Label(legend_frame, text="Voltage Color Map (2.5V → 4.2V)", font=("Arial", 12))
legend_label.pack()

legend_canvas = Canvas(legend_frame, width=200, height=20)
legend_canvas.pack()

for x in range(200):
    v = 2.5 + (x / 199) * (4.2 - 2.5)
    color = voltage_to_color(v)
    legend_canvas.create_line(x, 0, x, 20, fill=color)

legend_canvas.create_text(0, 25, anchor='nw', text="2.5V", font=("Arial", 10))
legend_canvas.create_text(180, 25, anchor='nw', text="4.2V", font=("Arial", 10))

# Timestamp log
log_label = tk.Label(root, text="Last 3 data timestamps:", font=("Arial", 12, "italic"))
log_label.grid(row=7, column=0, sticky="w", padx=10)

log_text = tk.StringVar()
log_display = tk.Label(root, textvariable=log_text, font=("Courier", 12), justify="left")
log_display.grid(row=8, column=0, columnspan=3, sticky="w", padx=10)

log_entries = []

def update_log():
    log_text.set('\n'.join(log_entries[-3:]))

# Update GUI
def update_gui(data):
    total = 0.0
    for i, voltage in enumerate(data):
        total += voltage
        color = voltage_to_color(voltage)
        labels[i].config(text=f"C{i+1}: {voltage:.3f} V", bg=color)
    total_label.config(text=f"Total: {total:.3f} V")

    timestamp = datetime.now().strftime("%H:%M:%S.%f")[:-3]  # Truncate microseconds to ms
    log_entries.append(timestamp)
    root.after(0, update_log)

# Serial thread
def serial_thread():
    voltages = [0.0] * 16
    while True:
        if ser.in_waiting >= 10:
            buffer = ser.read(10)
            if buffer[0] < 13:
                continue

            base = (buffer[0] % 13) * 3
            v1 = parse_signed_24bit(buffer[1:4]) / 100000.0
            v2 = parse_signed_24bit(buffer[4:7]) / 100000.0
            v3 = parse_signed_24bit(buffer[7:10]) / 100000.0

            if base < 16:
                voltages[base] = v1
            if base + 1 < 16:
                voltages[base + 1] = v2
            if base + 2 < 16:
                voltages[base + 2] = v3

            root.after(0, update_gui, voltages)

# Run
threading.Thread(target=serial_thread, daemon=True).start()
root.mainloop()
ser.close()