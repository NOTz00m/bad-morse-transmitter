import tkinter as tk
from tkinter import messagebox
import serial
import time
from serial.tools import list_ports

# Initialize serial connection variables
arduino = None
BAUD_RATE = 9600

def refresh_com_ports():
    """Refresh the list of available COM ports."""
    ports = [port.device for port in list_ports.comports()]
    com_port_menu["menu"].delete(0, "end")
    for port in ports:
        com_port_menu["menu"].add_command(label=port, command=lambda p=port: set_com_port(p))
    if ports:
        com_port_var.set(ports[0])  # Set default selection to the first port
    else:
        com_port_var.set("No COM Ports Found")

def set_com_port(port):
    """Set the selected COM port."""
    global arduino
    com_port_var.set(port)
    if arduino and arduino.is_open:
        arduino.close()
    try:
        arduino = serial.Serial(port, BAUD_RATE, timeout=1)
        time.sleep(2)  # Allow time for Arduino to reset
        messagebox.showinfo("Info", f"Connected to {port}")
    except Exception as e:
        messagebox.showerror("Error", f"Failed to connect to {port}: {e}")
        arduino = None

def send_command(command):
    """Send a command to the Arduino via the serial port."""
    if arduino and arduino.is_open:
        try:
            arduino.write((command + '\n').encode())
            time.sleep(0.1)  # Small delay to ensure Arduino processes the command
            response = arduino.readline().decode().strip()
            return response
        except Exception as e:
            return f"Error sending command: {e}"
    else:
        return "Arduino not connected."

def transmit_text():
    text = text_entry.get().strip()
    if not text:
        messagebox.showerror("Error", "Please enter text to transmit!")
        return
    response = send_command(text)
    result_label.config(text=f"Response: {response}")

def reset_to_default():
    response = send_command("RESET")
    if response == "Arduino not connected.":
        messagebox.showerror("Error", "Arduino not connected.")
    else:
        messagebox.showinfo("Info", "Reset to default settings successfully.")
        result_label.config(text=f"Response: {response}")

def stop_transmission():
    response = send_command("STOP")
    if response == "Arduino not connected.":
        messagebox.showerror("Error", "Arduino not connected.")
    else:
        messagebox.showinfo("Info", "Transmission stopped.")
        result_label.config(text=f"Response: {response}")


def view_last_transmission():
    response = send_command("LAST")
    if response == "Arduino not connected.":
        messagebox.showerror("Error", "Arduino not connected.")
    else:
        # Ensure only relevant Morse code data is displayed
        response = response.split("\n")[-1]  # Take the last line of the response
        result_label.config(text=f"Last Transmission: {response}")

# GUI setup
window = tk.Tk()
window.title("Morse Code Transmitter")
window.geometry("400x350")

# COM port dropdown
tk.Label(window, text="Select COM Port:").pack(pady=5)
com_port_var = tk.StringVar()
com_port_menu = tk.OptionMenu(window, com_port_var, [])
com_port_menu.pack(pady=5)
tk.Button(window, text="Refresh Ports", command=refresh_com_ports).pack(pady=5)

tk.Label(window, text="Enter Text:").pack(pady=5)
text_entry = tk.Entry(window, width=30)
text_entry.pack(pady=5)

# Buttons
tk.Button(window, text="Transmit", command=transmit_text).pack(pady=5)
tk.Button(window, text="Reset to Default", command=reset_to_default).pack(pady=5)
tk.Button(window, text="Stop Transmission", command=stop_transmission).pack(pady=5)
tk.Button(window, text="Last Transmission", command=view_last_transmission).pack(pady=5)

result_label = tk.Label(window, text="Response will appear here.", wraplength=300)
result_label.pack(pady=10)

def on_closing():
    if arduino:
        arduino.close()
    window.destroy()

window.protocol("WM_DELETE_WINDOW", on_closing)

refresh_com_ports()

window.mainloop()
