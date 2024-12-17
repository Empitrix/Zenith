# src
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/apps")

if(DEFINED MCU)
	add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/mcu")
else()
	add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/boards")
endif()
