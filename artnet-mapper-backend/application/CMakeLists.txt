
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${root_dir}/bin)

add_executable(app application.cpp runtime.cpp)
target_link_libraries(app PRIVATE pthread service)