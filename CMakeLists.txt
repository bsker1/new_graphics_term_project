cmake_minimum_required(VERSION 3.13)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")


project(opengl_tutorial)
set(CMAKE_CXX_STANDARD 17)

add_executable(opengl_tutorial src/main.cpp lib/glad.c lib/stb.cpp src/Shader.cpp src/VertexBuffer.cpp src/IndexBuffer.cpp src/VertexArray.cpp src/Texture.cpp src/Camera.cpp)


target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/res/") # This is useful to get an ASSETS_PATH in your IDE during development but you should comment this if you compile a release version and uncomment the next line
#target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCES_PATH="./resources/") # Uncomment this line to setup the ASSETS_PATH macro to the final assets directory when you share the project


target_include_directories(opengl_tutorial PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)
target_link_libraries(opengl_tutorial PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/libglfw3.a)
