add_executable(ChessThesis main.cpp
        chessboard.cpp
        chessboard.h
        engine.cpp
        engine.h
        evaluator.cpp
        evaluator.h
)

find_package(OpenMP REQUIRED)
target_link_libraries(ChessThesis OpenMP::OpenMP_CXX)