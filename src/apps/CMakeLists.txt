# Apps

# Add the APPLICATION directory here if you wish to support it
list(APPEND SUPPORTED_APPLICATIONS
	"${CMAKE_CURRENT_SOURCE_DIR}/blink"
	"${CMAKE_CURRENT_SOURCE_DIR}/hello-world"
	"${CMAKE_CURRENT_SOURCE_DIR}/main"
	"${CMAKE_CURRENT_SOURCE_DIR}/test"
	"${CMAKE_CURRENT_SOURCE_DIR}/btn_interrupt"
	"${CMAKE_CURRENT_SOURCE_DIR}/timer"
)


# Set default application if not set
if(NOT DEFINED APP)
	list(GET SUPPORTED_APPLICATIONS 0 first_item)
	get_filename_component(item_name "${first_item}" NAME)
	set(APP "${item_name}")
	message(WARNING "set default application: \"${item_name}\","
		" because the application (-DAPP=\"<application>\") is not set")
endif()


# Make shure every added directory is exists
foreach(path ${SUPPORTED_APPLICATIONS})
	if(NOT EXISTS "${path}")
		message(FATAL_ERROR "Application path \"${path}\" not exists!")
	endif()
endforeach()


# Make sure the given application (-DAPP) name is valid
set(app_exists FALSE)
foreach(path ${SUPPORTED_APPLICATIONS})
	get_filename_component(dir_name "${path}" NAME)
	if(dir_name STREQUAL "${APP}" AND NOT app_exists)
		set(app_exists TRUE)
		set(APP_TARGET_DIR "${path}")
	endif()
endforeach()

if(NOT app_exists)
	message(FATAL_ERROR "Application \"${APP}\" not exists!")
else()
	message(STATUS "Selected Application: ${APP}")
endif()


# Add target application's path to the sources
file(GLOB APP_SOURCES "${APP_TARGET_DIR}/*.c")
# Set the variable in parent scope too
set(SELECTED_APP_SOURCES "${APP_SOURCES}" CACHE STRING "${APP_SOURCES}")
set(SELECTED_APP_TARGET "${APP}" CACHE STRING "${APP}")


foreach(C_SOURCE ${APP_SOURCES})
	message(STATUS "Application Sources: ${C_SOURCE}")
endforeach()

