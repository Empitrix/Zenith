# MCU

# Add the MCU directory here if you wish to support it
list(APPEND SUPPORTED_MCUS
	"${CMAKE_CURRENT_SOURCE_DIR}/stm32f1xx"
)


unset(MCU_DEF)

# Make shure every added directory is exists
foreach(path ${SUPPORTED_MCUS})
	if(NOT EXISTS "${path}")
		message(FATAL_ERROR "MCU path \"${path}\" not exists!")
	endif()
endforeach()



set(mcu_exists FALSE)
foreach(path ${SUPPORTED_MCUS})
	get_filename_component(dir_name "${path}" NAME)
	# 

	# # Extact directory for STM32<XX>xx
	string(FIND "${MCU}" "STM32" POSITION)
	if(POSITION EQUAL 0)
		string(SUBSTRING "${MCU}" 0 7 FIRST_SEVEN_CHARS)
		set(TMP_MCU "${FIRST_SEVEN_CHARS}xx")
		string(TOLOWER "${TMP_MCU}" CURRENT_MCU_NAME)
	endif()
	if(dir_name STREQUAL "${CURRENT_MCU_NAME}" AND NOT mcu_exists)
		set(mcu_exists TRUE)
		set(MCU_TARGET "${path}")
	endif()

endforeach()

if(NOT mcu_exists)
	message(FATAL_ERROR "MCU \"${MCU}\" not exists!")
else()
	message(STATUS "Selected MCU: ${MCU}")
endif()

if(DEFINED MCU_TARGET)
	message(STATUS "Target MCU: ${MCU_TARGET}")
endif()

get_filename_component(MCU_NAME "${MCU_TARGET}" NAME)



set(MCU_DEF "${MCU}")


set(BASE_DIR "${MCU_TARGET}" CACHE STRING "${MCU_TARGET}")
set(TARGET_DEF "${MCU_DEF}" CACHE STRING "${MCU_DEF}")

