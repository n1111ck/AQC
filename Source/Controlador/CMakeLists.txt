# Add library
add_library(Controlador "")

# Include directory
target_include_directories(Controlador 
    PUBLIC 
        ${CMAKE_SOURCE_DIR}/Include/
)

# Include Files
target_sources(Controlador
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/RNL.cpp
    PUBLIC
        ${CMAKE_SOURCE_DIR}/Include/Controlador/RNL.h
        ${CMAKE_SOURCE_DIR}/Include/Controlador/IControlador.h
)

# Incluir Bibliotecas
target_link_libraries(Controlador
  PUBLIC
    Acoplador
    Sensor
    Utils
)