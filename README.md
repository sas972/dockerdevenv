# Boost.Asio TCP Server in Docker + VS Code Server

This project is an asynchronous C++ TCP echo server example built with **Boost.Asio**, running inside a containerized **VS Code Server** environment.

The entire C++ toolchain, Boost libraries, and development tools reside inside the Docker container, allowing you to develop seamlessly through VS Code.

---

## File Structure

- [`CMakeLists.txt`](file:///Users/sas/projects/cpp/indockerexample/CMakeLists.txt) – CMake configuration (C++20, Boost and pthreads linking).
- [`src/main.cpp`](file:///Users/sas/projects/cpp/indockerexample/src/main.cpp) – Asynchronous Boost.Asio TCP Echo server.
- [`Dockerfile`](file:///Users/sas/projects/cpp/indockerexample/Dockerfile) – Lightweight Debian Bookworm Slim container with C++ toolchain, Boost, and `code-server`.
- [`docker-compose.yml`](file:///Users/sas/projects/cpp/indockerexample/docker-compose.yml) – Container services and port mappings (`8443`, `8080`).
- [`.devcontainer/devcontainer.json`](file:///Users/sas/projects/cpp/indockerexample/.devcontainer/devcontainer.json) – Configuration for the native desktop VS Code Dev Containers extension.
- [`README_HU.md`](file:///Users/sas/projects/cpp/indockerexample/README_HU.md) – Hungarian version of this documentation.

---

## Workflow 1: VS Code in the Browser (code-server)

The fastest method: access VS Code running inside the container directly from your web browser.

### 1. Start the container:
```bash
docker-compose up -d vscode
```

### 2. Open in your browser:
Navigate to: **[http://localhost:8443](http://localhost:8443)**

A full-featured VS Code interface will open in your browser with the `/workspace` folder preloaded!

### 3. Build and run inside the VS Code integrated terminal:
Open a terminal in the browser VS Code (`Ctrl + \`` or `Terminal -> New Terminal`):

```bash
# 1. Generate CMake configuration (with Ninja generator)
cmake -B build -S . -G Ninja

# 2. Compile the project
cmake --build build

# 3. Start the server on port 8080
./build/server 8080
```

---

## Workflow 2: Desktop VS Code (Dev Containers)

If you prefer using the desktop VS Code application on your host machine while running the **VS Code Server** inside the container:

1. Install the **Dev Containers** extension (`ms-vscode-remote.remote-containers`) in your local VS Code.
2. Open this folder in your local VS Code:
   ```bash
   code .
   ```
3. A notification will appear in the bottom-right corner:
   > *"Folder contains a Dev Container configuration file. Reopen in Container"*
   Click **Reopen in Container** (or press `F1` -> `Dev Containers: Reopen in Container`).
4. VS Code will spin up the container, connect to the internal VS Code Server, and automatically configure C++ and CMake extensions.

---

## Testing the Server from the Host Machine

The TCP server inside the container listens on port `8080`, which is forwarded to your host machine. Open a terminal on your host (Mac):

```bash
nc localhost 8080
```

When you type something and hit Enter, the server replies:
```text
Welcome to the Boost.Asio TCP Echo Server!
Type something and press Enter:
hello docker!
Echo: hello docker!
```

---

## Stopping the Container

When you are done:
```bash
docker-compose down
```
*(Your custom VS Code settings and installed extensions are preserved in the `vscode-data` volume.)*
