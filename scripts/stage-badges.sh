#!/usr/bin/env bash
# scripts/stage-badges.sh — stage all badges for a CI leg
# Usage: bash scripts/stage-badges.sh <preset> <os> [coverage-lcov-path]
set -euo pipefail

PRESET="$1"
OS="${2:-linux}"
COVERAGE_LCOV="${3:-}"

mkdir -p badges-out

# Compiler badge
case "$OS" in
  linux)
    # Compiler badge is staged by the caller with a specific label
    ;;
  windows)
    case "$PRESET" in
      msvc-debug*)
        python3 scripts/make-badge.py --label "MSVC" --from-toolchain "build/$PRESET/toolchain.txt" > badges-out/msvc.json
        # OS badge
        os_name="$(powershell -NoProfile -Command '(Get-CimInstance Win32_OperatingSystem).Caption -replace "Microsoft ", ""')"
        python3 scripts/make-badge.py --label "tested on" --message "$os_name" --color 0078D6 --logo windows > badges-out/os-windows.json
        ;;
      clang-cl-debug*)
        python3 scripts/make-badge.py --label "Clang-CL" --from-toolchain "build/$PRESET/toolchain.txt" > badges-out/clang-cl.json
        ;;
      mingw-debug*)
        python3 scripts/make-badge.py --label "MinGW" --from-toolchain "build/$PRESET/toolchain.txt" > badges-out/mingw.json
        ;;
    esac
    ;;
  macos)
    python3 scripts/make-badge.py --from-toolchain "build/$PRESET/toolchain.txt" > badges-out/appleclang.json
    python3 scripts/make-badge.py --label "tested on" --message "macOS $(sw_vers -productVersion)" --color 999999 --logo apple > badges-out/os-macos.json
    ;;
esac

# Coverage badge (Linux only)
if [ -n "$COVERAGE_LCOV" ] && [ -f "$COVERAGE_LCOV" ]; then
  python3 scripts/cov-to-md.py --shields "$COVERAGE_LCOV" > badges-out/coverage.json
fi
