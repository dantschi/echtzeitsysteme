#!/bin/bash
set -eu

ROOT="$(cd "$(dirname "$0")" && pwd)"
cd "$ROOT"

OUT_DIR="labs/_output"
mkdir -p "$OUT_DIR"

echo "Baue Dozenten-Versionen (mit Lösungen)..."
quarto render labs/ --to pdf --profile solution

for pdf in labs/*.pdf; do
  [ -f "$pdf" ] || continue
  base="$(basename "$pdf" .pdf)"
  case "$base" in
    *template*|*student_check*|*studierende*|*musterloesung*) continue ;;
  esac
  cp "$pdf" "$OUT_DIR/${base}-musterloesung.pdf"
done

echo "Baue Studierenden-Versionen (ohne Lösungen)..."
quarto render labs/ --to pdf

for pdf in labs/*.pdf; do
  [ -f "$pdf" ] || continue
  base="$(basename "$pdf" .pdf)"
  case "$base" in
    *template*|*student_check*|*studierende*|*musterloesung*) continue ;;
  esac
  # Kanonischer Name labs/<base>.pdf = Studierenden-Version (nicht überschreiben)
  cp "$pdf" "$OUT_DIR/${base}-studierende.pdf"
done

echo "Fertig. PDFs unter $OUT_DIR (labs/*.pdf = Studierende):"
ls -la "$OUT_DIR"
ls -la labs/*.pdf 2>/dev/null || true
