cmake_minimum_required(VERSION 3.5)
project(multi_thread_toolkit LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

add_compile_options(-Ofast)

file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/example/*.cpp")

add_subdirectory(include)

foreach(src ${srcs})
    get_filename_component(name ${src} NAME)
    add_executable(${name} ${src})
    target_link_libraries(${name} MultiThreadToolkit)
endforeach()
