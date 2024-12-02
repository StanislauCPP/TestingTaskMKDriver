set(LIB_NAME "TLE8242")

add_library(${LIB_NAME} STATIC src/${LIB_NAME}.c)
set_target_properties(${LIB_NAME} PROPERTIES LINKER_LANGUAGE C)
target_include_directories(${LIB_NAME} PUBLIC "include")

add_subdirectory(SPI_commonAPI)
target_link_libraries(${LIB_NAME} PRIVATE SPI_commonAPI)