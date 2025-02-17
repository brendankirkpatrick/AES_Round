set(SOURCE_FILES
    main.cpp
    )

add_executable(AES ${SOURCE_FILES})
target_link_libraries(
    AES
    PRIVATE aes_warnings
)
                
