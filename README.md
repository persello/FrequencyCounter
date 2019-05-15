# FrequencyCounter
Arduino/AVR high frequency counter, goes up to half the clock speed of the microcontroller. It uses Timer/Counter1 in order to achieve high speeds.

## Usage
Simply upload the program using PlatformIO or Arduino (sketch file is located in the src folder) and connect to the serial monitor.
Connect your signal line to the digital pin 5 (on Arduino), called PORTD5 (T1 input) on AVR microcontrollers. Using an external clock is recommended.
Units are automatically calculated.
