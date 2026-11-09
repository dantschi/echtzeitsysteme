#!/usr/bin/env bash
# Source this file from Bash; paths are independent of the checkout location.
dhbw_repo_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
source "$dhbw_repo_dir/scripts/toolchain.env"
DHBW_TOOLCHAIN_DIR="${DHBW_TOOLCHAIN_DIR:-$dhbw_repo_dir/.tools}"
[[ "$DHBW_TOOLCHAIN_DIR" == /* ]] || DHBW_TOOLCHAIN_DIR="$PWD/$DHBW_TOOLCHAIN_DIR"
export DHBW_TOOLCHAIN_DIR
if [[ ! -x "$DHBW_TOOLCHAIN_DIR/quarto-$QUARTO_VERSION/bin/quarto" ]]; then
  echo 'Pinned Quarto is missing. Run bash scripts/setup.sh first.' >&2
  return 1
fi
export PATH="$DHBW_TOOLCHAIN_DIR/quarto-$QUARTO_VERSION/bin:$DHBW_TOOLCHAIN_DIR/node-$NODE_VERSION/bin:$DHBW_TOOLCHAIN_DIR/texlive-$TEXLIVE_VERSION/bin/x86_64-linux:$DHBW_TOOLCHAIN_DIR/rsvg-$RSVG_VERSION/usr/bin:$PATH"
export XDG_CACHE_HOME="$DHBW_TOOLCHAIN_DIR/cache"
export XDG_CONFIG_HOME="$DHBW_TOOLCHAIN_DIR/config"
export XDG_DATA_HOME="$DHBW_TOOLCHAIN_DIR/data"
export QUARTO_CHROMIUM="$DHBW_TOOLCHAIN_DIR/chrome-$CHROME_VERSION/chrome-headless-shell-linux64/chrome-headless-shell"
export QUARTO_TEXLIVE_BINPATH="$DHBW_TOOLCHAIN_DIR/texlive-$TEXLIVE_VERSION/bin/x86_64-linux"
export TEXMFVAR="$DHBW_TOOLCHAIN_DIR/texmf-var"
export TEXMFCONFIG="$DHBW_TOOLCHAIN_DIR/texmf-config"
export TEXMFHOME="$DHBW_TOOLCHAIN_DIR/texmf-home"
# Do not inherit fonts from a different TeX distribution (e.g. an older setup).
unset OPENTYPEFONTS
# Node's built-in fetch (used by livefigures) must honor the session's proxy.
# Inherited proxy variables, NO_PROXY and CA configuration remain intact.
export NODE_USE_ENV_PROXY=1
mkdir -p "$XDG_CACHE_HOME" "$XDG_CONFIG_HOME" "$XDG_DATA_HOME" "$TEXMFVAR" "$TEXMFCONFIG" "$TEXMFHOME"
unset dhbw_repo_dir
