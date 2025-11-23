#!/usr/bin/awk -f

function trim(s) {
  sub(/^[[:space:]]+/, "", s)
  sub(/[[:space:]]+$/, "", s)
  return s
}

BEGIN {
  LOG2 = log(2)

  print "| Letter | Flag (Bit) | Decimal | Event Name | Description |"
  print "| :---: | :---: | :---: | :--- | :--- |"
}

# Ищем строки вида: ИМЯ = ЦИФРЫ, // Комментарий
/^[[:space:]]*[A-Z0-9_]+[[:space:]]*=[[:space:]]*[0-9]+,/ {
  name = $1
  val_str = $3
  sub(",", "", val_str)
  val = val_str + 0

  if (val < 1)
    next

  bit_idx = int((log(val) / LOG2) + 0.5)

  if (bit_idx < 0 || bit_idx > 25)
    next

  char_code = 65 + bit_idx
  desc = ""
  if (index($0, "//") > 0) {
    split($0, parts, "//")
    desc = trim(parts[2])
  }

  printf "| **%c** | `1<<%d` | `%d` | `%s` | %s |\n", char_code, bit_idx, val, name, desc
}
