import os

TAB_SIZE:int = 2

FLAGS:str = """\
\t\t- "-mcpu=cortex-m3"
\t\t- "-DUSE_HAL_DRIVER"
\t\t- "-DSTM32F103xB"
\t\t- "-c"
\t\t- "-Os"
\t\t- "-ffunction-sections"
\t\t- "-fdata-sections"
\t\t- "-Wall"
\t\t- "--specs=nano.specs"
\t\t- "-mfloat-abi=soft"
\t\t- "-mthumb"
"""

DIRS:list[str] = [
	"src/boards/BLUE-PILL",
	"src/mcu/stm32f1xx/Core/Inc",
	"src/mcu/stm32f1xx/CMSIS/Headers",
	"src/mcu/stm32f1xx/CMSIS/Include",
	"src/mcu/stm32f1xx/HAL/Inc",
	"src/mcu/stm32f1xx/HAL/Inc/Legacy",
	"src/mcu/stm32f1xx/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc",
	"src/mcu/stm32f1xx/Middlewares/ST/STM32_USB_Device_Library/Core/Inc",
	"src/mcu/stm32f1xx/USB_DEVICE/App",
	"src/mcu/stm32f1xx/USB_DEVICE/Target",
	# Drivers
	"drivers/encoder",
	"drivers/neopixel",
	"drivers/ssd1306",
]

ADDITIONAL:list[str] = [
	"-isystem/usr/arm-none-eabi/include",
]

def main() -> None:
	cdir:str = os.getcwd();
	clangd_path:str = f"{cdir}/.clangd"
	output:str = "CompileFlags:\n\tCompiler: arm-none-eabi-gcc\n\tAdd:\n"
	for dir in DIRS:
		output += f'\t\t- "-I{cdir}/{dir}"\n'
	for parts in ADDITIONAL:
		output += f'\t\t- "{parts}"\n'
	output += FLAGS
	output = output.replace("\t", " " * TAB_SIZE)

	with open(clangd_path, "w+") as clangd:
		clangd.write(output)
	print('file ".clangd" is created!')


if __name__ == "__main__":
	main()
