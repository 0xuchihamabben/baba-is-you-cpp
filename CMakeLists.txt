cmake_minimum_required(VERSION 3.14)

project(BabaIsYou LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_POLICY_VERSION_MINIMUM "3.5")

include(FetchContent)
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
)
FetchContent_MakeAvailable(SFML)

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.hpp")
list(FILTER SOURCES EXCLUDE REGEX "test_model.cpp")

add_executable(BabaIsYou ${SOURCES})

target_link_libraries(BabaIsYou PRIVATE SFML::Graphics SFML::Audio SFML::Network SFML::Window SFML::System)

# Copy assets to build directory
add_custom_command(TARGET BabaIsYou POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:BabaIsYou>/assets)
