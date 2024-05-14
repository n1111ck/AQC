#
# Executable Target
#
add_executable(AQC-Main main.cpp)
  
# Source subdirectories
add_subdirectory(Acoplador)
add_subdirectory(Algoritmo)
add_subdirectory(Controlador)
add_subdirectory(Modelo)
add_subdirectory(Sensor)
add_subdirectory(Utils)

# Incluir Bibliotecas
link_directories(
    Acoplador/
    Algoritmo/
    Controlador/
    Modelo/
    Sensor/
    Utils/
)

# Incluir Bibliotecas
target_link_libraries(AQC-Main
  PUBLIC
    Acoplador
    Algoritmo
    Controlador
    Modelo
    Sensor
    Utils
)

#
# Library Target
#
add_library(AQC 
    
    Algoritmo/Algoritmo.cpp
    Acoplador/Acoplador.cpp
    Acoplador/GerenciadorAcopladores.cpp
    Controlador/RNL.cpp
    Modelo/Modelo.cpp
    Sensor/Barometro.cpp
    Sensor/Bussola.cpp
    Sensor/GerenciadorSensores.cpp
    Sensor/GPS.cpp
    Sensor/IMU.cpp
    Sensor/SensorDistancia.cpp
    Utils/Mutex.cpp
    Utils/Tipos.cpp
)

# Set Build output
set_target_properties( 
    AQC
    PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Library"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Library"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Binary"
)

# Incluir Bibliotecas
target_link_libraries(AQC
  PUBLIC
    Acoplador
    Algoritmo
    Controlador
    Modelo
    Sensor
    Utils
)

# Set Build output
set_target_properties( 
    AQC
    PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Library"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Library"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Binary"
)

# On post-build move library to UnrealEngine location
add_custom_command(
    TARGET AQC POST_BUILD
    COMMAND ${CMAKE_SOURCE_DIR}/MoveLibrary.bat $<CONFIG>
)