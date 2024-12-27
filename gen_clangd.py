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
]

def main() -> None:
	cdir:str = os.getcwd();
	clangd_path:str = f"{cdir}/.clangd"
	output:str = "CompileFlags:\n\tCompiler: arm-none-eabi-gcc\n\tAdd:\n"
	for dir in DIRS:
		output += f'\t\t- "-I{cdir}/{dir}"\n'
	output += FLAGS
	output = output.replace("\t", " " * TAB_SIZE)

	with open(clangd_path, "w+") as clangd:
		clangd.write(output)
	print('file ".clangd" is created!')


if __name__ == "__main__":
	main()
