# Add library
add_library(Acoplador)

# Include directory
target_include_directories(Acoplador 
    PUBLIC 
        ${CMAKE_SOURCE_DIR}/Include/
)

# Include Files
target_sources(Acoplador
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/Acoplador.cpp
        ${CMAKE_CURRENT_LIST_DIR}/GerenciadorAcopladores.cpp
    PUBLIC
        ${CMAKE_SOURCE_DIR}/Include/Acoplador/Acoplador.h
        ${CMAKE_SOURCE_DIR}/Include/Acoplador/GerenciadorAcopladores.h
)

# Incluir Bibliotecas
target_link_libraries(Acoplador
  PUBLIC
    Modelo
    Utils
)