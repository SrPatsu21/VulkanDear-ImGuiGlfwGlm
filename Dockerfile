FROM ubuntu:25.04

# Prevent apt from asking questions
ENV DEBIAN_FRONTEND=noninteractive

# Install essential build dependencies
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        build-essential \
        cmake \
        ninja-build \
        git \
        pkg-config \
        curl \
        wget \
        ca-certificates && \
    rm -rf /var/lib/apt/lists/*

# Define the working directory inside the container
WORKDIR /workspace

# Copy your project into the image
COPY . .

# Configure and build the project
# Output will go into /workspace/build
RUN cmake -B build -S . -G Ninja && \
    cmake --build build

# Optional: define a default output volume for build artifacts
# The host will mount a directory here to retrieve binaries
VOLUME ["/workspace/build"]

# Default command to enter an interactive shell
CMD ["/bin/bash"]
