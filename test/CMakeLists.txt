set(HEADER_FILES
    TestIO.h
    )
set(SOURCE_FILES
    test.cpp
    TestIO.cpp
    )

target_include_directories(AES PUBLIC ${PROJECT_SOURCE_DIR}/src)

add_executable(AES_UNIT_TEST ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(
    AES_UNIT_TEST
    PRIVATE aes_warnings
    PUBLIC src_files
)
                
