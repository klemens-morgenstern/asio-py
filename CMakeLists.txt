cmake_minimum_required(VERSION 3.16)
project(asio_py VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS -stdlib=libc++)


execute_process(
        COMMAND conan install ${CMAKE_CURRENT_SOURCE_DIR} --profile ${CMAKE_CURRENT_SOURCE_DIR}/clang --build=missing
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        RESULT_VARIABLE conan_RESULT)
if (conan_RESULT)
    message(FATAL_ERROR "[dependency] Conan failed.")
endif()

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)
include_directories(${CONAN_INCLUDE_DIRS} ./include)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH};${CMAKE_BINARY_DIR})

find_package(Python3 REQUIRED COMPONENTS Interpreter Development) # we need python3-devel
set(pybind11_INSTALL_DIR ${CONAN_PYBIND11_ROOT})
find_package(pybind11 REQUIRED)

set(asio_py_src
        src/asio-py/io_context.cpp
        src/asio-py/execution.cpp
        src/asio-py/execution_context.cpp
        src/asio-py/cancellation_signal.cpp
        src/asio-py/cancellation_state.cpp
        src/asio-py/cancellation_type.cpp
        src/asio-py/detached.cpp
        src/asio-py/error.cpp
        src/asio-py/pipe.cpp
        src/asio-py/register_all.cpp
        src/asio-py/system_context.cpp
        src/asio-py/this_coro.cpp
        src/asio-py/thread_pool.cpp
        src/asio-py/timer.cpp
        src/asio-py/ip/tcp.cpp src/asio-py/co_spawn.cpp)

add_library(asio-py ${asio_py_src})

target_link_libraries(asio-py PUBLIC Python3::Module)
pybind11_add_module(asio src/module.cpp ${asio_py_src})
target_link_libraries(asio PUBLIC Python3::Module)


add_subdirectory(test)