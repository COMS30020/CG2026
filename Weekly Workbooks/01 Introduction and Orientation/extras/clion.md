# Installing SDL2 for use with CLion under Windows

1. Download SDL2 from the
[SDL releases repo](https://github.com/libsdl-org/SDL/releases/download/release-2.32.10/SDL2-devel-2.32.10-mingw.zip)
2. Unzip the archive and move the `x86_64-w64-mingw32` folder to somewhere appropriate on your filesystem
3. Open the project template provided for the Computer Graphics unit (if prompted, open it as a CMake project)
4. When the project wizard appears, set `toolchain` to "Use default MinGW" and `generator` to "Use default Ninja"
5. You should then see a CMake error regarding `FindSDL2.cmake` - this is a prompt to specify the location of SDL2
6. Edit `CMakeLists.txt` to set `SDL2_DIR` to the full path of the `lib\cmake\SDL2` folder in the SDL2 install. e.g.:
`(set SDL2_DIR C:\\user\\x86_64-w64-mingw32\\lib\\cmake\\SDL2\\)` (note the escaped file path separators !)
7. Close the whole project in CLion and then reopen it again (to force a reload of the `CMakeLists.txt` file)
8. Copy `SDL2.dll` from the `bin` folder in the SDL2 install and add it to the project's `cmake-build-debug` folder
9. You should now be able to run the project using the green play button at the bottom of the CLion window
