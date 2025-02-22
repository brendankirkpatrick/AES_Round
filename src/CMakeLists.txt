# Header files
set(HEADER_FILES
    AES.h
    FileIO.h
    )

# Source Files
set(SOURCE_FILES
    AES.cpp
    FileIO.cpp
    tables.inl
    )

set(PROGRAM_ENTRY
    main.cpp
    )

# Add source files as library 
add_library(src_files ${SOURCE_FILES} ${HEADER_FILES})
set_target_properties(src_files 
                      PROPERTIES LINKER_LANGUAGE CXX 
                      OUTPUT_NAME "aesr1")
target_include_directories(src_files PUBLIC ${PROJECT_SOURCE_DIR}/src)

add_executable(AES ${PROGRAM_ENTRY})
target_link_libraries(
    AES
    PRIVATE aes_warnings
    PUBLIC src_files
)
