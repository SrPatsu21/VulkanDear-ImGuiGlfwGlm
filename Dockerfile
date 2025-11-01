FROM ubuntu:25.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && \
    apt-get install -y --no-install-recommends build-essential cmake ninja-build pkg-config git mingw-w64 ca-certificates curl wget

# GLFW
RUN apt-get install -y --no-install-recommends libgl-dev wayland-protocols libwayland-bin libwayland-dev libxkbcommon-dev libxrandr-dev \
        libxinerama-dev libxcursor-dev libxi-dev pkg-config mingw-w64 mingw-w64-x86-64-dev libgl1-mesa-dev
# Vulkan
RUN curl -fsSL https://packages.lunarg.com/lunarg-signing-key-pub.asc | tee /etc/apt/trusted.gpg.d/lunarg.gpg > /dev/null && \
    wget -qO /etc/apt/sources.list.d/lunarg-vulkan-1.3.296-noble.list "https://packages.lunarg.com/vulkan/1.3.296/lunarg-vulkan-1.3.296-noble.list" && \
    apt-get update && \
    apt-get install -y --no-install-recommends vulkan-utility-libraries-dev libvulkan-dev vulkan-tools vulkan-validationlayers
# GLM
RUN apt-get install -y --no-install-recommends libglm-dev
# GLS Lang Validator
RUN apt-get install -y --no-install-recommends glslang-tools
# Assimp
RUN apt-get install -y --no-install-recommends libassimp-dev
# Embedded libs
RUN RUN apt-get update && \
    apt-get install -y --no-install-recommends apt-file libdecor-0-0 libdecor-0-plugin-1-gtk libdecor-0-plugin-1-cairo libgtk-3-0 gnome-themes-extra-data && \
    apt-file update && \
    DEST=lib/linux && \
    mkdir -p "$DEST/share" && \
    LIBS="libgtk-3.so.0 libgdk-3.so.0 libdecor-0.so.0" && \
    for lib in $LIBS; do \
        cp "/usr/lib/x86_64-linux-gnu/$lib" "$DEST/"; \
        ldd "/usr/lib/x86_64-linux-gnu/$lib" | awk '{print $3}' | grep -E '^/' | while read dep; do \
            cp -u "$dep" "$DEST/" 2>/dev/null || true; \
        done; \
    done && \
    mkdir -p "$DEST/libdecor/plugins-1" && \
    cp /usr/lib/x86_64-linux-gnu/libdecor/plugins-1/* "$DEST/libdecor/plugins-1/" && \
    mkdir -p "$DEST/share/themes" && \
    cp -r /usr/share/themes/Adwaita "$DEST/share/themes/" && \
    mkdir -p "$DEST/share/gtk-3.0" && \
    cp -r /usr/share/gtk-3.0/* "$DEST/share/gtk-3.0/" && \
    mkdir -p "$DEST/share/glib-2.0/schemas" && \
    cp -r /usr/share/glib-2.0/schemas/* "$DEST/share/glib-2.0/schemas/" && \
# Remove apt list
RUN rm -rf /var/lib/apt/lists/*

WORKDIR /workspace

COPY . .

RUN chmod +x scripts/*.sh

VOLUME ["/workspace/build", "/workspace/build-release", "/workspace/build-windows", "/workspace/build-windows-release"]

CMD ["/bin/bash"]