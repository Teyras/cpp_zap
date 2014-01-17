project(interpreter)

set(CMAKE_CXX_FLAGS "-std=c++11")

add_executable(interpreter main.cpp)

install(TARGETS interpreter RUNTIME DESTINATION bin)
