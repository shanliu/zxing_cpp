cmake_minimum_required(VERSION 3.20)
project(zxing_cpp_php)

set(CMAKE_CXX_STANDARD 17)

include_directories(./stb-src)
include_directories(./zxing-cpp-src/core/src)

add_library(_ SHARED zxing_write_class.c zxing_read_class.c zxing_cpp.c zxing_cpp_lib.cpp zxing_common.c zxing_read_result_class.c zxing_write_result_class.c zxing_image_class.c zxing_exception_class.c )
add_custom_target(ext_install COMMAND phpize --clean && phpize &&./configure && make  && make install WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
add_custom_target(ext_clean COMMAND phpize --clean && rm -rf ./include && rm -rf ./*.dep WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
add_custom_target(ext_fast_install COMMAND make  && make install  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})