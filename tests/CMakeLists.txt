################################
# GTest
################################
ADD_SUBDIRECTORY (googletest-master)
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

file(GLOB_RECURSE TEST_SOURCES test_*.cpp)  # В переменной TEST_SOURCES лежат все тесты, имена файлов которые начинаются с test_

## link slae library
#add_library(slae STATIC IMPORTED)
#set_target_properties(slae PROPERTIES
#        IMPORTED_LOCATION "/home/evgen/Slaebase/libmy_project.a"
#        INTERFACE_INCLUDE_DIRECTORIES "/home/evgen/Slaebase/include")
#set(SLAE_LIBRARY slae)

foreach(TEST_FILE IN LISTS TEST_SOURCES)
    get_filename_component(TEST_FILE_NAME ${TEST_FILE} NAME_WE)
    set(EXECUTABLE_NAME ${TEST_FILE_NAME})

    add_executable(${EXECUTABLE_NAME} ${TEST_FILE})

    target_link_libraries(${EXECUTABLE_NAME} gtest gtest_main ${SLAE_LIBRARY})
    message("TEST_FILE: ${TEST_FILE}")
    add_test(NAME ${EXECUTABLE_NAME} COMMAND ${EXECUTABLE_NAME})
endforeach()


