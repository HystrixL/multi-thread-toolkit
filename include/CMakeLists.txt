cmake_minimum_required(VERSION 3.5)
project(MultiThreadToolkit LANGUAGES CXX)

include(fetchcontent)      # 照写，不需要修改

fetchcontent_declare(NlohmannJson #库名字
        GIT_REPOSITORY https://github.com/nlohmann/json # 仓库地址
        SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/nlohmann_json # 指定库下载地址
        )

fetchcontent_makeavailable(NlohmannJson)

file(GLOB_RECURSE mtt_src *.hpp)

add_library(MultiThreadToolkit OBJECT ${mtt_src})

target_include_directories(MultiThreadToolkit PUBLIC ${PROJECT_SOURCE_DIR})

set_target_properties(MultiThreadToolkit PROPERTIES LINKER_LANGUAGE CXX)