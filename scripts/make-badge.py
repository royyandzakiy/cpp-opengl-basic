#!/usr/bin/env python3
"""Emit a shields.io endpoint JSON object (https://shields.io/badges/endpoint-badge).

Two modes:

  # Compiler badge from CMake's toolchain.txt ("<CMAKE_CXX_COMPILER_ID> <version>"):
  python3 scripts/make-badge.py --from-toolchain build/<preset>/toolchain.txt > clang.json

  # Generic badge (OS, fuzzing stats, anything):
  python3 scripts/make-badge.py --label "tested on" --message "Ubuntu 24.04" \
      --color E95420 --logo ubuntu > os.json

The output is a shields endpoint object; publish it to the `badges` branch and point a
`https://img.shields.io/endpoint?url=...` badge at it. This keeps badges DRY and lets new
metrics (Windows compilers, fuzz iterations, ...) reuse the exact same pipeline.
"""
import argparse
import json
import sys

# CMAKE_CXX_COMPILER_ID -> (badge label, shields namedLogo, color). namedLogo "" => no logo.
COMPILER_MAP = {
    "Clang": ("Clang", "llvm", "262D3A"),
    "GNU": ("GCC", "gnu", "262D3A"),
    "AppleClang": ("Apple Clang", "apple", "262D3A"),
    "MSVC": ("MSVC", "", "5C2D91"),
    "IntelLLVM": ("Intel", "intel", "0071C5"),
}


def from_toolchain(path: str):
    with open(path, encoding="utf-8") as handle:
        parts = handle.read().split()
    compiler_id = parts[0] if parts else "Unknown"
    version = parts[1] if len(parts) > 1 else "unknown"
    label, logo, color = COMPILER_MAP.get(compiler_id, (compiler_id, "", "blue"))
    return label, version, color, logo


def main() -> None:
    parser = argparse.ArgumentParser(description="Emit a shields.io endpoint badge JSON.")
    parser.add_argument("--from-toolchain", metavar="FILE",
                        help="read '<compiler-id> <version>' and map to a compiler badge")
    parser.add_argument("--label")
    parser.add_argument("--message")
    parser.add_argument("--color", default="blue")
    parser.add_argument("--logo", default="", help="shields namedLogo (e.g. ubuntu, llvm)")
    args = parser.parse_args()

    if args.from_toolchain:
        detected_label, message, color, logo = from_toolchain(args.from_toolchain)
        label = args.label if args.label else detected_label
    elif args.label and args.message:
        label, message, color, logo = args.label, args.message, args.color, args.logo
    else:
        parser.error("provide --from-toolchain, or both --label and --message")

    badge = {"schemaVersion": 1, "label": label, "message": message, "color": color}
    if logo:
        badge["namedLogo"] = logo
    json.dump(badge, sys.stdout)
    sys.stdout.write("\n")


if __name__ == "__main__":
    main()
