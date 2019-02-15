cmake_minimum_required(VERSION 3.0)

project(gfb-html-parser VERSION 1.0.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 14)

include_directories(
        ${CMAKE_SOURCE_DIR}/vendor
        ${CMAKE_SOURCE_DIR}/vendor/hcxselect/src
)

add_library(${PROJECT_NAME} SHARED
        src/gfb-html-parser.cpp
        ${CMAKE_SOURCE_DIR}/vendor/hcxselect/src/hcxselect.cpp
        )

target_link_libraries(${PROJECT_NAME}
        curl
        htmlcxx
        )

set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER include/gfb-html-parser.h)
install(TARGETS ${PROJECT_NAME}
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        COMPONENT library
        )
install(DIRECTORY include/ DESTINATION include/${PROJECT_NAME})
#install(DIRECTORY ../../vendor/hcxselect/src DESTINATION include/hcxselect/src)

install(
        DIRECTORY ${CMAKE_SOURCE_DIR}/vendor/hcxselect/src/
        DESTINATION include/${PROJECT_NAME}
        FILES_MATCHING PATTERN "*.h*"
)
