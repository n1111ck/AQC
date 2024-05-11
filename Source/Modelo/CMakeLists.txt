# Add library
add_library(Modelo)

# Include directory
target_include_directories(Modelo 
    PUBLIC 
        ${CMAKE_SOURCE_DIR}/Include/Modelo/
)

# Include Files
target_sources(Modelo
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/Modelo.cpp
    PUBLIC
        ${CMAKE_SOURCE_DIR}/Include/Modelo/Modelo.h
)

# Incluir Bibliotecas
target_link_libraries(Modelo
  PUBLIC
    Utils
)