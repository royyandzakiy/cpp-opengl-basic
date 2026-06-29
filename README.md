# C++ Project Template

<!-- CI + coverage -->
[![CI](https://github.com/royyandzakiy/cpp-project-template/actions/workflows/build-and-test.yml/badge.svg)](https://github.com/royyandzakiy/cpp-project-template/actions/workflows/build-and-test.yml)
[![coverage](https://img.shields.io/endpoint?url=https://raw.githubusercontent.com/royyandzakiy/cpp-project-template/badges/coverage.json)](https://github.com/royyandzakiy/cpp-project-template/actions/workflows/build-and-test.yml)
![C++23](https://img.shields.io/badge/C%2B%2B-23-00599C?logo=cplusplus&logoColor=white)

[![Ubuntu](https://img.shields.io/endpoint?url=https://raw.githubusercontent.com/royyandzakiy/cpp-project-template/badges/os-linux.json)](https://github.com/royyandzakiy/cpp-project-template/actions/workflows/build-and-test.yml)
[![Clang](https://img.shields.io/endpoint?url=https://raw.githubusercontent.com/royyandzakiy/cpp-project-template/badges/clang.json)](https://github.com/royyandzakiy/cpp-project-template/actions/workflows/build-and-test.yml)
[![GCC](https://img.shields.io/endpoint?url=https://raw.githubusercontent.com/royyandzakiy/cpp-project-template/badges/gcc.json)](https://github.com/royyandzakiy/cpp-project-template/actions/workflows/build-and-test.yml)

[![Windows](https://img.shields.io/endpoint?url=https://raw.githubusercontent.com/royyandzakiy/cpp-project-template/badges/os-windows.json)](https://github.com/royyandzakiy/cpp-project-template/actions/workflows/build-and-test.yml)
[![MSVC](https://img.shields.io/endpoint?url=https://raw.githubusercontent.com/royyandzakiy/cpp-project-template/badges/msvc.json)](https://github.com/royyandzakiy/cpp-project-template/actions/workflows/build-and-test.yml)
[![Clang-CL](https://img.shields.io/endpoint?url=https://raw.githubusercontent.com/royyandzakiy/cpp-project-template/badges/clang-cl.json)](https://github.com/royyandzakiy/cpp-project-template/actions/workflows/build-and-test.yml)
[![MinGW](https://img.shields.io/endpoint?url=https://raw.githubusercontent.com/royyandzakiy/cpp-project-template/badges/mingw.json)](https://github.com/royyandzakiy/cpp-project-template/actions/workflows/build-and-test.yml)

[![macOS](https://img.shields.io/endpoint?url=https://raw.githubusercontent.com/royyandzakiy/cpp-project-template/badges/os-macos.json)](https://github.com/royyandzakiy/cpp-project-template/actions/workflows/build-and-test.yml)
[![AppleClang](https://img.shields.io/endpoint?url=https://raw.githubusercontent.com/royyandzakiy/cpp-project-template/badges/appleclang.json)](https://github.com/royyandzakiy/cpp-project-template/actions/workflows/build-and-test.yml)

<!-- Badges:
     • All compiler, OS, and coverage badges are DYNAMIC. Each CI leg stages a shields-endpoint
       JSON from build/<preset>/toolchain.txt (compiler id + version), /etc/os-release or
       Win32_OperatingSystem or sw_vers (OS version), and llvm-cov/gcovr (coverage). The
       publish-badges job commits them to the orphan `badges` branch on push to main/master;
       badges read them via shields.io endpoint, so versions track what actually built+passed
       and never drift.
     • To add a badge: have a CI leg stage another <name>.json artifact and add a matching
       endpoint badge to this README — no other plumbing needed.
     • C++23 is intentionally STATIC: it's a project invariant (CMAKE_CXX_STANDARD), not a
       CI-detected fact. -->


A cross-platform, **IDE- and OS-agnostic** C++23 project template. Language intelligence comes
from [clangd](https://clangd.llvm.org/), the build from CMake Presets, and dependencies from
vcpkg. Ships with sanitizers, static analysis, formatting, profiling hooks, example targets, and
unit tests.

Targets **Windows** (MSVC, Clang-CL, MinGW), **Linux** (GCC, Clang), and **macOS** (Apple Clang).
Requires CMake 3.28+.

> New to the project in your editor? See **[EDITOR_SETUP.md](EDITOR_SETUP.md)** for clangd setup
> and debugging in VS Code, CLion, Neovim, Emacs, Zed, Helix, and more.

---

## Prerequisites

- **CMake 3.28+** and **Ninja**
- A **C++23** compiler (one of the toolchains above)
- **vcpkg** — auto-detected from PATH and common install locations (see [vcpkg setup](#vcpkg-setup))
- **LLVM 21** (`clangd`, `clang-format`, `clang-tidy`) for editor intelligence and the
  format/lint targets (CI pins 21; other recent versions should work)

---

## Quick Start

```bash
# Configure (presets are filtered to your platform automatically)
cmake --preset clang-linux-debug    # Linux
cmake --preset clang-cl-debug       # Windows
cmake --preset appleclang-debug     # macOS

# Build (also mirrors compile_commands.json to the project root for clangd)
cmake --build --preset clang-linux-debug

# Run tests
ctest --preset test-clang-linux-debug
```

List everything available for your OS with `cmake --list-presets`.

---

## Dev Environment Setup

Three independent, non-conflicting ways to get the toolchain. Pick **one** based on how you work —
they don't interfere (VS Code only uses whichever remote you choose; each writes to its own
per-preset `build/<preset>/` dir; `.gitattributes` keeps the shell scripts LF so they survive a
Windows checkout):

| Tier | For | How |
|------|-----|-----|
| **Remote-WSL** *(recommended if you have WSL2)* | One shared Linux box for all your projects | `bash scripts/setup-ubuntu.sh` in your distro once, then **Reopen in WSL** in VS Code |
| **Dev Container** | No WSL/Linux box, or you want a reproducible/portable env | **Reopen in Container** (`.devcontainer/`) — builds the same toolchain via the *same* `setup-ubuntu.sh` |
| **Native Windows** | clang-cl / MSVC without WSL or Docker | `pwsh -File scripts/setup-windows.ps1` (winget) |

Notes:
- The Dev Container **and CI both reuse `scripts/setup-ubuntu.sh`** (the Dockerfile and the CI
  workflow each run it), and it **pins Clang 21** from apt.llvm.org — so WSL, container, and CI
  share one compiler *version*, not just the same package names. They can't drift apart.
- Add `--with-vcpkg` (Linux) / `-WithVcpkg` (Windows) to also install vcpkg; otherwise just Conan
  (the `PKG_MANAGER` default) is set up.
- Native Windows note: clang-cl **and** msvc presets still need the MSVC headers + Windows SDK from
  Visual Studio's "Desktop development with C++" workload.
- Already have a Linux toolbox? Use **Remote-WSL** and dismiss the container prompt — it's the
  lighter, shared-across-projects path.

---

## Project Structure

```
.
├── .clangd                       # clangd behavior (index, IncludeCleaner, inlay hints)
├── .clang-format                 # Formatting style (Microsoft base, tabs, 120 cols)
├── .clang-tidy                   # Static-analysis checks (warnings as errors)
├── .cmake-format.yaml            # CMake formatting style
├── .editorconfig                 # Cross-editor defaults (indent, EOL, charset)
├── .pre-commit-config.yaml       # Git pre-commit hooks (format + lint)
├── CMakeLists.txt                # Top-level build
├── CMakePresets.json             # Presets, platform-filtered (no setup needed)
├── CMakeUserPresets.json.example # Optional: custom preset overrides
├── project_options.local.cmake.example   # Optional: machine-local overrides (gitignored)
├── project_options.cmake         # Feature toggles (shared, committed)
├── vcpkg.json                    # vcpkg manifest (empty by default — add your own)
├── conanfile.txt                 # Conan manifest (fmt, scnlib, tracy)
├── version.txt                   # Project version (1.0.0)
├── EDITOR_SETUP.md               # Per-editor clangd setup
├── cmake/                        # CMake utility modules (internal)
│   ├── package_manager.cmake     #   dispatches to vcpkg / conan / none (PKG_MANAGER)
│   ├── config_vcpkg.cmake        #   vcpkg detection + toolchain wiring
│   ├── config_conan.cmake        #   Conan (cmake-conan) provider wiring
│   ├── compiler.cmake            #   warnings, strict mode, build-type guard
│   ├── sanitizer_analyzer.cmake  #   sanitizers, clang-tidy, ccache, fast linker
│   ├── coverage.cmake            #   coverage instrumentation + `coverage` target
│   ├── profiler.cmake            #   Tracy, Perfetto, ClangBuildAnalyzer
│   ├── format.cmake              #   format / format-check / tidy-all targets
│   ├── clangd.cmake              #   mirror compile_commands.json to project root
│   ├── target_options.cmake      #   configure_target(): PCH, unity, tidy, profiler links
│   └── version.cmake             #   generates the version header (when GENERATE_VERSION_HEADER)
├── include/cpp_project_template/
│   └── version.h                 # Auto-generated from version.txt (gitignored)
├── src/                          # Application sources (math, classify — a coverage demo)
├── test/                         # GoogleTest unit tests (+ sanitizer trip-wires)
├── examples/                     # Tracy / Perfetto demo targets
├── scripts/                      # setup-ubuntu.sh · setup-windows.ps1 · cov-to-md.py
└── .devcontainer/                # Portable container (runs setup-ubuntu.sh — same toolchain)
```

---

## Editor & clangd Setup

Language intelligence is editor-agnostic — the same four files drive every clangd-based editor:

| File | Purpose |
|------|---------|
| `.clangd` | Semantic behavior: background index, IncludeCleaner, inlay hints, clang-tidy integration |
| `.clang-tidy` | Which static-analysis checks run |
| `.clang-format` | Formatting style |
| `compile_commands.json` | Compilation database — mirrored to the project root on each build |

CMake exports `compile_commands.json` and the `mirror_compile_commands` target copies the active
preset's database to the project root, where `.clangd` (`CompilationDatabase: .`) picks it up.
Editor-specific launch flags (e.g. `--query-driver`) live per-editor — see
**[EDITOR_SETUP.md](EDITOR_SETUP.md)**.

---

## CMake Presets

Presets are defined in `CMakePresets.json` and **automatically filtered by platform** — only
presets relevant to your OS appear.

| Preset                                      | Platform | Compiler         |
| ------------------------------------------- | -------- | ---------------- |
| `msvc-debug` / `msvc-release`               | Windows  | MSVC (VS 2022)   |
| `clang-cl-debug` / `clang-cl-release`       | Windows  | Clang-CL + Ninja |
| `mingw-debug` / `mingw-release`             | Windows  | MinGW GCC        |
| `clang-linux-debug` / `clang-linux-release` | Linux    | Clang            |
| `gcc-linux-debug` / `gcc-linux-release`     | Linux    | GCC              |
| `appleclang-debug` / `appleclang-release`   | macOS    | Apple Clang      |

Each has matching `build` and `test` presets. The Linux/macOS **debug** presets also enable
`ENABLE_COVERAGE` (see [Coverage](#coverage)).

On top of those there are **sanitizer** presets — `clang-linux-asan` / `clang-linux-tsan` /
`clang-linux-msan` (Linux), `msvc-asan` / `clang-cl-asan` (Windows) — and a kitchen-sink
**`clang-linux-full`** that turns on ASan + strict + clang-tidy + PCH + unity + examples at once.

`CMakeUserPresets.json` is optional — copy `CMakeUserPresets.json.example` if you want custom names
or overrides.

---

## Feature Toggles (`project_options.cmake`)

| Option                      | Default | Description                                                  |
| --------------------------- | ------- | ------------------------------------------------------------ |
| `PKG_MANAGER`               | `vcpkg` | Dependency provider: `vcpkg` \| `conan` \| `none`            |
| `VCPKG_MANIFEST_MODE`       | ON      | Use `vcpkg.json` (manifest) vs. a global install             |
| `BUILD_TESTING`             | OFF     | Build the GoogleTest unit tests                              |
| `BUILD_EXAMPLES`            | OFF     | Build the `examples/` demo targets                           |
| `GENERATE_VERSION_HEADER`   | OFF     | Generate `include/<project>/version.h` from `version.txt`    |
| `ENABLE_STRICT_COMPILER`    | OFF     | Warnings as errors (`/WX`; `-Werror -pedantic`)              |
| `ENABLE_SANITIZERS`         | OFF     | UBSan baseline (`-fsanitize=undefined,bounds`)               |
| `ENABLE_ASAN`               | OFF     | AddressSanitizer + LeakSanitizer (needs `ENABLE_SANITIZERS`) |
| `ENABLE_TSAN`               | OFF     | ThreadSanitizer — data races (mutually exclusive with ASan/MSan) |
| `ENABLE_MSAN`               | OFF     | MemorySanitizer — uninit reads, Clang only (mutually exclusive) |
| `ENABLE_CLANG_TIDY`         | OFF     | clang-tidy on first-party code during the build (also live in clangd) |
| `ENABLE_COVERAGE`           | OFF     | Instrument + add the `coverage` target (see [Coverage](#coverage)) |
| `ENABLE_CCACHE`             | OFF     | ccache compiler launcher (no-op if ccache absent)            |
| `ENABLE_FAST_LINKER`        | OFF     | Use mold/lld if found (faster linking); no-op if absent / MSVC |
| `ENABLE_PCH`                | OFF     | Precompiled headers (faster full builds, slower incremental) |
| `ENABLE_UNITY_BUILD`        | OFF     | Unity/jumbo TUs (faster clean/CI builds; incremental-hostile) |
| `ENABLE_CLANG_BUILD_ANALYZER` | OFF   | `-ftime-trace` + ClangBuildAnalyzer target (Clang only)      |
| `ENABLE_TRACY`              | OFF     | Tracy debug profiler (FetchContent)                          |
| `ENABLE_PERFETTO`           | OFF     | Perfetto runtime tracing (FetchContent)                      |

The sanitizer/coverage/strict toggles are usually set for you by the matching presets; flip the
rest in `project_options.local.cmake` (copy `project_options.local.cmake.example`).

Pass at configure time or set permanently in `project_options.local.cmake`:

```bash
cmake --preset clang-linux-debug -DENABLE_ASAN=ON -DENABLE_CLANG_TIDY=ON
```

---

## Formatting, Linting & Static Analysis

Styles and checks are split by tool so each is shared and editor-agnostic:

- **`.clang-format`** — Microsoft base, tabs, 120-column limit, C++23.
- **`.clang-tidy`** — bugprone, modernize, performance, cppcoreguidelines; warnings as errors.
- **`.cmake-format.yaml`** — CMake formatting.
- **`.editorconfig`** — cross-editor indent/EOL/charset defaults (honored by VS Code, CLion, Visual Studio); matches the rules above.

Convenience build targets (from `cmake/format.cmake`):

```bash
cmake --build build/<preset> --target format        # rewrite sources in place
cmake --build build/<preset> --target format-check   # fail if unformatted (CI)
cmake --build build/<preset> --target tidy-all       # run clang-tidy over all TUs
```

Git hooks via [pre-commit](https://pre-commit.com/) (`.pre-commit-config.yaml`) run clang-format
and cmake-format on commit:

```bash
pip install pre-commit && pre-commit install
```

---

## Sanitizers & Profilers

Sanitizers are Clang/GCC oriented; on Windows use a `clang-*` or `mingw-*` preset
(MSVC supports ASan only). Wired by `cmake/sanitizer_analyzer.cmake`:

- **`ENABLE_SANITIZERS`** — UBSan baseline (`-fsanitize=undefined,bounds`)
- **`ENABLE_ASAN`** — AddressSanitizer + leak detection
- **`ENABLE_TSAN`** / **`ENABLE_MSAN`** — Thread or Memory sanitizer (mutually exclusive with ASan)

Use the sanitizer presets (`clang-linux-asan` / `-tsan` / `-msan`, `msvc-asan`, `clang-cl-asan`)
rather than setting these by hand. The sanitizers are verified by **`test/sanitizers/sanitizer_tests.cpp`** —
gtest death tests that fail if a sanitizer *doesn't* catch its bug class, so a silently-disabled
sanitizer turns CI red.

Profilers are wired by `cmake/profiler.cmake`: **Tracy** (`ENABLE_TRACY`) and **Perfetto**
(`ENABLE_PERFETTO`), each exposed as an interface target the main executable links when enabled.
The **`examples/`** directory has the runnable Tracy and Perfetto demos, built only when their
option is on.

---

## Coverage

`ENABLE_COVERAGE` instruments the build and adds a **`coverage`** target (wired by
`cmake/coverage.cmake`). The Linux/macOS **debug** presets enable it automatically:

```bash
cmake --build --preset clang-linux-debug --target coverage
```

This re-runs the unit tests under instrumentation and writes an HTML report to
`build/<preset>/coverage-html/`. Clang/AppleClang use LLVM source-based coverage
(`llvm-profdata` + `llvm-cov`); GCC uses `gcov` summarised by `gcovr`.

CI runs this on every push and, on `main`/`master`, publishes the shields.io endpoint behind the
**coverage badge** at the top of this README (to an orphan `badges` branch via
`scripts/cov-to-md.py`). It also renders a coverage table into the Actions run summary and uploads
the HTML report as a build artifact.

---

## Dependencies

Dependencies resolve through whichever `PKG_MANAGER` you pick (see below). The **Conan** manifest
(`conanfile.txt`) is the practical default:

| Package  | Source             | Use                                          |
| -------- | ------------------ | -------------------------------------------- |
| `fmt`    | `conanfile.txt`    | String formatting (the one actively used)    |
| `scnlib` | `conanfile.txt`    | Type-safe input parsing (available, unused)  |
| `tracy`  | `conanfile.txt`    | Profiling client                             |

`vcpkg.json` ships **empty** — add your packages there if you build with vcpkg.

**Fetched via `FetchContent`** (independent of the package manager):

| Package      | Use                                                        |
| ------------ | ---------------------------------------------------------- |
| `sml`        | `boost-ext/sml` state machine (header-only)                |
| `googletest` | Unit testing (in `test/`)                                  |
| `tracy` / `perfetto` | Profiler runtimes (when `ENABLE_TRACY` / `ENABLE_PERFETTO`) |

---

## Package Managers (vcpkg ↔ Conan)

The package manager is **orthogonal to the presets** — every preset works with either. Pick it
with the `PKG_MANAGER` knob (`vcpkg` default · `conan` · `none`), wherever is convenient:

```cmake
# project_options.local.cmake (gitignored)
set(PKG_MANAGER conan)
```

…or `-DPKG_MANAGER=conan`, or the IDE's CMake cache dropdown. vcpkg reads `vcpkg.json`; Conan reads
`conanfile.txt` (resolved via the cmake-conan provider, so `find_package()` is unchanged). For
clang-cl the matching `cmake/conan-profiles/clang-cl-windows-<buildtype>.ini` is selected
automatically; other compilers are auto-detected.

> Switching managers needs a **fresh configure** — the toolchain/provider locks in at first
> configure. Just configure into a new build directory (e.g. `cmake --preset clang-cl-debug -B build/conan`).

---

## vcpkg Setup

vcpkg is auto-detected in this order:

1. `VCPKG_ROOT_PATH` set in `project_options.local.cmake` or via `-D`
2. `VCPKG_ROOT` / `VCPKG_ROOT_PATH` environment variables
3. `vcpkg` executable found in PATH
4. Common default paths: `C:/vcpkg`, `/opt/vcpkg`, `~/vcpkg`, etc.
5. `./vcpkg` project subdirectory

For most setups nothing needs configuring. For a non-standard location, set `VCPKG_ROOT_PATH` in
`project_options.local.cmake` (copy `project_options.local.cmake.example`). Conan is available as an alternative
via `cmake/config_conan.cmake`.

---

## cmake/ Modules

- **`package_manager.cmake`** — `setup_package_manager()` dispatches to vcpkg / Conan / none per
  `PKG_MANAGER`.
- **`config_vcpkg.cmake`** — locates vcpkg, validates the toolchain, reports package status. Don't
  edit; configure via `project_options.local.cmake`.
- **`config_conan.cmake`** — Conan dependency flow via the cmake-conan provider.
- **`compiler.cmake`** — warning flags, strict mode, the no-build-type guard, debug postfix.
- **`sanitizer_analyzer.cmake`** — sanitizers, clang-tidy discovery, ccache, fast linker, per
  `project_options.cmake`.
- **`coverage.cmake`** — coverage instrumentation + `setup_coverage_target()` (the `coverage` target).
- **`profiler.cmake`** — Tracy, Perfetto, and ClangBuildAnalyzer integration.
- **`format.cmake`** — `format` / `format-check` / `tidy-all` targets.
- **`clangd.cmake`** — mirrors `compile_commands.json` to the project root for clangd.
- **`target_options.cmake`** — `configure_target()`: PCH, unity build, clang-tidy, profiler links,
  Windows ASan DLL deploy.
- **`version.cmake`** — when `GENERATE_VERSION_HEADER` is on, reads `version.txt` and generates
  `include/cpp_project_template/version.h` from `version.h.in`.

---

## Codebase Memory (Knowledge Graph)

**What it is:** a queryable knowledge graph of the codebase (symbols, calls, imports, structure)
stored in `.codebase-memory/`. It's **not used by the build** — it's developer/AI tooling.

**Workflow:** consumed by AI coding agents (e.g. Claude Code) over **MCP**
(Model Context Protocol). Instead of blindly grepping, the agent queries the graph — "who calls
this?", "where is X defined?", "show the architecture" — for faster, more accurate code
navigation and edits. The index is generated once and shared via git.

**To use it:** add the `codebase-memory` MCP server to your AI client (e.g. in Claude Code,
register it in your MCP config), then ask the agent to `index_repository` (or it bootstraps from
the committed `graph.db.zst`). No MCP client → these files are simply inert.

| File             | Purpose                                                                  |
| ---------------- | ------------------------------------------------------------------------ |
| `graph.db.zst`   | Compressed index snapshot — teammates bootstrap from it instead of re-indexing |
| `artifact.json`  | Index metadata (node/edge counts, timestamp, schema version)             |
| `.gitattributes` | Marks the artifact binary + `merge=ours` so it never conflicts           |

It is intentionally **committed** (the `.gitattributes` is auto-generated for exactly that). The
artifact is a point-in-time snapshot, so re-index before a release if you want it current — or
delete the directory and let each clone build its own index.

---

## Adapting the Template

1. Rename the project in `CMakeLists.txt` and `vcpkg.json` (the generated header path follows the
   project name).
2. Update `version.txt`.
3. Replace sources under `src/` and headers under `include/`.
4. Add dependencies to `vcpkg.json` and/or `conanfile.txt` (whichever `PKG_MANAGER` you use).
5. Adjust presets in `CMakePresets.json` as needed.
6. Replace this README using `README.md.template` as a starting point.
