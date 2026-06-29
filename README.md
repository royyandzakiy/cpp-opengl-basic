# cpp-project-template (clangd · minimal)

A lean C++ prototyping base: CMake + vcpkg (manifest mode) + clangd. Built for fast iteration in
VS Code or CLion on Windows (clang-cl) and Linux/WSL2 (clang, gcc).

## Requirements

- CMake ≥ 3.28, Ninja
- A compiler: `clang-cl` (Windows) or `clang` / `gcc` (Linux/WSL2)
- [vcpkg](https://github.com/microsoft/vcpkg) — auto-detected via `VCPKG_ROOT` env var or common
  paths (`C:/vcpkg`, `~/vcpkg`, `/opt/vcpkg`). Override with `-DVCPKG_ROOT_PATH=...` or a
  `CMakeUserPresets.json` (see `CMakeUserPresets.json.example`).

## Build & run

```sh
cmake --preset clang-cl-debug          # or clang-linux-debug / gcc-linux-debug
cmake --build --preset clang-cl-debug
./bin/clang-cl/cpp_project_template     # bin/clang or bin/gcc on Linux
```

List presets with `cmake --list-presets`. Each toolchain has `-debug` / `-release` variants.

## Tests & examples (off by default)

```sh
cmake --preset clang-linux-debug -DBUILD_TESTING=ON -DBUILD_EXAMPLES=ON
cmake --build --preset clang-linux-debug
ctest --preset clang-linux-debug
```

Or set `BUILD_TESTING` / `BUILD_EXAMPLES` `ON` in a `local_options.cmake` to avoid passing `-D`.

## Layout

```
CMakeLists.txt        self-contained: vcpkg toolchain, warnings, deps, targets
CMakePresets.json     clang-cl (Win) + clang/gcc (Linux) × debug/release
vcpkg.json            dependencies (manifest mode)
cmake/clangd.cmake    mirrors compile_commands.json to repo root for clangd
cmake/format.cmake    `format` / `format-check` targets
src/                  main.cpp + math.{cpp,hpp}
test/                 GoogleTest (fetched), lean
examples/             one demo
```

## clangd

`.clangd` points at the repo-root `compile_commands.json`, which `cmake/clangd.cmake` mirrors from
the active preset's build dir on every build. So F12 / go-to-definition / system-library indexing
work as soon as you've configured + built once. Build dirs live under `build/<preset>/`.

## Notes

- `CMAKE_CXX_STANDARD` is 23. For reflection (gcc) or modules (clang) experiments, bump it to 26 in
  `CMakeLists.txt` or override per-preset.
- Warnings are light (`-Wall -Wextra` / `/W3`), no `-Werror`.
- Add a dependency: list it in `vcpkg.json`, then `find_package(... CONFIG REQUIRED)` +
  `target_link_libraries` in `CMakeLists.txt`.
