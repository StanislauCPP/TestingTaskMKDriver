set(LIB_NAME "SPI_commonAPI")

add_library(${LIB_NAME} STATIC src/${LIB_NAME}.c)
set_target_properties(${LIB_NAME} PROPERTIES LINKER_LANGUAGE C)
target_include_directories(${LIB_NAME} PUBLIC "include")