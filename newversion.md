# How to

1. Make all files executable

    ```shell
    chmod +x scripts/*.sh
    ```

2. build image

    ```shell
    docker build -t dear-glfw-vulkan-compiler .
    ```

3. Run conteiner interative mode

    ```shell
    docker run --rm -it -v $(pwd):/workspace dear-glfw-vulkan-compiler
    ```

4. Run the scripts

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
