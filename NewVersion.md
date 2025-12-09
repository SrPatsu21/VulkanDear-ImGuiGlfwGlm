# How to create new version

1. Copy old env

    ```shell
    #!/usr/bin/env bash
    set -e

    IMAGE="srpatsu21/dear-glfw-vulkan-compiler:latest"
    DEST="."

    mkdir -p "$DEST"
    mkdir -p "$DEST/vscode-extensions"

    docker run --rm \
        -v "$(pwd)/$DEST":/copy-dest \
        "$IMAGE" \
        bash -c "
            cp -r /workspace/* /copy-dest/ || true
            cp -r /root/.vscode-server/extensions /copy-dest/vscode-extensions/ || true
        "

    echo "files from container copied to $DEST"
    ```

2. Change the env as you want

3. build image

    ```shell
    docker build -t dear-glfw-vulkan-compiler .
    ```

4. Run conteiner interative mode

    You just need to set the volumes that you will use.

    ```shell
    docker run --rm -it \
        -v $(pwd)/src:/workspace/src \
        -v $(pwd)/build:/workspace/build \
        -v $(pwd)/build-release:/workspace/build-release \
        -v $(pwd)/build-windows:/workspace/build-windows \
        -v $(pwd)/build-windows-release:/workspace/build-windows-release \
        -v $(pwd)/CMakeLists.txt:/workspace/CMakeLists.txt \
        -v $(pwd)/toolchain-mingw.cmake:/workspace/toolchain-mingw.cmake \
        -v $(pwd)/lib:/workspace/lib \
        -v $(pwd)/scripts:/workspace/scripts \
        dear-glfw-vulkan-compiler
    ```

5. Run the scripts to compile for:

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
