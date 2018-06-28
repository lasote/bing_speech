project(BingSpeech)
cmake_minimum_required(VERSION 2.8.12)

include(${CMAKE_CURRENT_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_library(bing_speech src/bing_speech.cpp include/bing_speech.h)
target_link_libraries(bing_speech ${CONAN_LIBS})
