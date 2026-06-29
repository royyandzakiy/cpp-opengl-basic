# .devcontainer/Dockerfile
# Portable, reproducible Linux toolchain for people without a WSL/Linux box (or for CI parity).
# It runs scripts/setup-ubuntu.sh — the SAME script the Remote-WSL workflow uses — so the
# container and WSL toolchains never drift.
FROM mcr.microsoft.com/devcontainers/base:ubuntu-24.04

# Build runs as root; setup-ubuntu.sh detects that and installs Conan system-wide
# (/usr/local/bin) so the default 'vscode' user picks it up.
COPY scripts/setup-ubuntu.sh /tmp/setup-ubuntu.sh
RUN bash /tmp/setup-ubuntu.sh && rm -f /tmp/setup-ubuntu.sh
