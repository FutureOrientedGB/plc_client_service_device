cmake_minimum_required(VERSION 3.20)

# set project name
set(PROJECT_NAME plc)

# declare project
project(${PROJECT_NAME})

# find deps
find_package(CLI11 CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(libmodbus CONFIG REQUIRED)

# defines
ADD_DEFINITIONS(-DUNICODE -D_UNICODE)
ADD_DEFINITIONS(-DVC_EXTRALEAN)
ADD_DEFINITIONS(-DWIN32_LEAN_AND_MEAN)

# classify filters
FILE(GLOB_RECURSE HEADER_FILES
        "*.hpp"
        "../protocol/*.hpp"
)
FILE(GLOB_RECURSE SOURCE_FILES
        "*.cpp"
        "../protocol/*.cpp"
)
SOURCE_GROUP("Header Files" FILES ${HEADER_FILES})
SOURCE_GROUP("Source Files" FILES ${SOURCE_FILES})

# executable
add_executable(
    ${PROJECT_NAME}
    ${HEADER_FILES}
    ${SOURCE_FILES}
)

# Visual Studio - Properity - C/C++ - Code Generation - Rutime Library > /MT
if(MSVC)
set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
)
endif(MSVC)

# link deps
target_link_libraries(
        ${PROJECT_NAME}
        PRIVATE
        CLI11::CLI11
        fmt::fmt
        spdlog::spdlog
        modbus
)


