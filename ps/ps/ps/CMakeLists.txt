add_library(ps ps.cpp Expr.cpp Scanner.cpp)
target_link_libraries(ps PRIVATE logger)
target_include_directories(ps PUBLIC ..)

add_subdirectory(tests)
