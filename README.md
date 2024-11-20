# Arduino Morse Code Transmitter

A versatile Morse code transmitter using an Arduino. This project converts text input into Morse code, blinking an LED and playing tones with a buzzer. It supports real-time configuration, stop functionality, and logging the last transmission.

## Features

- Converts text to Morse code using an LED and piezo buzzer.
- Adjustable timings for dots, dashes, and pauses.
- Serial commands to configure or reset timings.
- Ability to stop ongoing transmissions.
- Logs and prints the last transmitted Morse code.

## How to Use

### Option 1 (With Executable – Not recommended as it is quite buggy currently and TRIGGERS FALSE POSITIVES DUE TO NO CERTIFICATE):
1. Download the latest executable from the [releases page](#) + clone or download the repository.
2. Upload the Arduino sketch to your Arduino board using the Arduino IDE.
3. Run `morsetransmission.exe`. This will automatically start the application.
4. Select your COM port (usually something like `COM3`, `COM4` on Windows).
5. Enter your desired text and hit "Transmit" to send Morse code.
6. To configure timings or reset, click the respective buttons in the interface.

### Option 2 (Using Source Code):
1. Clone or download the repository.
2. Connect the components as described below:
   - **LED** to pin 13 (default on most Arduino boards).
   - **Piezo buzzer** to pin 12 (optional).
3. Upload the Arduino sketch to your Arduino board using the Arduino IDE.
4. Open the Serial Monitor and start interacting with the project using commands:
   - You will be prompted to input text to transmit into morse code automaticlaly.
   - `TIMINGS`: View current timings.
   - `RESET`: Reset all configurations to defaults.
   - `LAST`: View the last transmitted Morse code.

## Serial Commands

- `SET DOT <value>`: Set the dot duration in milliseconds.
- `SET DASH <value>`: Set the dash duration in milliseconds.
- `SET SYMBOL <value>`: Set the pause between symbols.
- `SET LETTER <value>`: Set the pause between letters.
- `SET WORD <value>`: Set the pause between words.
- `TIMINGS`: View current timings for morse transmission.
- `RESET`: Reset all configurations to defaults.
- `LAST`: Print the last Morse code transmission.

## Setup Instructions for Option 1 (with Executable):

### Prerequisites:
Before running the executable or compiling the Python code, make sure you have the following dependencies installed:

### Required Dependencies:

1. **Python 3.x** – Make sure you have Python installed (download from [python.org](https://www.python.org/)).
2. **Required Libraries**:
   - **pySerial**: Used for serial communication with the Arduino.
   - **Tkinter**: GUI toolkit for building the interface (comes pre-installed with Python in most distributions).
   - **pyInstaller (optional)**: Converts Python scripts into standalone executables.

```bash
pip install pyserial pyinstaller
```

## Creating the Executable (if you're building it yourself):

1. Clone or download the repository.
2. Navigate to the folder containing `main.py`.
3. Run the following command to generate the executable:

   ```bash
   pyinstaller --onefile --noconsole main.py
   ```

This command will create a standalone executable file in the `dist` folder.

## Troubleshooting:

- If you encounter issues with **COM port selection**, ensure that your Arduino is properly connected to your computer and recognized by the operating system.
- Make sure you are not running multiple applications that may block the serial port at the same time (like the Arduino IDE or Serial Monitor).
- THIS WAS TESTED ON AN ARDUINO UNO, I AM NOT RESPONSIBLE IF THE CODE DOES NOT WORK ON YOUR ARDUINO
