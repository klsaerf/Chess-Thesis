add_executable(ChessThesis main.cpp
        chessboard.cpp
        chessboard.h
        engine.cpp
        engine.h
        evaluator.cpp
        evaluator.h
)

# Enable OpenMP on MacOS
if(APPLE)
    execute_process(
            COMMAND brew --prefix
            OUTPUT_VARIABLE HOMEBREW_PREFIX
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    set(ENV{OpenMP_ROOT} "${HOMEBREW_PREFIX}/opt/libomp")
    set(ENV{LDFLAGS} "$LDFLAGS -L${OpenMP_ROOT}/lib")
    set(ENV{CPPFLAGS} "-I${OpenMP_ROOT}/include")
endif()

find_package(OpenMP REQUIRED)
target_link_libraries(ChessThesis OpenMP::OpenMP_CXX)