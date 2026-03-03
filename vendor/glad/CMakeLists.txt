file(GLOB_RECURSE SRC "*.cpp" "*.c")
file(GLOB_RECURSE HEADERS "*.hpp" "*.h")

set_source_files_properties(FILES ${HEADERS} PROPERTIES HEADER_FILE_ONLY TRUE)

add_library(glad ${SRC} ${HEADERS})

target_include_directories(glad PUBLIC include)
