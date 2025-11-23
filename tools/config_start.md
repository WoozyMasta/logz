# LogZ Configuration

> Automatically generated configuration reference.

This document lists configuration options and CLI flags used
by the **LogZ** mod.  
You can configure the mod via `serverDZ.cfg`
or via command line parameters (launch parameters).

## Configuration Priority

The mod applies settings in the following order (highest priority first):

1. **CLI Flags**:
  Launch parameters (e.g., `-logz-level=debug`) override everything.
2. **Server Config**:
  Settings in `serverDZ.cfg` (e.g., `LogZ_Level = 1;`).
3. **Defaults**:
  Hardcoded fallback values if nothing is specified.

## Value Types

* **Boolean**:
  * Config: `1` (true) or `0` (false).
  * CLI: `true`, `1` (enable) or `false`, `0` (disable).
* **Enum**: Case-insensitive strings (e.g., `Trace`, `Info`)
  or their numeric equivalents.
