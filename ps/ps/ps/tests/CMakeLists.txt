add_executable(ps.tests ps.scanner.tests.cpp ps.parser.tests.cpp ps.static_analysis.tests.cpp)
target_link_libraries(ps.tests PRIVATE ps GTest::GTest GTest::Main)
