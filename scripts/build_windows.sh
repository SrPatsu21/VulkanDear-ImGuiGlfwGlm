set -e

mkdir -p build-windows
cd build-windows
cmake .. -D CMAKE_TOOLCHAIN_FILE=../toolchain-mingw.cmake -G Ninja
cmake --build .