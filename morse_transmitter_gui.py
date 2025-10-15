"""
Morse Code Transmitter GUI
A graphical interface for controlling the Arduino-based Morse code transmitter.

Features:
- Dynamic COM port detection and selection
- Text transmission to Morse code
- Timing configuration controls
- Transmission history viewer
- Real-time Arduino communication

Requirements:
- pyserial
- tkinter (included with Python)
"""

import tkinter as tk
from tkinter import messagebox, scrolledtext
import serial
import serial.tools.list_ports as list_ports
import time
from typing import Optional

class MorseTransmitterGUI:
    """Main application class for the Morse Code Transmitter interface."""
    
    BAUD_RATE = 9600
    CONNECTION_TIMEOUT = 2
    COMMAND_DELAY = 0.1
    
    def __init__(self, root):
        self.root = root
        self.arduino: Optional[serial.Serial] = None
        self.setup_window()
        self.create_widgets()
        self.refresh_ports()
        
    def setup_window(self):
        """Configure the main application window."""
        self.root.title("Morse Code Transmitter")
        self.root.geometry("500x600")
        self.root.resizable(False, False)
        self.root.protocol("WM_DELETE_WINDOW", self.on_close)
        
    def create_widgets(self):
        """Create and layout all GUI components."""
        # Header
        header = tk.Label(
            self.root,
            text="Morse Code Transmitter",
            font=("Arial", 16, "bold"),
            pady=10
        )
        header.pack()
        
        # Connection frame
        self.create_connection_frame()
        
        # Separator
        tk.Frame(self.root, height=2, bd=1, relief=tk.SUNKEN).pack(fill=tk.X, padx=5, pady=10)
        
        # Input frame
        self.create_input_frame()
        
        # Control buttons frame
        self.create_control_frame()
        
        # Output frame
        self.create_output_frame()
        
    def create_connection_frame(self):
        """Create COM port selection controls."""
        frame = tk.LabelFrame(self.root, text="Connection", padx=10, pady=10)
        frame.pack(padx=10, pady=5, fill=tk.X)
        
        tk.Label(frame, text="COM Port:").grid(row=0, column=0, sticky=tk.W, pady=5)
        
        self.port_var = tk.StringVar()
        self.port_menu = tk.OptionMenu(frame, self.port_var, "No Ports")
        self.port_menu.config(width=20)
        self.port_menu.grid(row=0, column=1, padx=5, pady=5)
        
        refresh_btn = tk.Button(
            frame,
            text="Refresh",
            command=self.refresh_ports,
            width=10
        )
        refresh_btn.grid(row=0, column=2, padx=5, pady=5)
        
        connect_btn = tk.Button(
            frame,
            text="Connect",
            command=self.connect_to_port,
            width=10,
            bg="#4CAF50",
            fg="white"
        )
        connect_btn.grid(row=0, column=3, padx=5, pady=5)
        
        self.status_label = tk.Label(frame, text="Not connected", fg="red")
        self.status_label.grid(row=1, column=0, columnspan=4, pady=5)
        
    def create_input_frame(self):
        """Create text input controls."""
        frame = tk.LabelFrame(self.root, text="Message", padx=10, pady=10)
        frame.pack(padx=10, pady=5, fill=tk.BOTH, expand=True)
        
        tk.Label(frame, text="Enter text to transmit:").pack(anchor=tk.W)
        
        self.text_entry = tk.Entry(frame, width=50, font=("Arial", 11))
        self.text_entry.pack(pady=5, fill=tk.X)
        self.text_entry.bind("<Return>", lambda e: self.transmit_text())
        
        transmit_btn = tk.Button(
            frame,
            text="Transmit",
            command=self.transmit_text,
            width=15,
            height=2,
            bg="#2196F3",
            fg="white",
            font=("Arial", 10, "bold")
        )
        transmit_btn.pack(pady=5)
        
    def create_control_frame(self):
        """Create control button grid."""
        frame = tk.LabelFrame(self.root, text="Controls", padx=10, pady=10)
        frame.pack(padx=10, pady=5, fill=tk.X)
        
        buttons = [
            ("Stop Transmission", self.stop_transmission, "#F44336"),
            ("Reset Settings", self.reset_settings, "#FF9800"),
            ("View Timings", self.view_timings, "#9C27B0"),
            ("Last Transmission", self.view_last, "#607D8B")
        ]
        
        for i, (text, command, color) in enumerate(buttons):
            btn = tk.Button(
                frame,
                text=text,
                command=command,
                width=18,
                bg=color,
                fg="white"
            )
            row = i // 2
            col = i % 2
            btn.grid(row=row, column=col, padx=5, pady=5)
            
    def create_output_frame(self):
        """Create output display area."""
        frame = tk.LabelFrame(self.root, text="Output", padx=10, pady=10)
        frame.pack(padx=10, pady=5, fill=tk.BOTH, expand=True)
        
        self.output_text = scrolledtext.ScrolledText(
            frame,
            height=8,
            wrap=tk.WORD,
            font=("Courier", 9),
            state=tk.DISABLED
        )
        self.output_text.pack(fill=tk.BOTH, expand=True)
        
        clear_btn = tk.Button(
            frame,
            text="Clear Output",
            command=self.clear_output
        )
        clear_btn.pack(pady=5)
        
    def refresh_ports(self):
        """Scan and update available COM ports."""
        ports = [port.device for port in list_ports.comports()]
        
        menu = self.port_menu["menu"]
        menu.delete(0, "end")
        
        if ports:
            for port in ports:
                menu.add_command(
                    label=port,
                    command=lambda p=port: self.port_var.set(p)
                )
            self.port_var.set(ports[0])
            self.log_output(f"Found {len(ports)} COM port(s)")
        else:
            self.port_var.set("No Ports Found")
            self.log_output("No COM ports detected")
            
    def connect_to_port(self):
        """Establish serial connection to selected port."""
        selected_port = self.port_var.get()
        
        if selected_port == "No Ports Found":
            messagebox.showwarning("Warning", "No COM port selected")
            return
            
        if self.arduino and self.arduino.is_open:
            self.arduino.close()
            
        try:
            self.arduino = serial.Serial(
                selected_port,
                self.BAUD_RATE,
                timeout=1
            )
            time.sleep(self.CONNECTION_TIMEOUT)
            
            # Clear initial Arduino messages
            while self.arduino.in_waiting:
                self.arduino.readline()
                
            self.status_label.config(text=f"Connected to {selected_port}", fg="green")
            self.log_output(f"Successfully connected to {selected_port}")
            messagebox.showinfo("Success", f"Connected to {selected_port}")
            
        except Exception as e:
            self.status_label.config(text="Connection failed", fg="red")
            self.log_output(f"Connection error: {str(e)}")
            messagebox.showerror("Error", f"Failed to connect:\n{str(e)}")
            self.arduino = None
            
    def send_command(self, command: str) -> str:
        """Send a command to Arduino and return response."""
        if not self.arduino or not self.arduino.is_open:
            return "ERROR: Not connected to Arduino"
            
        try:
            self.arduino.write((command + '\n').encode('utf-8'))
            time.sleep(self.COMMAND_DELAY)
            
            response_lines = []
            timeout = time.time() + 1
            
            while time.time() < timeout:
                if self.arduino.in_waiting:
                    line = self.arduino.readline().decode('utf-8', errors='ignore').strip()
                    if line:
                        response_lines.append(line)
                else:
                    time.sleep(0.05)
                    
            return '\n'.join(response_lines) if response_lines else "No response"
            
        except Exception as e:
            return f"ERROR: {str(e)}"
            
    def transmit_text(self):
        """Send text to Arduino for Morse transmission."""
        text = self.text_entry.get().strip()
        
        if not text:
            messagebox.showwarning("Warning", "Please enter text to transmit")
            return
            
        self.log_output(f"Sending: {text}")
        response = self.send_command(text)
        self.log_output(response)
        
    def stop_transmission(self):
        """Send stop command to Arduino."""
        self.log_output("Stopping transmission...")
        response = self.send_command("STOP")
        self.log_output(response)
        
    def reset_settings(self):
        """Reset Arduino timing to defaults."""
        self.log_output("Resetting to default settings...")
        response = self.send_command("RESET")
        self.log_output(response)
        
    def view_timings(self):
        """Request and display current timing settings."""
        self.log_output("Retrieving timing settings...")
        response = self.send_command("TIMINGS")
        self.log_output(response)
        
    def view_last(self):
        """Display last transmitted Morse sequence."""
        self.log_output("Retrieving last transmission...")
        response = self.send_command("LAST")
        self.log_output(response)
        
    def log_output(self, message: str):
        """Append message to output display."""
        self.output_text.config(state=tk.NORMAL)
        self.output_text.insert(tk.END, message + "\n")
        self.output_text.see(tk.END)
        self.output_text.config(state=tk.DISABLED)
        
    def clear_output(self):
        """Clear the output display."""
        self.output_text.config(state=tk.NORMAL)
        self.output_text.delete(1.0, tk.END)
        self.output_text.config(state=tk.DISABLED)
        
    def on_close(self):
        """Clean up resources and close application."""
        if self.arduino and self.arduino.is_open:
            self.arduino.close()
        self.root.destroy()


def main():
    """Application entry point."""
    root = tk.Tk()
    app = MorseTransmitterGUI(root)
    root.mainloop()


if __name__ == "__main__":
    main()