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
        ${CMAKE_CURRENT_LIST_DIR}/CascataPD.cpp
        ${CMAKE_CURRENT_LIST_DIR}/LRE.cpp
    PUBLIC
        ${CMAKE_SOURCE_DIR}/Include/Controlador/CascataPD.h
        ${CMAKE_SOURCE_DIR}/Include/Controlador/LRE.h
        ${CMAKE_SOURCE_DIR}/Include/Controlador/IControlador.h
)

# Incluir Bibliotecas
target_link_libraries(Controlador
  PUBLIC
    Acoplador
    Sensor
    Utils
)