cmake_minimum_required(VERSION 3.28)

set(CMAKE_CXX_COMPILER "/bin/g++")
set(CMAKE_C_COMPILER "/bin/gcc")


# cleanup build dir
if(EXISTS ${CMAKE_BINARY_DIR})
    file(REMOVE_RECURSE ${CMAKE_BINARY_DIR})
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # for LSP support

set(SFML_STATIC_LIBRARIES ON)

project(SFMLProject LANGUAGES CXX)

find_library(SFML PATHS /usr/lib/ /usr/local/lib/ /usr/lib/cmake/)
find_package(SFML 3 COMPONENTS Graphics System Window Audio Network REQUIRED)


if (NOT SFML_FOUND)
    include(FetchContent)
    FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.0
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM)
    FetchContent_MakeAvailable(SFML)
endif()

include_directories(include)

file(GLOB main_src CONFIGURE_DEPENDS "src/*.cpp" "src/item/*.cpp")
file(COPY 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/entities.json
    ${CMAKE_CURRENT_SOURCE_DIR}/src/item/items.json
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
)

add_executable(main ${main_src})
target_link_libraries(main PRIVATE SFML::Graphics SFML::Audio SFML::Network SFML::System SFML::Window)
