#############################################################################################################################
# file:  CMakeLists.txt
#
# usage: Edit "VARIABLES"-section to suit project requirements.
#        For debug build:
#          cmake -DCMAKE_TOOLCHAIN_FILE=cubeide-gcc.cmake  -S ./ -B Debug -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug
#          make -C Debug VERBOSE=1 -j
#        For release build:
#          cmake -DCMAKE_TOOLCHAIN_FILE=cubeide-gcc.cmake  -S ./ -B Release -G"Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
#          make -C Release VERBOSE=1 -j
#############################################################################################################################
cmake_minimum_required(VERSION 3.20)


###################### VARIABLES ######################################

set(PROJECT_NAME    "CMake_contains")
set(OUTPUT_ELF      "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.elf")
set(MAP_FILE        "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.map")
set(OUTPUT_BIN      "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.bin")

set(PROJECT_TYPE    "exe")
set(LINKER_SCRIPT   "../STM32F103C8TX_FLASH.ld")
set(OUTPUT_JSN      "config.json")
set(OUTPUT_MONITOR  "monitor.py")

set(C_COMPILER "arm-none-eabi-gcc")
set(C_OBJCOPY  "arm-none-eabi-objcopy")

set(CMAKE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/cmake")


set(PY_INTERPRETER  "python")


# Flash Commands
set(FLASH_START_ADDRESS "0x8000000")
set(FLASHER "st-flash")
set(FLASHER_HARD_FLAGS
	"--reset"
	"--connect-under-reset"
	"write"
)

set(FLASHER_SOFT_FLAGS
	"--reset"
	"write"
)


unset(SUPPORTED_BOARDS)
unset(SUPPORTED_APPLICATIONS)
unset(SUPPORTED_MCUS)

unset(SELECTED_APP_SOURCES)
unset(SELECTED_APP_TARGET)

unset(BOARD)
unset(MCU)

unset(BASE_DIR)
unset(TARGET_DEF)
unset(MCU_BASE_DIR)

# Default Serial Ports
set(SERIAL_PORT "/dev/ttyUSB0")
set(SERIAL_SPEED "115200")


# Update target board if defined
if(NOT DEFINED BOARD)
	message(FATAL_ERROR "You have to specify a target")
endif()

# Add sub directory '/src'
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src")

file(GLOB BOARD_SOURCES  "${BASE_DIR}/*.c")

file(GLOB SOURCES_S "${MCU_BASE_DIR}/Core/Startup/*.s")
message(STATUS "MCU Directory: ${MCU_BASE_DIR}")


###################### Build Number ######################
execute_process(
	COMMAND git rev-parse HEAD
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
	RESULT_VARIABLE git_result
	OUTPUT_VARIABLE git_sha
	ERROR_VARIABLE git_error
	OUTPUT_STRIP_TRAILING_WHITESPACE
)

if(NOT git_result EQUAL 0)
	message(FATAL_ERROR "Faild to get build number version: ${git_error}")
endif()

set(GIT_SHA ${git_sha})
add_definitions(-DGIT_SHA="${git_sha}")
message(STATUS "Git SHA: ${GIT_SHA}")


###################### Build Info ######################
configure_file(
	${CMAKE_DIR}/template.json.in
	${CMAKE_BINARY_DIR}/${OUTPUT_JSN}
	@ONLY
)

# Optionally, print the path of the generated JSON file
message(STATUS "Generated config JSON at: ${CMAKE_BINARY_DIR}/${OUTPUT_JSN}")


configure_file(
	${CMAKE_DIR}/monitor.py.in
	${CMAKE_BINARY_DIR}/${OUTPUT_MONITOR}
	@ONLY
)

# Optionally, print the path of the generated JSON file
message(STATUS "Monitor script generated at: ${CMAKE_BINARY_DIR}/$OUTPUT_MONITOR}")


###################### Project Files ######################

file(GLOB HAL_SOURCES    "${MCU_BASE_DIR}/HAL/Src/*.c")
file(GLOB CORE_SOURCES   "${MCU_BASE_DIR}/Core/Src/*.c")
file(GLOB SYSTEM_SOURCES "${MCU_BASE_DIR}/System/*.c")


# List source files here
set(PROJECT_SOURCES
	${BOARD_SOURCES}           # Board Sources
	${HAL_SOURCES}             # HAL sources
	${CORE_SOURCES}            # Core sources
	${SOURCES_S}               # Startup sources
	${SYSTEM_SOURCES}          # System sources
	${SELECTED_APP_SOURCES}    # selected application sources
	"${CMAKE_CURRENT_SOURCE_DIR}/drivers/encoder/encoder.c"
)


# List source files here
set(PROJECT_INCLUDES
	"${MCU_BASE_DIR}/HAL/Inc"
	"${MCU_BASE_DIR}/Core/Inc"
	"${MCU_BASE_DIR}/CMSIS/Headers"
	"${MCU_BASE_DIR}/CMSIS/Include"
	"${BASE_DIR}"              # Board Includes
	"${CMAKE_CURRENT_SOURCE_DIR}/drivers/encoder"
)

foreach(DIR ${PROJECT_INCLUDES})
	list(APPEND INCLUDE_FLAGS "-I${DIR}")
endforeach()


################## PROJECT SETUP ######################################

set(PROJECT_NAME    "${SELECTED_APP_TARGET}")
set(OUTPUT_ELF      "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.elf")
set(MAP_FILE        "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.map")
set(OUTPUT_BIN      "${CMAKE_BINARY_DIR}/${PROJECT_NAME}.bin")

project(${PROJECT_NAME})
enable_language(C)
enable_language(ASM)


set(OBJECT_FLAGS
	"-mcpu=cortex-m3"
	"-std=gnu11"
	"-DUSE_HAL_DRIVER"
	"-D${TARGET_DEF}"
	"-c"
	"-Os"
	"-ffunction-sections"
	"-fdata-sections"
	"-Wall"
	# "-fstack-usage"
	# "-fcyclomatic-complexity"
	# "-MMD"
	# "-MP"
	"--specs=nano.specs"
	"-mfloat-abi=soft"
	"-mthumb"
)


set(LINKER_FLAGS
	"-mcpu=cortex-m3"
	"-T${LINKER_SCRIPT}"
	"--specs=nosys.specs"
	"-Wl,-Map=${MAP_FILE}"
	"-Wl,--gc-sections"
	"-static"
	"--specs=nano.specs"
	"-mfloat-abi=soft"
	"-mthumb"
	"-Wl,--start-group"
	"-lc"
	"-lm"
	"-Wl,--end-group"
)

unset(OBJECTS)

foreach(SRC ${PROJECT_SOURCES})
	get_filename_component(OBJ_NAME ${SRC} NAME_WE)   # Get object name
	set(OBJ ${CMAKE_BINARY_DIR}/${OBJ_NAME}.o)        # Change extention container to .o

	# Compile each given source to an object
	add_custom_command(
		OUTPUT ${OBJ}
		COMMAND ${C_COMPILER} ${OBJECT_FLAGS} ${SRC} ${INCLUDE_FLAGS} -o ${OBJ}
		DEPENDS ${SRC}
		COMMENT "Compiling ${SRC} to \"${OBJ_NAME}.o\""
		# COMMENT "Compiling ${SRC} to ${OBJ}"
	)

	# Save the object path into 'OBJECTS'
	list(APPEND OBJECTS ${OBJ})
endforeach()


# Link objects and create "elf" file
add_custom_command(
	OUTPUT ${OUTPUT_ELF}
	COMMAND ${C_COMPILER} -o ${OUTPUT_ELF} ${OBJECTS} ${LINKER_FLAGS}
	COMMENT "Linking ${OUTPUT_ELF} from object files"
	DEPENDS ${OBJECTS}  # Make sure objects are compiled
)

# Convert "elf" file to "binary"
add_custom_command(
	OUTPUT ${OUTPUT_BIN}
	COMMAND ${C_OBJCOPY} -Obinary ${OUTPUT_ELF} ${OUTPUT_BIN}
	COMMENT "Converting \"${OUTPUT_ELF}\" to binary format"
	DEPENDS ${OUTPUT_ELF}  # Make sure elf file is created
)

# Compile the program
add_custom_target(generate_binary ALL DEPENDS ${OUTPUT_BIN})
message(STATUS "Binary target: \"${OUTPUT_BIN}\"")


if(DEFINED Flash)
endif()


# Add target "flash" to Makefile
add_custom_target(flash
	COMMAND ${FLASHER} ${FLASHER_SOFT_FLAGS} ${OUTPUT_BIN} ${FLASH_START_ADDRESS}
	COMMENT "Flashing"
)

add_custom_target(hard
	COMMAND ${FLASHER} ${FLASHER_HARD_FLAGS} ${OUTPUT_BIN} ${FLASH_START_ADDRESS}
	COMMENT "Flashing"
)

add_custom_target(monitor
	COMMAND ${PY_INTERPRETER} ${OUTPUT_MONITOR}
	COMMENT "Monitor"
)
