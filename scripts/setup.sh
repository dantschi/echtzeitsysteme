#!/usr/bin/env bash
set -euo pipefail
repo_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
source "$repo_dir/scripts/toolchain.env"
toolchain_dir="${DHBW_TOOLCHAIN_DIR:-$repo_dir/.tools}"
[[ "$toolchain_dir" == /* ]] || toolchain_dir="$PWD/$toolchain_dir"
export DHBW_TOOLCHAIN_DIR="$toolchain_dir"
mode="${1:-full}"
case "$mode" in full|--quarto-only|--system-deps) ;; *) echo 'Usage: bash scripts/setup.sh [--quarto-only|--system-deps]' >&2; exit 2 ;; esac
if [[ "$(uname -s)" != Linux || "$(uname -m)" != x86_64 ]]; then
  echo 'The pinned toolchain supports Linux x86_64 (Debian 13 / Ubuntu 24.04).' >&2
  exit 1
fi
if [[ "$mode" == --system-deps ]]; then
  command -v apt-get >/dev/null || { echo 'Install the documented host libraries on this distribution.' >&2; exit 1; }
  privilege=()
  if [[ "$(id -u)" != 0 ]]; then
    command -v sudo >/dev/null || { echo 'System dependencies require root or sudo; the toolchain itself installs without root.' >&2; exit 1; }
    privilege=(sudo -n)
  fi
  proxy_options=()
  [[ -z "${HTTPS_PROXY:-}" ]] || proxy_options+=(-o "Acquire::https::Proxy=$HTTPS_PROXY")
  [[ -z "${HTTP_PROXY:-}" ]] || proxy_options+=(-o "Acquire::http::Proxy=$HTTP_PROXY")
  "${privilege[@]}" apt-get "${proxy_options[@]}" update
  "${privilege[@]}" apt-get "${proxy_options[@]}" install -y --no-install-recommends \
    ca-certificates curl python3 xz-utils dpkg libnss3 libgbm1 libasound2t64 \
    libatk-bridge2.0-0t64 libcups2t64 libxcomposite1 libxdamage1 libxrandr2 \
    libcairo2 libcairo-gobject2 libglib2.0-0t64 libxml2 libpango-1.0-0 \
    libpangocairo-1.0-0 libdav1d7 libfontconfig1
  exit 0
fi
for prerequisite in curl python3 tar xz sha256sum; do
  command -v "$prerequisite" >/dev/null || { echo "Missing prerequisite: $prerequisite (see --system-deps)." >&2; exit 1; }
done
mkdir -p "$toolchain_dir/downloads"
download() {
  local url="$1" file="$2" digest="$3"
  if [[ ! -f "$file" ]]; then
    curl --fail --location --silent --show-error --retry 3 --connect-timeout 15 --max-time 600 "$url" -o "$file.partial"
    mv "$file.partial" "$file"
  fi
  printf '%s  %s\n' "$digest" "$file" | sha256sum --check --status || {
    echo "Checksum verification failed: $file. Remove this archive and retry; do not change the expected checksum." >&2
    exit 1
  }
}
install_tar() {
  local target="$1" archive="$2" digest="$3" url="$4"
  [[ ! -f "$target/.installed-sha256" ]] || [[ "$(cat "$target/.installed-sha256")" != "$digest" ]] || return 0
  [[ ! -e "$target" ]] || { echo "Unrecognized/incomplete installation: $target. Choose a fresh DHBW_TOOLCHAIN_DIR or inspect it first." >&2; exit 1; }
  download "$url" "$archive" "$digest"
  local stage
  stage="$(mktemp -d "$toolchain_dir/.install.XXXXXX")"
  tar -xf "$archive" --strip-components=1 -C "$stage"
  printf '%s\n' "$digest" > "$stage/.installed-sha256"
  mv "$stage" "$target"
}
install_tar "$toolchain_dir/quarto-$QUARTO_VERSION" "$toolchain_dir/downloads/quarto-$QUARTO_VERSION.tar.gz" "$QUARTO_SHA256" \
  "https://github.com/quarto-dev/quarto-cli/releases/download/v$QUARTO_VERSION/quarto-$QUARTO_VERSION-linux-amd64.tar.gz"
if [[ "$mode" == --quarto-only ]]; then
  "$toolchain_dir/quarto-$QUARTO_VERSION/bin/quarto" --version
  exit 0
fi
command -v dpkg-deb >/dev/null || { echo 'dpkg-deb is required (see --system-deps).' >&2; exit 1; }
install_tar "$toolchain_dir/node-$NODE_VERSION" "$toolchain_dir/downloads/node-$NODE_VERSION.tar.xz" "$NODE_SHA256" \
  "https://nodejs.org/dist/v$NODE_VERSION/node-v$NODE_VERSION-linux-x64.tar.xz"
install_tar "$toolchain_dir/texlive-$TEXLIVE_VERSION" "$toolchain_dir/downloads/tinytex-$TEXLIVE_VERSION.tar.xz" "$TEXLIVE_SHA256" \
  "https://github.com/rstudio/tinytex-releases/releases/download/v$TEXLIVE_VERSION/TinyTeX-linux-x86_64-v$TEXLIVE_VERSION.tar.xz"
chrome_dir="$toolchain_dir/chrome-$CHROME_VERSION"
if [[ ! -f "$chrome_dir/.installed-sha256" ]] || [[ "$(cat "$chrome_dir/.installed-sha256")" != "$CHROME_SHA256" ]]; then
  [[ ! -e "$chrome_dir" ]] || { echo "Unrecognized installation: $chrome_dir" >&2; exit 1; }
  archive="$toolchain_dir/downloads/chrome-$CHROME_VERSION.zip"
  download "https://storage.googleapis.com/chrome-for-testing-public/$CHROME_VERSION/linux64/chrome-headless-shell-linux64.zip" "$archive" "$CHROME_SHA256"
  stage="$(mktemp -d "$toolchain_dir/.chrome.XXXXXX")"
  python3 - "$archive" "$stage" <<'PY'
import os, sys, zipfile
from pathlib import Path
archive, target = sys.argv[1], Path(sys.argv[2])
with zipfile.ZipFile(archive) as z:
    for entry in z.infolist():
        path = (target / entry.filename).resolve()
        if not path.is_relative_to(target.resolve()):
            raise ValueError('Archive path escapes installation directory')
        z.extract(entry, target)
        mode = (entry.external_attr >> 16) & 0o777
        if mode:
            os.chmod(path, mode)
PY
  printf '%s\n' "$CHROME_SHA256" > "$stage/.installed-sha256"
  mv "$stage" "$chrome_dir"
fi
rsvg_dir="$toolchain_dir/rsvg-$RSVG_VERSION"
if [[ ! -f "$rsvg_dir/.installed-sha256" ]] || [[ "$(cat "$rsvg_dir/.installed-sha256")" != "$RSVG_SHA256" ]]; then
  [[ ! -e "$rsvg_dir" ]] || { echo "Unrecognized installation: $rsvg_dir" >&2; exit 1; }
  archive="$toolchain_dir/downloads/librsvg2-bin-$RSVG_VERSION.deb"
  download "https://deb.debian.org/debian/pool/main/libr/librsvg/librsvg2-bin_${RSVG_VERSION}_amd64.deb" "$archive" "$RSVG_SHA256"
  stage="$(mktemp -d "$toolchain_dir/.rsvg.XXXXXX")"
  dpkg-deb -x "$archive" "$stage"
  printf '%s\n' "$RSVG_SHA256" > "$stage/.installed-sha256"
  mv "$stage" "$rsvg_dir"
fi
source "$repo_dir/scripts/activate.sh"
quarto --version
node --version
"$QUARTO_CHROMIUM" --version
lualatex --version | head -1
rsvg-convert --version
for package in tcolorbox.sty sectsty.sty needspace.sty adjustbox.sty texgyreheros-regular.otf; do
  kpsewhich "$package" >/dev/null || { echo "Missing package in pinned TeX bundle: $package" >&2; exit 1; }
done
echo 'Toolchain ready. Run: source scripts/activate.sh'
