# Boards

# Add the board directory here if you wish to support it
list(APPEND SUPPORTED_BOARDS
	"${CMAKE_CURRENT_SOURCE_DIR}/BLUE-PILL"
)


unset(BOARD_DEF)

# Make shure every added directory is exists
foreach(path ${SUPPORTED_BOARDS})
	if(NOT EXISTS "${path}")
		message(FATAL_ERROR "board path \"${path}\" not exists!")
	endif()
endforeach()



set(board_exists FALSE)
foreach(path ${SUPPORTED_BOARDS})
	get_filename_component(dir_name "${path}" NAME)
	if(dir_name STREQUAL "${BOARD}" AND NOT board_exists)
		set(board_exists TRUE)
		set(BOARD_TARGET "${path}")
	endif()
endforeach()

if(NOT board_exists)
	message(FATAL_ERROR "Board \"${BOARD}\" not exists!")
else()
	message(STATUS "Selected Board: ${BOARD}")
endif()

if(DEFINED BOARD_TARGET)
	message(STATUS "Target Board: ${BOARD_TARGET}")
endif()

get_filename_component(BOARD_NAME "${BOARD_TARGET}" NAME)



# Add board definatinos here
if(BOARD_NAME STREQUAL "BLUE-PILL")
	set(BOARD_DEF "STM32F103x8")
else()
	message(FATAL_ERROR "Processor for \"${BOARD_NAME}\" is not defined")
endif()


set(BASE_DIR "${BOARD_TARGET}" CACHE STRING "${BOARD_TARGET}")
set(TARGET_DEF "${BOARD_DEF}" CACHE STRING "${BOARD_DEF}")

