#!/usr/bin/env bash
set -eu

cd /p/logz

./tools/validate.sh "$PWD"
./tools/astyle.sh "$PWD"
./tools/config.sh "$PWD"
./tools/ctags.sh "$PWD"

pbo='/d/SteamLibrary/steamapps/common/DayZ Tools/Bin/AddonBuilder/AddonBuilder.exe'
src='P:\logz'
mod='P:\@logz\addons'
include="$src"'\tools\pbo_include.txt'

"$pbo" "$src" "$mod" -clear -include="$include" -prefix="logz"
cp README.md /p/\@logz/
