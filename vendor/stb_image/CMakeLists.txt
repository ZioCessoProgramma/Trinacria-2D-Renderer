add_library(stb_image STATIC stb_image.cpp include/stb_image/stb_image.h)
set_source_files_properties(FILES include/stb_image/stb_image.h PROPERTIES HEADER_FILE_ONLY TRUE)
target_include_directories(stb_image PUBLIC include)