FROM debian:bookworm-slim

# Prevent interactive prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Install only the necessary build tools and libraries:
# - g++ and make (minimal C++ compiler toolchain)
# - libboost-dev (NOT libboost-all-dev! Asio is header-only, saving ~1.5 GB of unused bindings)
RUN apt-get update && apt-get install -y --no-install-recommends \
    g++ \
    make \
    cmake \
    ninja-build \
    gdb \
    libboost-dev \
    netcat-openbsd \
    curl \
    git \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

# Install code-server (VS Code Server)
RUN curl -fsSL https://code-server.dev/install.sh | sh

# Set working directory inside the container
WORKDIR /workspace

# 8443: VS Code Server web interface port
# 8080: C++ TCP Echo server port
EXPOSE 8443 8080

# Default command: start VS Code Server without authentication on /workspace
CMD ["code-server", "--bind-addr", "0.0.0.0:8443", "--auth", "none", "/workspace"]
