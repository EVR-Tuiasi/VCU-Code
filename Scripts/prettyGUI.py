import serial
import threading
from PySide6.QtWidgets import (
    QApplication, QLabel, QWidget, QGridLayout, QVBoxLayout, QHBoxLayout, QFrame
)
from PySide6.QtGui import QColor, QPainter, QLinearGradient
from PySide6.QtCore import Qt, QTimer
import sys

# Serial Setup
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
    return QColor(r, g, 0)

class VoltageGradient(QWidget):
    def paintEvent(self, event):
        painter = QPainter(self)
        grad = QLinearGradient(0, 0, self.width(), 0)
        grad.setColorAt(0.0, voltage_to_color(2.5))
        grad.setColorAt(1.0, voltage_to_color(4.2))
        painter.fillRect(self.rect(), grad)

class LipoMonitor(QWidget):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("15S LiPo Monitor")
        self.setStyleSheet("background-color: #f6f6f6; font-family: Segoe UI;")
        self.voltages = [0.0] * 15
        self.init_ui()
        self.start_serial_thread()

    def init_ui(self):
        layout = QVBoxLayout()
        grid = QGridLayout()

        self.labels = []
        for i in range(15):
            label = QLabel(f"C{i+1}: --- V")
            label.setFixedHeight(30)
            label.setStyleSheet("padding: 6px; background: #eeeeee; font-size: 14px;")
            grid.addWidget(label, i // 3, i % 3)
            self.labels.append(label)

        self.total_label = QLabel("Total: --- V")
        self.total_label.setAlignment(Qt.AlignCenter)
        self.total_label.setStyleSheet("font-weight: bold; font-size: 16px; color: #0055cc; margin: 10px;")

        legend_title = QLabel("Voltage Color Map (2.5V → 4.2V)")
        legend_title.setAlignment(Qt.AlignCenter)

        self.gradient = VoltageGradient()
        self.gradient.setFixedHeight(20)

        legend = QVBoxLayout()
        legend.addWidget(legend_title)
        legend.addWidget(self.gradient)

        layout.addLayout(grid)
        layout.addWidget(self.total_label)
        layout.addLayout(legend)
        self.setLayout(layout)

    def update_gui(self):
        total = 0.0
        for i, voltage in enumerate(self.voltages):
            total += voltage
            color = voltage_to_color(voltage)
            self.labels[i].setText(f"C{i+1}: {voltage:.3f} V")
            self.labels[i].setStyleSheet(f"padding: 6px; background-color: {color.name()}; font-size: 14px;")
        self.total_label.setText(f"Total: {total:.3f} V")

    def serial_loop(self):
        while True:
            if ser.in_waiting >= 10:
                buffer = ser.read(10)
                if buffer[0] < 13:
                    continue
                base = (buffer[0] % 13) * 3
                v1 = parse_signed_24bit(buffer[1:4]) / 100000.0
                v2 = parse_signed_24bit(buffer[4:7]) / 100000.0
                v3 = parse_signed_24bit(buffer[7:10]) / 100000.0
                if base < 15:
                    self.voltages[base] = v1
                if base + 1 < 15:
                    self.voltages[base + 1] = v2
                if base + 2 < 15:
                    self.voltages[base + 2] = v3

                QTimer.singleShot(0, self.update_gui)

    def start_serial_thread(self):
        threading.Thread(target=self.serial_loop, daemon=True).start()

if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = LipoMonitor()
    window.resize(400, 500)
    window.show()
    app.exec()
    ser.close()
