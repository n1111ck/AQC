# Add library
add_library(Utils)

# Include directory
target_include_directories(Utils 
    PUBLIC 
        ${CMAKE_SOURCE_DIR}/Include/
)

# Include Files
target_sources(Utils
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/Mutex.cpp
        ${CMAKE_CURRENT_LIST_DIR}/Tipos.cpp
    PUBLIC
        ${CMAKE_SOURCE_DIR}/Include/Utils/Mutex.h
        ${CMAKE_SOURCE_DIR}/Include/Utils/Tipos.h
        ${CMAKE_SOURCE_DIR}/Include/Utils/Constantes.h
)