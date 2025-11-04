# Vulkan Dear-ImGui Glfw Glm

Container to compile everything together

## script to make a start setup

- Linux

  ```shell
  set -e

  IMAGE="srpatsu21/dear-glfw-vulkan-compiler"
  DEST="."

  echo "📦 Copying entire /workspace from image '$IMAGE' to local folder '$DEST'..."

  mkdir -p "$DEST"
  docker run --rm -v "$(pwd)/$DEST":/copy-dest "$IMAGE" \
    bash -c "cp -r /workspace/* /copy-dest/ || true"

  echo "✅ All files from /workspace copied to $DEST"
  ```

- Windows

  ```shell
  $ErrorActionPreference = "Stop"

  $image = "srpatsu21/dear-glfw-vulkan-compiler"
  $dest = "."

  Write-Host "📦 Copying entire /workspace from image '$image' to local folder '$dest'..."

  if (-not (Test-Path $dest)) {
      New-Item -ItemType Directory -Path $dest | Out-Null
  }

  docker run --rm -v "${PWD}\$dest:/copy-dest" $image `
      bash -c "cp -r /workspace/* /copy-dest/ || true"

  Write-Host "✅ All files from /workspace copied to $dest"
  ```

## Docker Compose setup

- docker-compose.yml

```yaml
services:
    builder:
        image: srpatsu21/dear-glfw-vulkan-compiler
        container_name: dear-glfw-vulkan-dev
        stdin_open: true
        tty: true
        working_dir: /workspace
        volumes:
            # create build files
            - ./build:/workspace/build
            - ./build-release:/workspace/build-release
            - ./build-windows:/workspace/build-windows
            - ./build-windows-release:/workspace/build-windows-release

            # map your libs and config
            - ./CMakeLists.txt:/workspace/CMakeLists.txt
            - ./toolchain-mingw.cmake:/workspace/toolchain-mingw.cmake
            - lib-data:/workspace/lib

            # make src interactive
            - ./src:/workspace/src

            # if you want to replace the scripts
            # - ./scripts:/workspace/scripts
        command: ["/bin/bash"]
```

- Acess the container to compile

```shell
docker compose up -d
docker exec -it dear-glfw-vulkan-dev bash
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

1. Run container interative mode

    ```shell
    docker run --rm -it -v $(pwd):/workspace srpatsu21/dear-glfw-vulkan-compiler
    ```

2. Run the scripts

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

3. Temporarily override files if you want

    ```shell
    docker run -it --rm \
    -v $(pwd)/src:/workspace/src \
    -v $(pwd)/MyAltCMakeLists.txt:/workspace/CMakeLists.txt \
    -v $(pwd)/MyAltToolchain-mingw.cmake:/workspace/toolchain-mingw.cmake \
    -v $(pwd)/MyAltLib:/workspace/lib \
    -v $(pwd)/MyAltScripts:/workspace/scripts \
    srpatsu21/dear-glfw-vulkan-compiler
    ```
