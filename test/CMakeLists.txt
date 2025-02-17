set(SOURCE_FILES
    test.cpp
    )

add_executable(AES_TEST ${SOURCE_FILES})
target_link_libraries(
    AES_TEST
    PRIVATE aes_warnings
)
                
