set -e

mkdir -p build-release
cd build-release
cmake .. -DCMAKE_BUILD_TYPE=Release -G Ninja
cmake --build .