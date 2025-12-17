# LogZ Configuration

> Automatically generated configuration reference.

Configuration is handled via a JSON file located at `$profile:logz/config.json`.
Changes require a server restart to take effect.

## Default Configuration

```json
{
  "version": "dev",
  "settings": {
    "instance_id": "",
    "level": "info",
    "events_mask": "ABCDEFGHIJKLMOPQSTUVWXYZ"
  },
  "file": {
    "file_name": "",
    "append": 0,
    "rotation_keep": 5,
    "buffer_size": 32,
    "flush_interval": 5
  },
  "filters": {
    "only_player_inventory_input": 1,
    "only_player_inventory_output": 0,
    "only_player_suicide": 1
  },
  "throttling": {
    "weapon_fire_ms": 250
  },
  "thresholds": {
    "hit_damage": 3.0,
    "hit_damage_vehicle": 15.0
  },
  "geo": {
    "world_effective_size": 0
  }
}
```

## Options [Config/DTO.c](./scripts/3_Game/LogZ/Config/DTO.c)

### Config

* **`version`** (`string`) = LogZ_Constants.VERSION -
  Internal configuration version. **Do not modify**.
* **`settings`** (`ref LogZ_ConfigDTO_Settings`) -
  General logging settings.
* **`file`** (`ref LogZ_ConfigDTO_File`) -
  File output and rotation settings.
* **`filters`** (`ref LogZ_ConfigDTO_Filters`) -
  Logic filters for specific events.
* **`throttling`** (`ref LogZ_ConfigDTO_Throttling`) -
  Throttling settings to prevent log spam.
* **`thresholds`** (`ref LogZ_ConfigDTO_Thresholds`) -
  Damage thresholds for hit events.
* **`geo`** (`ref LogZ_ConfigDTO_Geo`) -
  Geographic coordinate settings.

### Settings

* **`settings.instance_id`** (`string`) -
  Overrides the Instance ID. By default, this is detected automatically from
  serverDZ.cfg instanceID, or uses gamePort/steamQueryPort if instanceID is
  undefined or zero. Used for log file naming.
* **`settings.host_name`** (`string`) -
  Overrides the Host Name. By default, attempts to obtain it automatically.
  In some environments, reading the host name may be limited and
  unavailable. You can use this if you want to explicitly override the host
  name.
* **`settings.level`** (`string`) = "info" -
  Minimum severity level for logging. Values: `trace`, `debug`, `info`,
  `warn`, `error`, `fatal`, `off`. Default is `info`.
* **`settings.events_mask`** (`string`) = "ABCDEFGHIJKLMOPQSTUVWXYZ" -
  Event mask configuration. Can be a specific bitmask integer or a string of
  characters representing event categories. Example: "ABKqz" or "-1" (for
  all).
* **`settings.disable_telemetry`** (`bool`) -
  Disable send minimal telemetry 10-20 minutes after server startup.

### File

* **`file.file_name`** (`string`) -
  Log file name override (without extension). Default file path
  `$profile:logz/logs/logz_${instance_id}.ndjson`.
* **`file.append`** (`bool`) -
  Enables append mode. true - New logs are appended to the existing file.
  false - The log file is truncated (cleared) on server start.
* **`file.rotation_keep`** (`int`) = 5 -
  Number of rotated log files to keep. 0 or 1 - Disables rotation. > 1 -
  Keeps N rotated files (e.g., .0.ndjson, .1.ndjson).
* **`file.buffer_size`** (`int`) = 32 -
  Buffer size (in lines) for file writing. 0 - Disable buffer, write each
  line immediately (Direct mode). > 0 - Accumulate lines until this limit is
  reached.
* **`file.flush_interval`** (`int`) = 5 -
  Maximum time in seconds to hold lines in the buffer before flushing.
  Prevents logs from being stuck in memory if the server is quiet.

### Filters

* **`filters.only_player_inventory_input`** (`bool`) = true -
  Log `INVENTORY_IN` events only when the item's parent is a player. Reduces
  spam from item spawning on ground/zombies.
* **`filters.only_player_inventory_output`** (`bool`) -
  Log `INVENTORY_OUT` events only when the item's parent is a player.
* **`filters.only_player_suicide`** (`bool`) = true -
  Log suicide events (killer == victim) only for players. Filters out
  zombies/animals killing themselves via glitches.

### Throttling

* **`throttling.weapon_fire_ms`** (`int`) = 250 -
  Throttling window in milliseconds for Weapon `OnFire()` events. Prevents
  logging every single shot for high RPM weapons.

### Thresholds

* **`thresholds.hit_damage`** (`float`) = 3.0 -
  Minimum damage required to log hit events in `EEHitBy()`. Values of -1 or
  less disable this threshold.
* **`thresholds.hit_damage_vehicle`** (`float`) = 15.0 -
  Minimum damage from vehicles required to log hit events. Values of -1 or
  less disable this threshold.

### Geo

* **`geo.world_effective_size`** (`float`) -
  Overrides the effective map tile size in world units. Useful if the web
  map size is larger than the game world size. (For example, iZurvive tiles
  for Chernarus have a size of `15926`, although the world size is `15360`).

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
* **Example**: `settings.events_mask: "AFG"`
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
