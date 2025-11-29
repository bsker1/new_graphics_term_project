cmake_minimum_required(VERSION 3.13)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")


project(graphics_term_project)
set(CMAKE_CXX_STANDARD 17)

add_executable(graphics_term_project src/main.cpp src/glad.c)


target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/res/") # This is useful to get an ASSETS_PATH in your IDE during development but you should comment this if you compile a release version and uncomment the next line
#target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCES_PATH="./resources/") # Uncomment this line to setup the ASSETS_PATH macro to the final assets directory when you share the project


target_include_directories(graphics_term_project PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)
target_link_libraries(graphics_term_project PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/libglfw3.a)
