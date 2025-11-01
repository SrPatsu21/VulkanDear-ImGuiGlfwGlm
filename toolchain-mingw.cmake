set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)
set(CMAKE_RC_COMPILER x86_64-w64-mingw32-windres)

# SET CMAKE_FIND ROOT PATHS TO FIND BINARIES OR COMPILERS
set(CMAKE_FIND_ROOT_PATH # COMPILER
    /usr/x86_64-w64-mingw32
    # VENDOR LIBS
)

# adjust the default behavior of the FIND_XXX() commands: search programs in the
# host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# search headers and libraries in the target environment
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# Vulkan Paths (modify based on your extracted Vulkan SDK)
set(VULKAN_SDK_PATH "./lib/vulkan-sdk-win")
set(CMAKE_FIND_ROOT_PATH "${VULKAN_SDK_PATH}")
set(CMAKE_PREFIX_PATH "${VULKAN_SDK_PATH}")