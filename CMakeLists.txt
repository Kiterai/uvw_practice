project(libuvtest)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/target) 
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/target) 
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/target) 

add_executable(srv srv.cpp)
add_executable(cli cli.cpp)

find_package(uvw CONFIG REQUIRED)
target_link_libraries(srv PRIVATE uvw::uvw)
target_link_libraries(cli PRIVATE uvw::uvw)

target_precompile_headers(srv PRIVATE pch.hpp)
target_precompile_headers(cli PRIVATE pch.hpp)
