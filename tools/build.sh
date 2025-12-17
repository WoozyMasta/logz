#!/usr/bin/env bash
set -eu

url() {
  printf '[InternetShortcut]\nURL=%s\n' "$1" >"/p/\@logz/$2.url"
}

cd /p/logz

./tools/validate.sh "$PWD"
./tools/update-contstants.sh "$PWD" true
./tools/astyle.sh "$PWD"
./tools/config.sh "$PWD"
./tools/metrics.sh "$PWD"
./tools/ctags.sh "$PWD"

pbo='/d/SteamLibrary/steamapps/common/DayZ Tools/Bin/AddonBuilder/AddonBuilder.exe'
src='P:\logz'
mod='P:\@logz\addons'
include="$src"'\tools\pbo_include.txt'

"$pbo" "$src" "$mod" -clear -include="$include" -prefix="logz"
cp README.md CONFIG.md /p/\@logz/

url 'https://steamcommunity.com/sharedfiles/filedetails/?id=3610709966' workshop
url 'https://github.com/WoozyMasta/logz' github

./tools/update-contstants.sh "$PWD" false
