add_library(ps Expr.cpp Scanner.cpp Parser.cpp)
target_link_libraries(ps PRIVATE logger)
target_include_directories(ps PUBLIC ..)

add_subdirectory(tests)
