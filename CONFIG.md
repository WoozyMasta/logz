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

## Options [Utils/Config.c](./scripts/3_Game/LogZ/Utils/Config.c)

* **`LogZ_FileAppend`**
  `-logz-file-append`
  — Enable append mode for base log file. Config: non-zero enables, CLI:
 `true`, `1`, or empty enables.
* **`LogZ_FilesKeep`**
  `-logz-files-keep`
  (default: `1`)
  — Number of rotated log files to keep [0..100]; `1` or less disables
 rotation.
* **`LogZ_Level`**
  `-logz-level`
  (default: `LogZ_Level.INFO`)
  — Minimum severity for logging. Accepts enum number or:
 `trace|debug|info|warn|error|fatal|off` (can be one letter like `i` or
 `e`). Default is `INFO`.
* **`LogZ_EventsMask`**
  `-logz-events-mask`
  — Event bitmask for logging. Config: numeric int or -1 to disable all. CLI:
 numeric int or letters A-Z/a-z (e.g. `ABKqz`).
* **`LogZ_OnlyPlayerInventoryInput`**
  `-logz-only-player-inventory-input`
  — Log `INVENTORY_IN` only when item parent is player.
* **`LogZ_OnlyPlayerInventoryOutput`**
  `-logz-only-player-inventory-output`
  — Log `INVENTORY_OUT` only when item parent is player.
* **`LogZ_OnlyPlayerSuicide`**
  `-logz-only-player-suicide`
  — Log suicides (when killer is same object) only for players.
* **`LogZ_WeaponFireThrottlingMs`**
  `-logz-weapon-fire-throttling`
  (default: `250`)
  — Weapon `OnFire()` throttling window in milliseconds.
* **`LogZ_EntityHitDamageThreshold`**
  `-logz-entity-hit-damage-threshold`
  (default: `1`)
  — Minimum damage to log in `EEHitBy()`; -1 disables threshold.
* **`LogZ_EntityVehicleHitDamageThreshold`**
  `-logz-entity-vehicle-hit-damage-threshold`
  (default: `15`)
  — Minimum vehicle-hit damage to log in `EEHitBy()`; -1 disables threshold.

## Event Filters

Events allow you to precisely control what gets logged.
Since logging every hit or step can impact disk I/O on high-pop servers,
LogZ uses a **Bitmask** system.

You can configure the `EventsMask` in two ways:

### Using Letters

This is the most user-friendly method. Simply combine
the letters corresponding to the events you want.

* **Case insensitive**: `abc` is the same as `ABC`.
* **Format**: Just a string of letters.
* **Example**: `-logz-events-mask="AFG"`
  logs Server (A), Sessions (F), and Chat (G).

### Using Integers

You can sum the decimal values of the desired events.

* **Example**: To log `SYSTEM_GAME` (1) and `PLAYER_SESSION` (32),
  set value to `33`.
* **Disable All**: Set to `0`.
* **Enable All**: Set to `-1` or a very large number.

### Reference Table [Logger/Event.c](./scripts/3_Game/LogZ/Logger/Event.c)

Use the letters or decimal values from the table below
to build your configuration:

| Letter | Flag (Bit) | Decimal | Event Name | Description |
| :---: | :---: | :---: | :--- | :--- |
| **A** | `1<<0` | `1` | `SYSTEM_GAME` | 3_Game scope logs level |
| **B** | `1<<1` | `2` | `SYSTEM_WORLD` | 4_World scope logs level |
| **C** | `1<<2` | `4` | `SYSTEM_MISSION` | 5_Mission scope logs level |
| **D** | `1<<3` | `8` | `ADMIN_ACTIVITY` | Some admin activity (COT/VPP/commands) |
| **E** | `1<<4` | `16` | `PLAYER_ACTIVITY` | OnFire, unconscious, restrain, swim, fall, gesture, etc |
| **F** | `1<<5` | `32` | `PLAYER_SESSION` | Connect, disconnect, respawn, select character |
| **G** | `1<<6` | `64` | `PLAYER_CHAT` | Chat messages |
| **H** | `1<<7` | `128` | `PLAYER_HIT` | Player received damage |
| **I** | `1<<8` | `256` | `PLAYER_KILL` | Player died |
| **J** | `1<<9` | `512` | `CREATURE_HIT` | Infected, Animal and eAI received damage |
| **K** | `1<<10` | `1024` | `CREATURE_KILL` | Infected or Animal died |
| **L** | `1<<11` | `2048` | `TRANSPORT_HIT` | Vehicle received damage |
| **M** | `1<<12` | `4096` | `TRANSPORT_KILL` | Vehicle destroyed |
| **N** | `1<<13` | `8192` | `BUILDING_HIT` | Building or base object damaged |
| **O** | `1<<14` | `16384` | `BUILDING_KILL` | Building or base object destroyed |
| **P** | `1<<15` | `32768` | `ENTITY_HIT` | Generic item damaged |
| **Q** | `1<<16` | `65536` | `ENTITY_KILL` | Generic item destroyed |
| **R** | `1<<17` | `131072` | `ACTION_START` | Action started |
| **S** | `1<<18` | `262144` | `ACTION_END` | Action finished |
| **T** | `1<<19` | `524288` | `EVENT_IN` | Enter dynamic event (contamination area) |
| **U** | `1<<20` | `1048576` | `EVENT_OUT` | Leave dynamic event |
| **V** | `1<<21` | `2097152` | `TRIGGER_IN` | Enter script trigger (contamination area + other triggers) |
| **W** | `1<<22` | `4194304` | `TRIGGER_OUT` | Leave script trigger |
| **X** | `1<<23` | `8388608` | `INVENTORY_IN` | Item taken/attached/cargo-in |
| **Y** | `1<<24` | `16777216` | `INVENTORY_OUT` | Item dropped/detached/cargo-out |
| **Z** | `1<<25` | `33554432` | `EXPLOSIVE` | Explosive arming and detonation |
