# Se7en-Segment-Display-DUOS
> This is an example project to showcase bare-metal embedded developement on the STM32F4xxx series MCU.

## Prerequisites

### Hardware
- [STM32-Neucleo Board](https://www.st.com/en/microcontrollers-microprocessors/stm32f446re.html) 
- [5161AS-LED-Display](https://www.jameco.com/z/LS-5161AS-Jameco-ValuePro-Red-7-Segment-LED-Display-640nm-0-56-H-Digit-Common-Cathode-Right-Hand-Decimal_2325447.html)
- Some Jumper Wires

### Toolchain
- Unix-like environment e.g. Linux or MacOS or Git-Bash/WSL on Windows
- [ARM GNU Toolchain](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads)
- [stlink-tools](https://github.com/stlink-org/stlink)
- [```find```](https://www.gnu.org/software/findutils/)
- [```make```](https://www.gnu.org/software/make/manual/make.html)

## Installation
- ### Ubuntu/Debian
  ```sh
  apt install gcc-arm-none-eabi stlink-tools make
  ```
 
- ### MacOS (Preferrably using [brew](https://brew.sh/))
  ```sh
  brew install stlink
  brew install --cask gcc-arm-embedded
  ```
- ### Windows
  - It is highly recommended that you use GIT Bash or WSL like environments so that GNU utils like ```make``` and ```find``` are better accesible
  - Download [mingw-w64-i686-arm-none-eabi](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads), [stlink-x86_64-w64-mingw32](https://github.com/stlink-org/stlink/releases), [make](https://gnuwin32.sourceforge.net/packages/make.htm)
  - If you do not use GIT Bash or WSL then you need to download [```find```](https://gnuwin32.sourceforge.net/packages/findutils.htm) seperately 
  - Add the downloaded executables to your ```$PATH``` as needed

- ### Verfication
- Make sure you can run the following commands on a terminal
  ```sh
  st-info --probe #[Run this with MCU connected]
  arm-none-eabi-gcc --version
  find --version
  make --version
  ```
- If any of these tools is not accessible or not working properly then fix it first.


## Build & Flash
- Check if ST-Link is able to communicate with MCU
  ```sh
  make probe
  ```
- Build ARM Binary
  ```sh
  make clean all
  ```
- Flash!
  ```sh
  make flash
  ```
***STlink may require superuser permission on MacOS/Linux
## Pin Logic
- Look at the LED-Display so that a small circle appears on your top left corner. 
- Let's label the pins from left to right 1->2->3->4->5
- Pin 3 from TOP or BOTTOM side should be connected to GND

## Serial Communication
You may use [Hercules](https://www.hw-group.com/software/hercules-setup-utility), [Serial Monitor](https://marketplace.visualstudio.com/items?itemName=ms-vscode.vscode-serial-monitor) on VS Code or any CLI/GUI tool to send and receive data over USART. 
