file(GLOB_RECURSE SRC CONFIGURE_DEPENDS "*.c" "*.cpp")
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "*.h" "*.hpp")
file(GLOB_RECURSE TEXTURES CONFIGURE_DEPENDS "assets/textures/*")
file(GLOB_RECURSE SHADERS CONFIGURE_DEPENDS "assets/shaders/*")

set_source_files_properties(FILES ${HEADERS} ${TEXTURES} ${SHADERS} PROPERTIES HEADER_FILE_ONLY TRUE)

add_executable(Test1 ${SRC} ${HEADERS} ${TEXTURES} ${SHADERS})

target_link_libraries(Test1 PUBLIC Trinacria)
target_include_directories(Test1 PUBLIC include)