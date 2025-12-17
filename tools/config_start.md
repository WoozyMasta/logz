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
