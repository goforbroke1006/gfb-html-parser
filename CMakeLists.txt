cmake_minimum_required(VERSION 3.0)

project(gfb-html-parser VERSION 1.0.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 14)

include_directories(
        ${CMAKE_SOURCE_DIR}/vendor
        ${CMAKE_SOURCE_DIR}/vendor/hcxselect/src
)

add_library(gfb-html-parser SHARED
        src/gfb-html-parser.cpp
        ${CMAKE_SOURCE_DIR}/vendor/hcxselect/src/hcxselect.cpp
        )

target_link_libraries(gfb-html-parser
        curl
        htmlcxx
        )

set_target_properties(gfb-html-parser PROPERTIES PUBLIC_HEADER include/gfb-html-parser.h)
install(TARGETS gfb-html-parser LIBRARY DESTINATION lib)
install(DIRECTORY include/ DESTINATION include/${PROJECT_NAME})
#install(DIRECTORY ../../vendor/hcxselect/src DESTINATION include/hcxselect/src)

install(
        DIRECTORY ${CMAKE_SOURCE_DIR}/vendor/hcxselect/src/
        DESTINATION include/${PROJECT_NAME}/hcxselect/src
        FILES_MATCHING PATTERN "*.h*"
)
