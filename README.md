# Morse Magician — A Morse Code Interpreter

Morse Magician is a project that utilizes the FRDM-KL46Z microcontroller board and a light sensor to translate Morse code messages transmitted through light pulses into readable text.

## Features

- Real-time decoding of Morse code messages transmitted through light pulses
- Utilizes the FRDM-KL46Z board's ADC for light signal capture
- UART communication between the board and the computer for displaying decoded messages
- Translation of Morse code sequences into corresponding characters

## Getting Started

### Prerequisites

- FRDM-KL46Z development board
- MCUXpresso IDE

### Setup

1. Clone the repository and open the project in MCUXpresso IDE.

2. Ensure that the board is selected when creating a new project.

3. In the project settings, navigate to the SDK Debug Console under Project Options and make sure UART is selected.

4. Connect the board to your computer, building and flashing the project to it.

## Usage

1. Open a serial terminal (e.g., PuTTY, Arduino IDE Serial Monitor) to view the decoded messages.

2. Transmit Morse code messages using a light source directed towards the board's light sensor.

3. Observe the decoded characters and the final decoded message on the serial terminal.