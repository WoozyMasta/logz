#!/usr/bin/env bash
set -euo pipefail

: "${PROJECT_DIR:=${1:-$PWD}}"
cd "$PROJECT_DIR"

out=CONFIG.md
config_file=./scripts/3_Game/LogZ/Config/DTO.c
events_file=./scripts/3_Game/LogZ/Logger/Event.c

# Static template files
start_md=tools/config_start.md
events_info_md=tools/events_info.md

{
  cat "$start_md" >"$out"
  printf '\n## Options [%s](%s)\n' "${config_file##*LogZ/}" "$config_file"
  awk -f tools/config_extract.awk "$config_file"

  printf '\n'
  cat "$events_info_md"
  printf '\n'

  awk -f tools/events_extract.awk "$events_file"
} >>"$out"

echo "Generate $out done"
