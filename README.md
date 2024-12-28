# Zenith: Abstraction Layer for Bluepill HAL

Zenith is an abstraction layer designed to simplify and streamline development on the Bluepill board by building on top of the Hardware Abstraction Layer (HAL). This project provides a cleaner, more user-friendly interface while maintaining the low-level flexibility needed for embedded development.

By using Zenith, developers can focus on application-level logic without getting bogged down in repetitive and complex HAL configurations.

## Purpose
The purpose of Zenith is to:

- Provide an intuitive, lightweight interface for interacting with Bluepill hardware.
- Abstract the complexities of HAL while retaining performance and flexibility.
- Standardize common hardware interactions, reducing redundancy in application code.

## Features
- Ease of Use: Simplified APIs for peripherals like GPIO, PWM, and timers.
- Modular Design: Organized into reusable modules for different peripherals and features.
- Extensibility: Add support for new peripherals or extend functionality with minimal effort.
- Optimized Performance: Minimal overhead, preserving the efficiency of HAL.


## Project Structure
The project is organized as follows:
```plaintext
Zenith
├── cmake
│   └── template.json.in
├── CMakeLists.txt
├── config
├── sensors
├── shared
├── src
│   ├── apps                     # Test applications
│   ├── boards                   # supported boards
│   │   ├── BLUE-PILL
│   └── mcu                      # Microcontroller Unit
│       └── stm32f1xx
│           ├── CMSIS
│           │   ├── Headers
│           │   └── Include
│           ├── Core             # Core abstraction logic
│           │   ├── Inc
│           │   ├── Src
│           │   └── Startup
│           ├── HAL               # HAL
│           │   ├── Inc
│           │   │   └── Legacy
│           │   └── Src
│           └── system
└── system
```

## Key Modules
- **GPIO**: High-level functions for pin configuration and usage.
- **Timers**: Abstractions for configuring and using hardware timers.


## Getting Started
### Prerequisites
1. [Bluepill board (STM32F103C8T6)](https://stm32-base.org/boards/STM32F103C8T6-Blue-Pill.html).
2. [ARM GCC toolchain](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads).
3. [CMake](https://cmake.org/)

# Installation
Clone the repository:
```bash
git clone https://github.com/empitrix/zenith
cd ./zenith
```

Now, by using [CMake](https://cmake.org/) and the [ARM GCC toolchain](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads), you can compile this project to generate `.elf` and `.bin` files.
```bash
cmake -DAPP="blink" -DBOARD="BLUE-PILL" -DCMAKE_TOOLCHAIN_FILE=cubeide-gcc.cmake  -S ./ -B Release -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
make -C Release
```

In this example, we set the board target to `"BLUE-PILL"` and the application target to `"blink"`, which compiles the applications located in `/src/apps/blink`. By using `make`, we run the generated `Makefile` located in the Release directory to compile the `.bin` file for us.



## Usage
Here’s an example of using Zenith to toggle an LED:

```c
#include "board.h"
#include "gpio.h"

#define DELAY 1000

int main(void) {
	boardInit();  // Initialize board

	// Set C13 as output GPIO
	GPIN led;
	gpinInit(&led, GPIN_13, GPIO_TYPE_C, GPIO_OUTPUT_MODE, GPIN_NO_PULL);

	while (1) {
		gpinToggle(&led);  // Toggle the LED
		delayMs(DELAY);    // Wait for 1 second
	}
}
```

For more examples, check the `src/apps/` directory.

