# Arduino Morse Code Transmitter

A professional Morse code transmission system featuring Arduino hardware control and a Python GUI interface. Convert text to International Morse Code with visual (LED) and audio (buzzer) output, complete with configurable timing parameters and real-time transmission control.
<div align="center">
  <img src="https://img.shields.io/badge/version-2.0-blue" alt="Software Version">
  <img src="https://img.shields.io/badge/arduino-compatible-green" alt="Arduino Compatibility">
  <img src="https://img.shields.io/badge/python-3.7+-yellow" alt="Python Version">
</div>

## Features

- **Full Character Support**: Letters (A-Z), digits (0-9), and common punctuation
- **Configurable Timing**: Adjust dot duration, dash duration, and all pause intervals
- **Real-time Control**: Stop transmissions mid-sequence
- **Transmission History**: View the last transmitted Morse code sequence
- **User-friendly GUI**: Intuitive Python interface for easy operation
- **Standard Compliant**: Follows International Morse Code timing standards

## Requirements

### Hardware
- Arduino board (known to work/tested on Arduino Uno)
- LED (built-in LED on pin 13 works perfectly)
- Piezo buzzer
- Breadboard and jumper wires (optional)

### Software
- Arduino IDE (1.8.x or later)
- Python 3.7 or higher
- Required Python packages:
  ```bash
  pip install pyserial
  ```
  
**Note**: `tkinter` comes pre-installed with most Python distributions

## Hardware Setup

Connect your components as follows:

| Component | Arduino Pin | Notes |
|-----------|-------------|-------|
| LED | Pin 13 | Can use built-in LED |
| Piezo Buzzer | Pin 12 | Positive to pin, negative to GND |

**Wiring Diagram:**
```
Arduino Pin 13 ──→ LED (+) ──→ Resistor (220Ω) ──→ GND
Arduino Pin 12 ──→ Buzzer (+) ──→ GND
```

## Quick Start

### Method 1: GUI Application (Recommended)

1. **Clone the repository**
   ```bash
   git clone https://github.com/yourusername/morse-code-transmitter.git
   cd morse-code-transmitter
   ```

2. **Upload Arduino sketch**
   - Open `morse_transmitter.ino` in Arduino IDE
   - Select your board and port from Tools menu
   - Click Upload

3. **Run the Python GUI**
   ```bash
   python morse_transmitter_gui.py
   ```

4. **Connect and transmit**
   - Select your COM port from the dropdown
   - Click "Connect"
   - Enter your message and click "Transmit"

### Method 2: Serial Terminal

1. Upload the Arduino sketch as described above
2. Open Arduino IDE Serial Monitor (Ctrl+Shift+M)
3. Set baud rate to 9600
4. Type your message and press Enter

## 📟 Serial Commands

Control the transmitter directly through serial communication:

| Command | Description | Example |
|---------|-------------|---------|
| `[text]` | Transmit text as Morse code | `HELLO WORLD` |
| `TIMINGS` | Display current timing settings | `TIMINGS` |
| `RESET` | Restore default timing values | `RESET` |
| `STOP` | Halt current transmission | `STOP` |
| `LAST` | Show last transmitted sequence | `LAST` |
| `SET DOT [ms]` | Set dot duration | `SET DOT 150` |
| `SET DASH [ms]` | Set dash duration | `SET DASH 450` |
| `SET SYMBOL [ms]` | Set inter-symbol pause | `SET SYMBOL 150` |
| `SET LETTER [ms]` | Set inter-letter pause | `SET LETTER 450` |
| `SET WORD [ms]` | Set inter-word pause | `SET WORD 1050` |

### Timing Configuration Examples

**Speed up transmission (faster Morse):**
```
SET DOT 100
```
This automatically adjusts all related timings to maintain proper ratios.

**Custom dash duration:**
```
SET DASH 500
```

**Reset everything to defaults:**
```
RESET
```

## Default Timing Values

Following International Morse Code standards:

- **Dot duration**: 200 ms
- **Dash duration**: 600 ms (3× dot)
- **Symbol pause**: 200 ms (between dots/dashes)
- **Letter pause**: 600 ms (between letters)
- **Word pause**: 1400 ms (between words)

## GUI Application Features

The Python GUI provides:

- **COM Port Manager**: Auto-detect and connect to Arduino
- **Message Input**: Type and transmit text easily
- **Control Panel**: Stop, reset, and query status
- **Output Log**: View all responses and transmission history
- **Status Indicator**: Visual connection feedback

## Building Executable

Create a standalone `.exe` file (Windows):

```bash
pip install pyinstaller
pyinstaller --onefile --noconsole --name="MorseTransmitter" morse_transmitter_gui.py
```

The executable will be in the `dist/` folder.

**Security Note**: Unsigned executables may trigger antivirus warnings. This is normal for self-compiled applications. You can either:
- Add an exception in their antivirus software
- Run the Python script directly instead
- Code-sign the executable (requires certificate)

## Troubleshooting

### Arduino not detected
- Ensure USB cable is properly connected
- Install appropriate drivers for your Arduino board
- Check Device Manager (Windows) or `ls /dev/tty*` (Linux/Mac)

### COM port access denied
- Close Arduino IDE Serial Monitor
- Close any other applications using the port
- Restart the GUI application

### Transmission not working
- Verify hardware connections
- Check that Arduino code uploaded successfully
- Test with Serial Monitor first to isolate GUI issues

### Python GUI won't start
- Verify Python version: `python --version`
- Reinstall pyserial: `pip install --upgrade pyserial`
- Check tkinter: `python -m tkinter`

## Morse Code Reference

### Letters
```
A .-    B -...  C -.-.  D -..   E .     F ..-.
G --.   H ....  I ..    J .---  K -.-   L .-..
M --    N -.    O ---   P .--.  Q --.-  R .-.
S ...   T -     U ..-   V ...-  W .--   X -..-
Y -.--  Z --..
```

### Numbers
```
0 -----  1 .----  2 ..---  3 ...--  4 ....-
5 .....  6 -....  7 --...  8 ---..  9 ----.
```

### Punctuation
```
. .-.-.-  , --..--  ? ..--..  ' .----.  ! -.-.--
/ -..-.   ( -.--.   ) -.--.-  & .-...  : ---...
; -.-.-.  = -...-   + .-.-.   - -....-  _ ..--.-
" .-..-.  $ ...-..- @ .--.-.
```

## Contributing

Contributions are welcome! Please feel free to submit pull requests or open issues for bugs and feature requests.

## Support

- **Issues**: [GitHub Issues](https://github.com/NOTz00m/bad-morse-transmitter/issues)
- **Documentation**: See inline code comments
---

**Tested on**: Arduino Uno, Arduino Nano, Arduino Mega 2560  
**Python Version**: 3.7, 3.8, 3.9, 3.10, 3.11  
**OS Compatibility**: Windows 10/11, macOS, Linux
