# How to setup

1. build image

    ```shell
    docker build -t dear-glfw-vulkan-compiler .
    ```

2. Run conteiner interative mode

    You just need to set the volumes that you will use.

    ```shell
    docker run --rm -it \
        -v $(pwd)/src:/workspace/src \
        -v $(pwd)/build:/workspace/build \
        -v $(pwd)/build-release:/workspace/build-release \
        -v $(pwd)/build-windows:/workspace/build-windows \
        -v $(pwd)/build-windows-release:/workspace/build-windows-release \
        dear-glfw-vulkan-compiler
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

4. Temporarily override files

    ```shell
    docker run -it --rm \
        -v $(pwd)/src:/workspace/src \
        -v $(pwd)/MyAltCMakeLists.txt:/workspace/CMakeLists.txt \
        -v $(pwd)/MyAltToolchain-mingw.cmake:/workspace/toolchain-mingw.cmake \
        -v $(pwd)/MyAltLib:/workspace/Lib \
        -v $(pwd)/MyAltScripts:/workspace/scripts \
        dear-glfw-vulkan-compiler
    ```
