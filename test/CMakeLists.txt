enable_testing()

function(add_py_test script)
    add_test(NAME ${script}
            COMMAND
            ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/${script}.py
            ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
            WORKING_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY} )
endfunction()

add_py_test(io_context)

pybind11_add_module(template_test_impl template_test_impl.cpp)
target_link_libraries(template_test_impl PUBLIC Python3::Module)
add_py_test(template_test)
add_py_test(pipe)

add_executable(embed embed.cpp)
target_link_libraries(embed PUBLIC asio-py Python3::Module Python3::Python)

add_test(NAME embed
        COMMAND
        $<TARGET_FILE:embed>
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})