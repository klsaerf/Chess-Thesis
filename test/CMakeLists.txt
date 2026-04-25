add_executable(TestRepeat testrepeat.cpp
        ../src/chessboard.cpp
        ../src/chessboard.h
        ../src/engine.cpp
        ../src/engine.h
        ../src/evaluator.cpp
        ../src/evaluator.h
)

add_executable(TestPosition testposition.cpp
        ../src/chessboard.cpp
        ../src/chessboard.h
        ../src/engine.cpp
        ../src/engine.h
        ../src/evaluator.cpp
        ../src/evaluator.h
)

find_package(OpenMP REQUIRED)
target_link_libraries(TestRepeat OpenMP::OpenMP_CXX)
target_link_libraries(TestPosition OpenMP::OpenMP_CXX)

# Add tests for TestRepeat with arguments for the depth, repeat and verbose
# The first case tests depth 5 for 10 times, and returns the average runtime
# Any argument passed to verbose makes the output show the progress
add_test(TestRepeat1 TestRepeat 4 1000 v)
add_test(TestRepeat2 TestRepeat 5 100 v)
add_test(TestRepeat3 TestRepeat 6 10 v)

# Add tests for a set position with argument for the depth
add_test(TestPosition1 TestPosition 4)
add_test(TestPosition2 TestPosition 6)