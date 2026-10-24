# Boost.Asio TCP Server Docker + VS Code Server Környezetben

Ez a projekt egy aszinkron C++ TCP echo szerver példa **Boost.Asio** segítségével, amely egy konténerizált **VS Code Server** környezetben fut.

Így a teljes C++ fordítási lánc, a Boost és a fejlesztői eszközök a Docker konténerben élnek, te pedig közvetlenül a VS Code-on keresztül dolgozhatsz benne.

---

## Fájlstruktúra

- [`CMakeLists.txt`](file:///Users/sas/projects/cpp/indockerexample/CMakeLists.txt) – CMake konfiguráció (C++20, Boost és pthreads linkelés).
- [`src/main.cpp`](file:///Users/sas/projects/cpp/indockerexample/src/main.cpp) – Aszinkron Boost.Asio TCP Echo szerver C++20 coroutine-okkal (`co_await`).
- [`Dockerfile`](file:///Users/sas/projects/cpp/indockerexample/Dockerfile) – Debian Bookworm Slim konténer C++ eszközökkel, Boosttal és a `code-server`-rel.
- [`docker-compose.yml`](file:///Users/sas/projects/cpp/indockerexample/docker-compose.yml) – Konténer szolgáltatások és port-továbbítás (`8443`, `8080`).
- [`.devcontainer/devcontainer.json`](file:///Users/sas/projects/cpp/indockerexample/.devcontainer/devcontainer.json) – Konfiguráció a natív asztali VS Code Dev Containers kiegészítőhöz.

---

## 1. Munkafolyamat: VS Code a Böngészőben (code-server)

A leggyorsabb módja, ha a konténerben futó VS Code-ot egyszerűen megnyitod a böngésződből:

### 1. Konténer indítása:
```bash
docker-compose up -d vscode
```

### 2. Nyisd meg a böngésződben:
Navigálj ide: **[http://localhost:8443](http://localhost:8443)**

Egy teljes értékű VS Code felület fogad a böngésződben, közvetlenül a `/workspace` mappával megnyitva!

### 3. Fordítás és futtatás a VS Code integrált termináljában:
Nyiss egy terminált a böngészős VS Code-ban (`Ctrl + \`` vagy `Terminal -> New Terminal`):

```bash
# 1. CMake konfiguráció (Ninja build generátorral)
cmake -B build -S . -G Ninja

# 2. Fordítás
cmake --build build

# 3. Szerver futtatása a 8080-as porton
./build/server 8080
```

---

## 2. Munkafolyamat: Asztali VS Code (Dev Containers)

Ha az asztali VS Code alkalmazást szeretnéd használni a gépeden, de úgy, hogy a **VS Code Server** a konténerben fusson:

1. Telepítsd a helyi VS Code-ban a **Dev Containers** kiegészítőt (`ms-vscode-remote.remote-containers`).
2. Nyisd meg ezt a mappát a helyi VS Code-ban:
   ```bash
   code .
   ```
3. A VS Code jobb alsó sarkában felugrik egy ablak:
   > *"Folder contains a Dev Container configuration file. Reopen in Container"*
   Kattints a **Reopen in Container** gombra (vagy `F1` -> `Dev Containers: Reopen in Container`).
4. A VS Code elindítja a konténert, telepíti benne a VS Code Servert és a C++ / CMake kiegészítőket, és összekapcsolódik vele.

---

## Szerver tesztelése kívülről (a gazdagépről)

A szerver a konténerben a `8080`-as porton figyel, ami ki van vezetve a gépedre. Nyiss egy sima terminált a Mac-eden:

```bash
nc localhost 8080
```

Amikor beírsz valamit és Entert ütsz, a szerver visszaküldi:
```text
Üdv a Boost.Asio TCP Echo Szerveren!
Írj be valamit és nyomj Entert:
hello docker!
Echo: hello docker!
```

---

## Konténer leállítása

Ha végeztél:
```bash
docker-compose down
```
*(A telepített VS Code beállítások és bővítmények megmaradnak a `vscode-data` kötetben.)*
