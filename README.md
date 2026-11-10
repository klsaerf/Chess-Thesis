# Source Code for Thesis

This repository includes the source code for the Bachelor's thesis, titled "COMPARING PERFORMANCES OF MINIMAX AND ALPHA-BETA SEARCH ALGORITHMS FOR A CHESS ENGINE" by Kaan Pide, April 2026, and the thesis document itself.

## Project Overview

This project first makes a CLI based chess game, where the player is given the choice of playing color with the prompt:

```text
Enter the color to play (w/b): 
```

where 'w' is for White pieces, and 'b' is for Black pieces.

Then the program asks for the depth of the opposing engine:

```text
Enter depth for engine: 
```

which specifies how deep will the engine calculate the moves.

Finally, the game will begin:

```text
8  r n b q k b n r 
7  p p p p p p p p 
6  0 0 0 0 0 0 0 0 
5  0 0 0 0 0 0 0 0 
4  0 0 0 0 0 0 0 0 
3  0 0 0 0 0 0 0 0 
2  P P P P P P P P 
1  R N B Q K B N R 
   a b c d e f g h 

Enter move (q to exit): 
```

where the move is entered with the chess coordinates notation, with a space in between. 

White pieces are denoted with capital letters, whereas the Black pieces are denoted with lowercase letters.

The characters represent the pieces in chess notation:

```text
0 = empty space, P = pawn, N = knight, B = bishop, R = rook, Q = queen, K = king
```

When a pawn reaches the final square, it'll automatically become a Queen. En passant isn't implemented. 

To castle, input the chess notation as:

```text
Enter move (q to exit): 0-0     # short castle
Enter move (q to exit): 0-0-0   # long castle
```

Example run of the program:

```text
Enter the color to play (w/b): w
Enter depth for engine: 4
8  r n b q k b n r 
7  p p p p p p p p 
6  0 0 0 0 0 0 0 0 
5  0 0 0 0 0 0 0 0 
4  0 0 0 0 0 0 0 0 
3  0 0 0 0 0 0 0 0 
2  P P P P P P P P 
1  R N B Q K B N R 
   a b c d e f g h 

Enter move (q to exit): e2 e4   
339 ms to calculate
The bot played d7 to d5
8  r n b q k b n r 
7  p p p 0 p p p p 
6  0 0 0 0 0 0 0 0 
5  0 0 0 p 0 0 0 0 
4  0 0 0 0 P 0 0 0 
3  0 0 0 0 0 0 0 0 
2  P P P P 0 P P P 
1  R N B Q K B N R 
   a b c d e f g h 

Enter move (q to exit): q
```

Default engine is parallelAlphabeta engine.

## Thesis Overview

In the thesis, the performance differences between Minimax and Alphabeta algorithms are explored. Then, both algorithms were accelerated with CPU parallelism using OpenMP. Finally, tests are conducted on the engines and their performances and relative speed ups are compared. Refer to the thesis file found in the project, or using this link: https://urn.fi/URN:NBN:fi:tuni-202604264312

## Project Structure

```text
Chess-Thesis/
├── build/                  # Build folder
├── src/                    # Source files
│   ├── chessboard.cpp          # Chessboard implementation
│   ├── chessboard.h            # Chessboard header
│   ├── CMakeLists.txt          # CMakeLists file
│   ├── engine.cpp              # Engine implementation
│   ├── engine.h                # Engine header
│   ├── evaluator.cpp           # Evaluator implementation
│   ├── evaluator.h             # Evaluator header
│   └── main.cpp                # Main program
├── test/                   # Test files
│   ├── CMakeLists.txt          # CMakeLists file
│   ├── testposition.cpp        # Test Position program
│   └── testrepeat.cpp          # Test Repeat program
├── CMakeLists.txt          # CMakeLists
├── PideKaan.pdf            # Thesis report
├── .gitignore
└── README.md
```

## Building the Project


### Step 0:

This project requires OpenMP installed, thus run following commands if it is not installed.

Windows: OpenMP already installed

MacOS:
```bash
brew install libomp
```

Ubuntu/Debian:
```bash
sudo apt-get install libomp-dev
```

### Step 1:

This project uses CMake to build, make sure you have CMake installed before running the commands.

Build the project using CMake

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

### Step 2:

Run the main program:

MacOS/Ubuntu/Debian:
```bash
build/src/ChessThesis
```

Windows:
```bash
build/src/ChessThesis.exe
```

Run the tests:

MacOS/Ubuntu/Debian:
```bash
build/test/TestPosition <engine-depth>
build/test/TestRepeat <engine-depth> <repeat-amount> <verbose=v (leave empty for silent)>
```

Windows:
```bash
build/test/TestPosition.exe <engine-depth>
build/test/TestRepeat.exe <engine-depth> <repeat-amount> <verbose=v (leave empty for silent)>
```

Example tests:

```bash
build/test/TestPosition 4
build/test/TestRepeat 4 10 v
```