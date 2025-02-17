set(SOURCE_FILES
    test.cpp
    )

add_executable(AES_UNIT_TEST ${SOURCE_FILES})
target_link_libraries(
    AES_UNIT_TEST
    PRIVATE aes_warnings
)
                
