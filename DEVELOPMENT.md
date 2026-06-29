# DEVELOPMENT

## Backlog

- revamp readme
  - create github wiki

- add cpack
- expand tests to have unit, integ, e2e (pytest)
- add doxygen with good ui

- examples
  - qt qml example
  - ftxui example
  - raylib example
  - crow example
  - emscripten example
  - cppwinrt scan
  - bluez scan

- continue create zephyr nrf
- recreate nxsdgldr

## Bugs

- clang-cl-asan fails to compile in CI
- selecting `conan` as default pkg manager fails to compile on some compilers (eg: `PKG_MANAGER: ${{ github.event.inputs.pkg-manager || 'conan' }}`)

## Skip

- set vcenv (needed or not?)
  - if msvc asan, set debugger target

- test on clion for all features
- test in vs for all features

- add gcovr.cfg
  
- fuzz example
- emscripten compiler for wasm (consider)

- if user selects compiler that they dont hv, give graceful fallback

- examples of bugs captured with sanitizers

- move cecep project to use clangd instead

- add license
- docs generator (doxygen but with most modern ui)
  - add doxyfile & doxygen
  - create wiki for this project (cleanup current readme, VERY bloated, move content to wiki)
- open for contributor: issues, discussions, contributions
  - fix readme: logo & badge, pitch (problem to solve), quick start (inline code example), live demo
  - add complete docs suite (tutorials, how-to-guides, understand, reference, release blog, release/change log)
  - add compiler explore quick test
