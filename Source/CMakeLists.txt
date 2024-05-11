# Executable
add_executable(AQC main.cpp)
  
# Source subdirectories
add_subdirectory(Acoplador)
add_subdirectory(Controlador)
add_subdirectory(Modelo)
add_subdirectory(Sensor)
add_subdirectory(Utils)

# Incluir Bibliotecas
target_link_libraries(AQC
  PRIVATE
    Acoplador
    Controlador
    Modelo
    Sensor
)

# Include directory
target_include_directories(AQC 
    PUBLIC 
        ${CMAKE_SOURCE_DIR}/Include/
)

# Include Files
target_sources(AQC
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/AQC.cpp
    PUBLIC
        ${CMAKE_SOURCE_DIR}/Include/AQC.h
)

# Set Build output
set_target_properties( 
    AQC
    PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Library"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Library"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Binary"
)

