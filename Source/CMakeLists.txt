#
# Executable Target
#
add_executable(AQC-Main main.cpp)
  
# Source subdirectories
add_subdirectory(Acoplador)
add_subdirectory(Controlador)
add_subdirectory(Modelo)
add_subdirectory(Sensor)
add_subdirectory(Utils)

# Incluir Bibliotecas
link_directories(
    Acoplador/
    Controlador/
    Modelo/
    Sensor/
    Utils/
)

# Incluir Bibliotecas
target_link_libraries(AQC-Main
  PUBLIC
    Acoplador
    Controlador
    Modelo
    Sensor
    Utils
)

# Include directory
target_include_directories(AQC-Main 
    PUBLIC 
        ${CMAKE_SOURCE_DIR}/Include/
)

# Include Files
target_sources(AQC-Main
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/AQC.cpp
    PUBLIC
        ${CMAKE_SOURCE_DIR}/Include/AQC.h
)

#
# Library Target
#
add_library(AQC 
    AQC.cpp
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

# Incluir Bibliotecas
target_link_libraries(AQC
  PUBLIC
    Acoplador
    Controlador
    Modelo
    Sensor
    Utils
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