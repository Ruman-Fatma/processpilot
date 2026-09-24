cmake_minimum_required(VERSION 3.16)
project(ProcessPilot VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra -Wpedantic)

include_directories(${CMAKE_SOURCE_DIR}/src)

add_library(processpilot_core
    src/daemon/daemon.cpp
    src/process/process_manager.cpp
    src/config/config_parser.cpp
    src/monitor/resource_monitor.cpp
    src/dependency/dependency_graph.cpp
    src/ipc/unix_socket.cpp
    src/logging/logger.cpp
)
target_link_libraries(processpilot_core PRIVATE pthread)

add_executable(processpilot src/main.cpp)
target_link_libraries(processpilot PRIVATE processpilot_core pthread)

add_executable(processpilot-cli cli/processpilot_cli.cpp)
target_link_libraries(processpilot-cli PRIVATE processpilot_core pthread)

add_executable(sample_server examples/sample_server.cpp)
add_executable(crash_service examples/crash_service.cpp)

add_executable(process_tests tests/process_tests.cpp)
target_link_libraries(process_tests PRIVATE processpilot_core pthread)
add_executable(config_tests tests/config_tests.cpp)
target_link_libraries(config_tests PRIVATE processpilot_core pthread)
add_executable(dependency_tests tests/dependency_tests.cpp)
target_link_libraries(dependency_tests PRIVATE processpilot_core pthread)

enable_testing()
add_test(NAME process_tests COMMAND process_tests)
add_test(NAME config_tests COMMAND config_tests)
add_test(NAME dependency_tests COMMAND dependency_tests)
