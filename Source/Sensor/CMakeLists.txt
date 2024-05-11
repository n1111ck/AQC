# Add library
add_library(Sensor "")

# Include directory
target_include_directories(Sensor 
    PUBLIC 
        ${CMAKE_SOURCE_DIR}/Include/Sensor/
)

# Include Files
target_sources(Sensor
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/Barometro.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Bussola.cpp
        ${CMAKE_CURRENT_LIST_DIR}/GerenciadorSensores.cpp
        ${CMAKE_CURRENT_LIST_DIR}/GPS.cpp
        ${CMAKE_CURRENT_LIST_DIR}/IMU.cpp
        ${CMAKE_CURRENT_LIST_DIR}/SensorDistancia.cpp
    PUBLIC
        ${CMAKE_SOURCE_DIR}/Include/Sensor/Barometro.h
        ${CMAKE_SOURCE_DIR}/Include/Sensor/Bussola.h
        ${CMAKE_SOURCE_DIR}/Include/Sensor/GerenciadorSensores.h
        ${CMAKE_SOURCE_DIR}/Include/Sensor/GPS.h
        ${CMAKE_SOURCE_DIR}/Include/Sensor/IMU.h
        ${CMAKE_SOURCE_DIR}/Include/Sensor/SensorDistancia.h
        ${CMAKE_SOURCE_DIR}/Include/Sensor/ISensor.h
)

# Incluir Bibliotecas
target_link_libraries(Sensor
  PUBLIC
    Modelo
    Utils
)