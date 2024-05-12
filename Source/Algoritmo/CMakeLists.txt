# Add library
add_library(Algoritmo)

# Include directory
target_include_directories(Algoritmo 
    PUBLIC 
        ${CMAKE_SOURCE_DIR}/Include/Algoritmo/
)

# Include Files
target_sources(Algoritmo
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/Algoritmo.cpp
    PUBLIC
        ${CMAKE_SOURCE_DIR}/Include/Algoritmo/Algoritmo.h
)

# Incluir Bibliotecas
target_link_libraries(Algoritmo
  PUBLIC
    Utils
)