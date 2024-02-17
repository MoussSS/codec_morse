enable_testing()

add_test(NAME test_blink
    COMMAND pytest -v -r test_blink.py
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
