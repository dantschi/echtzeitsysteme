#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
source "$ROOT/scripts/activate.sh"
exec python3 "$ROOT/scripts/build-materials.py" labs "$@"
