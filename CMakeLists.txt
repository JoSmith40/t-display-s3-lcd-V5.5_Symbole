cmake_minimum_required(VERSION 3.5)

if (NOT DEFINED PROJECT_NAME)
    include($ENV{IDF_PATH}/tools/cmake/project.cmake)
    project(esp-idf-t-display-s3-lcd-V5.5_original)
else()
    message(FATAL_ERROR "esp-idf-t-display-s3-lcd-V5.5_original: This must be a project's main CMakeLists.txt.")
endif()
