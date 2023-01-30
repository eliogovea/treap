project(flexible_array)

find_package(GTest REQUIRED)

add_executable(
    flexible_array_test 
    flexible_array_test.cpp
)

target_link_libraries(
    flexible_array_test 
    PRIVATE treap::treap 
            GTest::gtest_main
)

add_test(
    NAME flexible_array_test 
    COMMAND flexible_array_test
)
