blue:
	@ cmake -DBOARD="BLUE-PILL" -DCMAKE_TOOLCHAIN_FILE=cubeide-gcc.cmake  -S ./ -B Release -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
	@ make -C Release

btn:
	@ cmake -DAPP="btn_interrupt" -DBOARD="BLUE-PILL" -DCMAKE_TOOLCHAIN_FILE=cubeide-gcc.cmake  -S ./ -B Release -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
	@ make -C Release

neo:
	@ cmake -DAPP="neopixel" -DBOARD="BLUE-PILL" -DCMAKE_TOOLCHAIN_FILE=cubeide-gcc.cmake  -S ./ -B Release -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
	@ make -C Release

rot:
	@ cmake -DAPP="rotary-encoder" -DBOARD="BLUE-PILL" -DCMAKE_TOOLCHAIN_FILE=cubeide-gcc.cmake  -S ./ -B Release -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
	@ make -C Release

timer:
	@ cmake -DAPP="timer" -DBOARD="BLUE-PILL" -DCMAKE_TOOLCHAIN_FILE=cubeide-gcc.cmake  -S ./ -B Release -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
	@ make -C Release

hello:
	@ cmake -DSERIAL_PORT="/dev/ttyACM0" -DAPP="hello-world" -DBOARD="BLUE-PILL" -DCMAKE_TOOLCHAIN_FILE=cubeide-gcc.cmake  -S ./ -B Release -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
	@ make -C Release

echo:
	@ cmake -DSERIAL_PORT="/dev/ttyACM0" -DAPP="echo" -DBOARD="BLUE-PILL" -DCMAKE_TOOLCHAIN_FILE=cubeide-gcc.cmake  -S ./ -B Release -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
	@ make -C Release

test:
	@ cmake -DAPP="test" -DBOARD="BLUE-PILL" -DCMAKE_TOOLCHAIN_FILE=cubeide-gcc.cmake  -S ./ -B Release -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
	@ make -C Release

verbose:
	@ cmake -DBOARD="BLUE-PILL" -DCMAKE_TOOLCHAIN_FILE=cubeide-gcc.cmake  -S ./ -B Release -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
	@ make -C Release VERBOSE=1 -j

stm32f1:
	@ cmake -DBoard="STM32F103x8" -DCMAKE_TOOLCHAIN_FILE=cubeide-gcc.cmake  -S ./ -B Release -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
	@ make -C Release VERBOSE=1 -j

arduino:
	@ cmake -DBoard="Arduino" -DCMAKE_TOOLCHAIN_FILE=cubeide-gcc.cmake  -S ./ -B Release -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
	@ make -C Release VERBOSE=1 -j
