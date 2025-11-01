# Vulkan Dear-ImGui Glfw Glm
Container to compile everything together

## How to setup

1. build image

    ```shell
    docker build -t dear-glfw-vulkan-compiler .
    ```

2. Run conteiner interative mode

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
    -v $(pwd)/MyAltLib:/workspace/Lib \
    -v $(pwd)/MyAltScripts:/workspace/scripts \
    dear-glfw-vulkan-compiler
    ```
