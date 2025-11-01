# Vulkan Dear-ImGui Glfw Glm

Container to compile everything together

You just need a src folder on workdir that is: `file(GLOB_RECURSE SOURCES src/*.cpp)`

if you use the default CmakeLists.txt make sure this:

```cmake
set(SHADER_DIR ${CMAKE_SOURCE_DIR}/src/client/assets/shaders)
set(COMPILED_SHADER_DIR ${CMAKE_BINARY_DIR}/shaders)

file(MAKE_DIRECTORY ${COMPILED_SHADER_DIR})

set(SHADERS
    ${SHADER_DIR}/vertex.glsl
    ${SHADER_DIR}/fragment.glsl
)
```

## Docker Compose setup

- docker-compose.yml

```yaml
services:
  builder:
    image: dear-glfw-vulkan-compiler
    container_name: dear-glfw-vulkan-dev
    build:
      context: .
      dockerfile: Dockerfile
    stdin_open: true
    tty: true
    working_dir: /workspace
    volumes:
      - ./build:/workspace/build
      - ./build-release:/workspace/build-release
      - ./build-windows:/workspace/build-windows
      - ./build-windows-release:/workspace/build-windows-release

      - ./src:/workspace/src
      - ./scripts:/workspace/scripts
    command: ["/bin/bash"]
```

- run

```shell
docker compose build
```

```shell
docker compose run builder
```

- run script
  - Linux Debug

    ```shell
    ./scripts/build_linux.sh
    ```

  - Linux Release

    ```shell
    ./scripts/build_linux_release.sh
    ```

  - Windows Debug

    ```shell
    ./scripts/build_windows.sh
    ```

  - Windows Release

    ```shell
    ./scripts/build_windows_release.sh
    ```

  - Others scripts

    ```shell
        ./scripts/build_all_debug.sh
        ./scripts/build_all_release.sh
        ./scripts/build_all.sh
    ```

## How to setup just docker (if already docker compose no need)

1. build image

    ```shell
    docker build -t dear-glfw-vulkan-compiler .
    ```

2. Run container interative mode

    ```shell
    docker run --rm -it -v $(pwd):/workspace dear-glfw-vulkan-compiler
    ```

3. Run the scripts

    - Linux Debug

        ```shell
        ./scripts/build_linux.sh
        ```

    - Linux Release

        ```shell
        ./scripts/build_linux_release.sh
        ```

    - Windows Debug

        ```shell
        ./scripts/build_windows.sh
        ```

    - Windows Release

        ```shell
        ./scripts/build_windows_release.sh
        ```

    - Others scripts

        ```shell
            ./scripts/build_all_debug.sh
            ./scripts/build_all_release.sh
            ./scripts/build_all.sh
        ```

4. Temporarily override files if you want

    ```shell
    docker run -it --rm \
    -v $(pwd)/src:/workspace/src \
    -v $(pwd)/MyAltCMakeLists.txt:/workspace/CMakeLists.txt \
    -v $(pwd)/MyAltToolchain-mingw.cmake:/workspace/toolchain-mingw.cmake \
    -v $(pwd)/MyAltLib:/workspace/lib \
    -v $(pwd)/MyAltScripts:/workspace/scripts \
    dear-glfw-vulkan-compiler
    ```
