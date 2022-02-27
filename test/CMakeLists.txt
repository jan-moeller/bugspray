
add_executable(bugspray-test test.cpp test2.cpp)
target_link_libraries(bugspray-test bugspray-with-main)
target_compile_options(bugspray-test PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Wduplicated-cond
        -Wno-duplicated-branches
        -Wlogical-op
        -Wnull-dereference
        -Wno-old-style-cast
        -Wuseless-cast
        -Wdouble-promotion
        -Wshadow
        -Wformat=2
        -Wuninitialized
        )
set_target_properties(bugspray-test PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
        )