# Build catch main
add_library(catch_main STATIC catch_main.cpp)



set(SerialTestSources
    test_math.cpp;
    test_fields.cpp;
    test_equation.cpp;
)


add_executable(SerialTest.bin ${SerialTestSources})

#set_target_properties(SerialTest.bin PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})
target_link_libraries(SerialTest.bin PUBLIC project_options catch_main io)
#target_compile_options(SerialTest.bin PRIVATE -DDEBUG)

add_test( NAME SerialTest COMMAND SerialTest.bin)
