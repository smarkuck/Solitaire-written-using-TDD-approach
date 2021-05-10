# Solitaire written using TDD approach
![alt text](https://github.com/smarkuck/Solitaire-written-using-TDD-approach/blob/master/solitaire.png?raw=true)

# How to build
> **Note:** Used only Boost library headers needed for MSVC and gcc compiler. If compilation fails replace boost directory with https://boostorg.jfrog.io/artifactory/main/release/1.75.0/source/

## Windows - Visual Studio Code
1. download SDL2 runtime library: https://www.libsdl.org/download-2.0.php
2. add SDL2.dll to C:\Windows\System32
3. git clone https://github.com/smarkuck/Solitaire-written-using-TDD-approach.git
4. git submodule update --init --recursive
5. install VS Code: https://code.visualstudio.com/
6. install Microsoft extensions: C/C++, C/C++ Extension Pack
7. install Build Tools for Visual Studio 2019: https://visualstudio.microsoft.com/pl/downloads/
8. open VS Code
9. press F1, select "CMake: Select a Kit"
10. select "Visual Studio Build Tools 2019 Release - [architecture]"
11. press F1, select "CMake: Configure"
12. press F1, select "CMake: Build Target"
13. select ALL_BUILD
> **Note:** If application does not start, remove WIN32 in first line in SolitaireExe/CMakeLists.txt to see runtime errors in console

## Linux - gcc
1. sudo apt install cmake libsdl2-dev g++
2. git clone https://github.com/smarkuck/Solitaire-written-using-TDD-approach.git
3. cd Solitaire-written-using-TDD-approach
4. git submodule update --init --recursive
5. mkdir build
6. cd build
7. cmake ..
8. make -j \`nproc\`
