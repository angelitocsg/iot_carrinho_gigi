import tkinter as tk
from tkinter import ttk, messagebox
import serial

import serial.tools.list_ports

class SerialUI(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title("Serial Command Sender")
        self.geometry("500x600")
        self.serial_port = None

        # Serial port selection
        self.port_var = tk.StringVar()
        ports = [
            f"{port.device} - {port.description} ({port.manufacturer or 'Unknown'})"
            for port in serial.tools.list_ports.comports()
        ]
        self.port_map = {p: port.device for p, port in zip(ports, serial.tools.list_ports.comports())}
        ttk.Label(self, text="Serial Port:").pack(pady=5)
        self.port_combo = ttk.Combobox(
            self, textvariable=self.port_var, values=ports, state="readonly", width=60
        )
        self.port_combo.pack(pady=5)

        # Baudrate selection
        self.baud_var = tk.StringVar(value="9600")
        ttk.Label(self, text="Baudrate:").pack(pady=5)
        self.baud_combo = ttk.Combobox(self, textvariable=self.baud_var, values=["9600", "115200", "57600"], state="readonly")
        self.baud_combo.pack(pady=5)

        # Connect button
        self.connect_btn = ttk.Button(self, text="Connect", command=self.connect_serial)
        self.connect_btn.pack(pady=5)

        # Command entry
        ttk.Label(self, text="Command:").pack(pady=5)
        self.cmd_entry = ttk.Entry(self)
        self.cmd_entry.pack(pady=5, fill="x", padx=10)

        # Grupo de botões para comandos rápidos
        btn_frame = ttk.Frame(self)
        btn_frame.pack(pady=5)
        quick_cmds = [
            ("START", "o"),
            ("STOP", "s"),
            ("^ TURBO", "t"),
            ("< LEFT", "l"),
            ("> RIGHT", "r"),
            ("^ FORWARD", "f"),
            ("^ BACK", "b"),
        ]
        for label, char in quick_cmds:
            btn = ttk.Button(btn_frame, text=label, width=8, command=lambda c=char: self.send_quick_command(c))
            btn.pack(side="left", padx=2)

        # Send button
        self.send_btn = ttk.Button(self, text="Send", command=self.send_command)
        self.send_btn.pack(pady=10)

        # Returned data
        ttk.Label(self, text="Serial Output:").pack(pady=5)
        self.output_text = tk.Text(self, height=5, wrap="word", state="disabled")
        self.output_text.pack(pady=5, fill="both", padx=10, expand=True)

        self.protocol("WM_DELETE_WINDOW", self.on_close)

    def connect_serial(self):
        if self.serial_port and self.serial_port.is_open:
            self.serial_port.close()
        try:
            port_name = self.port_map.get(self.port_var.get(), self.port_var.get())
            self.serial_port = serial.Serial(port_name, int(self.baud_var.get()), timeout=1)
            messagebox.showinfo("Info", "Connected to serial port.")
            self.after(100, self.read_serial)  # Inicia leitura periódica
        except Exception as e:
            messagebox.showerror("Error", f"Failed to connect: {e}")

    def read_serial(self):
        if self.serial_port and self.serial_port.is_open:
            try:
                if self.serial_port.in_waiting:
                    data = self.serial_port.readline().decode(errors="ignore")
                    self.output_text.config(state="normal")
                    self.output_text.insert("end", data)
                    self.output_text.see("end")
                    self.output_text.config(state="disabled")
            except Exception:
                pass
            self.after(100, self.read_serial)

    def send_command(self):
        if not self.serial_port or not self.serial_port.is_open:
            messagebox.showwarning("Warning", "Serial port not connected.")
            return
        cmd = self.cmd_entry.get()
        if cmd:
            self.serial_port.write((cmd + "\n").encode())

    def send_quick_command(self, cmd):
        if not self.serial_port or not self.serial_port.is_open:
            messagebox.showwarning("Warning", "Serial port not connected.")
            return
        self.serial_port.write((cmd + "\n").encode())

    def on_close(self):
        if self.serial_port and self.serial_port.is_open:
            self.serial_port.close()
        self.destroy()

if __name__ == "__main__":
    app = SerialUI()
    app.mainloop()