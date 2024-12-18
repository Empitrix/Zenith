#############################################################################################################################
# file:  CMakeLists.txt
# brief: Template "CMakeLists.txt" for building of executables and static libraries.
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

###################### CONSTANTS ######################################
set(PROJECT_TYPE_EXECUTABLE          "exe")
set(PROJECT_TYPE_STATIC_LIBRARY      "static-lib")
set(MCPU_CORTEX_M0                   "-mcpu=cortex-m0")
set(MCPU_CORTEX_M0PLUS               "-mcpu=cortex-m0plus")
set(MCPU_CORTEX_M3                   "-mcpu=cortex-m3")
set(MCPU_CORTEX_M4                   "-mcpu=cortex-m4")
set(MCPU_CORTEX_M7                   "-mcpu=cortex-m7")
set(MCPU_CORTEX_M33                  "-mcpu=cortex-m33")
set(MCPU_CORTEX_M55                  "-mcpu=cortex-m55")
set(MCPU_CORTEX_M85                  "-mcpu=cortex-m85")
set(MFPU_FPV4_SP_D16                 "-mfpu=fpv4-sp-d16")
set(MFPU_FPV5_D16                    "-mfpu=fpv5-d16")
set(RUNTIME_LIBRARY_REDUCED_C        "--specs=nano.specs")
set(RUNTIME_LIBRARY_STD_C            "")
set(RUNTIME_LIBRARY_SYSCALLS_MINIMAL "--specs=nosys.specs")
set(RUNTIME_LIBRARY_SYSCALLS_NONE    "")
set(MFLOAT_ABI_SOFTWARE              "-mfloat-abi=soft")
set(MFLOAT_ABI_HARDWARE              "-mfloat-abi=hard")
set(MFLOAT_ABI_MIX                   "-mfloat-abi=softfp")
#######################################################################

###################### VARIABLES ######################################
set(PROJECT_NAME             "CMake_contains")
set(PROJECT_TYPE             "exe")
set(LINKER_SCRIPT            "../STM32F103C8TX_FLASH.ld")
set(MCPU                     "-mcpu=Cortex-M3")
set(MFLOAT_ABI               "")
set(RUNTIME_LIBRARY          "--specs=nano.specs")
set(RUNTIME_LIBRARY_SYSCALLS "--specs=nosys.specs")

set(CMAKE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

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



# Update target board if defined
if(NOT DEFINED BOARD)
	message(FATAL_ERROR "You have to specify a target")
endif()

# Add sub directory '/src'
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src")


file(GLOB BOARD_INCLUDES "${BASE_DIR}/*.h")
file(GLOB BOARD_SOURCES "${BASE_DIR}/*.c")


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
  ${CMAKE_BINARY_DIR}/config.json
  @ONLY
)

# Optionally, print the path of the generated JSON file
message(STATUS "Generated config JSON at: ${CMAKE_BINARY_DIR}/config.json")


###################### Project Files ######################



file(GLOB HAL_SOURCES "${MCU_BASE_DIR}/HAL/Src/*.c")
file(GLOB CORE_SOURCES "${MCU_BASE_DIR}/Core/Src/*.c")
file(GLOB SYSTEM_SOURCES "${MCU_BASE_DIR}/System/*.c")


# LIST SOURCE FILES HERE
set(PROJECT_SOURCES
	${SOURCES_S}
	${BOARD_SOURCES}  # Board Sources
	${HAL_SOURCES}
	${CORE_SOURCES}

	${SELECTED_APP_SOURCES}  # Set in /src
	${SYSTEM_SOURCES}
)

# file(GLOB PROJECT_SOURCES
# 	"${MCU_BASE_DIR}/HAL/Src/*.c"
# 	"${MCU_BASE_DIR}/Core/Src/*.c"
# )


set(PROJECT_DEFINES
)

set(PROJECT_INCLUDES
	"${MCU_BASE_DIR}/HAL/Inc"
	"${MCU_BASE_DIR}/Core/Inc"
	"${MCU_BASE_DIR}/CMSIS/Headers"
	"${MCU_BASE_DIR}/CMSIS/Include"
	${BOARD_INCLUDES}  # Board Includes
)



# message(FATAL_ERROR "${MCU_BASE_DIR}/System/*.c")

############ MODIFY ACCORDING TO REQUIREMENTS) ########################

#######################################################################

################## PROJECT SETUP ######################################
project(${PROJECT_NAME})
enable_language(ASM)


if (${PROJECT_TYPE} MATCHES ${PROJECT_TYPE_EXECUTABLE})
  add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
  add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${CMAKE_PROJECT_NAME}>)
elseif (${PROJECT_TYPE} MATCHES ${PROJECT_TYPE_STATIC_LIBRARY})
  add_library(${PROJECT_NAME} ${PROJECT_SOURCES})
endif()


# Add target board macro
add_definitions("-D${TARGET_DEF}")

add_compile_definitions (${PROJECT_DEFINES})
include_directories (${PROJECT_INCLUDES})

set(CMAKE_EXECUTABLE_SUFFIX ".elf")
set(CMAKE_STATIC_LIBRARY_SUFFIX ".a")
set(CMAKE_C_FLAGS "${MCPU} -std=gnu11 ${MFPU} ${MFLOAT_ABI} ${RUNTIME_LIBRARY} -mthumb -Wall -Werror")
set(CMAKE_EXE_LINKER_FLAGS "-T${LINKER_SCRIPT} ${RUNTIME_LIBRARY_SYSCALLS} -Wl,-Map=test.map -Wl,--gc-sections -static -Wl,--start-group -lc -lm -Wl,--end-group")
set(CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS} -x assembler-with-cpp")

