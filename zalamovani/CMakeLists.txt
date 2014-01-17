project(zalamovani)

set(CMAKE_CXX_FLAGS "-std=c++11")

add_executable(zalamovani main.cpp)

install(TARGETS zalamovani RUNTIME DESTINATION bin)
