# Build catch main
add_library(catch_main STATIC catch_main.cpp)



set(SerialTestSources
    test_convection.cpp;
    test_common.cpp;
    test_differentiation.cpp;
    test_equation.cpp;
)

add_executable(SerialTest.bin ${SerialTestSources})
target_link_libraries(SerialTest.bin PUBLIC project_options catch_main convection)
target_compile_options(SerialTest.bin PRIVATE -DDEBUG)
add_test( NAME SerialTest COMMAND SerialTest.bin)
