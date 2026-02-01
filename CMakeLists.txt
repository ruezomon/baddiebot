cmake_minimum_required(VERSION 3.22)

project(baddiebot VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_executable(${PROJECT_NAME}
    src/main.cpp
)

# IMPORTANT: use CONFIG mode
find_package(dpp CONFIG REQUIRED)

# Link the exported target
target_link_libraries(${PROJECT_NAME}
    PRIVATE
    dpp::dpp
    pthread
)