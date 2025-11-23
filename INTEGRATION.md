# Using LogZ in Your Own Mods

LogZ provides a robust API for structured logging. You can use it to log
events from your own mods into the shared NDJSON stream.

Dependency is optional: you should wrap calls in `#ifdef LOGZ` (assuming you
define this in your config) or check for class existence to avoid compile
errors if LogZ is not loaded.

## Compile-time guard

```cpp
#ifdef SERVER
#ifdef LOGZ
// Code using LogZ classes
#endif
#endif
```

## Basic Logging

The simplest way to log is using static level methods.

```cpp
// Simple message
LogZ.Info("Server started custom event");

// Message with specific Event Type (for filtering)
LogZ.Warn("High memory usage detected", LogZ_Event.SYSTEM_GAME);

// Available levels
LogZ.Trace("Detailed tracing");
LogZ.Debug("Debug info");
LogZ.Info("General info");
LogZ.Warn("Warning");
LogZ.Error("Error occurred");
LogZ.Fatal("Fatal error");
```

### Adding Context (Extra Fields)

You can pass a `map<string, string>` to add custom fields to the JSON
output.

> [!NOTE]  
> If a value string looks like a JSON object/array/vector/int/float, LogZ
> inserts it as raw JSON. Otherwise, it escapes and quotes it as a string.

```cpp
ref map<string, string> ctx = new map<string, string>();

// Simple string
ctx.Insert("mod_version", "1.0.42");

// Raw JSON (will be inserted as a nested object, not a string)
ctx.Insert("config", "{\"enabled\":true, \"rate\":5}"); 

LogZ.Info("Mod initialized", LogZ_Event.SYSTEM_MISSION, ctx);
```

**Output:**

```json
{..., "msg": "Mod initialized", "mod_version": "1.0.42", "config": {"enabled":true, "rate":5}}
```

## Advanced: Manual DTO Serialization

LogZ provides Data Transfer Objects (DTOs) to capture snapshots of game
entities (`LogZ_DTO_Man`, `LogZ_DTO_Transport`, `LogZ_DTO_Entity`, etc.).
You can use the internal serializer to generate JSON for them.

```cpp
PlayerBase player = ...;

ref map<string, string> data = new map<string, string>();
string json_player;

// 1. Create DTO
LogZ_DTO_Man playerDTO = new LogZ_DTO_Man(player);

// 2. Serialize to string
if (LogZ.GetSerializer().WriteToString(playerDTO, false, json_player))
  data.Insert("player", json_player);

LogZ.Info("Player processed", LogZ_Event.PLAYER_ACTIVITY, data);
```

## Helper: Auto-Serialization

Instead of manually choosing `LogZ_DTO_Man` vs `LogZ_DTO_Transport`, use
`LogZ_GameLogger.SerializeObject`. It automatically detects the object type
and uses the appropriate DTO.

### `SerializeObject`

```cpp
Object item = ...; // Can be a Gun, a Car, or a Zombie
string json;

// Automatically detects type and serializes fields (ID, Position, Health, etc.)
if (LogZ_GameLogger.SerializeObject(item, json))
{
  ref map<string, string> data = new map<string, string>();
  data.Insert("subject", json);

  // Optional: Serialize hierarchy root (e.g. if item is inside a backpack)
  string json_parent;
  if (LogZ_GameLogger.SerializeParentObject(item, json_parent))
    data.Insert("subject_parent", json_parent);

  LogZ.Info("Item handled", LogZ_Event.Entity_HIT, data);
}
```

## High-Level Abstractions

The `LogZ_GameLogger` class provides high-level wrappers to log common
scenarios with a single line of code.

### `WithObject`

Use this when an event revolves around a single entity.

```cpp
// Syntax: Object, Message, Level, EventType, [SlotName], [WithParent], [WithStats]
LogZ_GameLogger.WithObject(
    myPlayer,
    "Player performed custom action",
    LogZ_Level.INFO,
    LogZ_Event.PLAYER_ACTIVITY);
```

### `WithObjectAndOwner`

Use this for interactions between two objects (e.g., putting an item into a
container, entering a trigger, or attaching a part to a car).

```cpp
EntityAI item = ...;
EntityAI container = ...;

// Syntax: Subject, Owner/Target, Message, Level, EventType
LogZ_GameLogger.WithObjectAndOwner(
    item,
    container,
    "Item moved to custom container",
    LogZ_Level.INFO,
    LogZ_Event.INVENTORY_IN);
```

## References and Examples

For complex implementations (like combat logic or action state tracking),
refer to the internal LogZ implementations:

* **`LogZ_WorldLogger.WithKiller(...)`**: Shows how to handle Victim/Killer
  pairs and calculate distances.
* **`LogZ_WorldLogger.WithHit(...)`**: Demonstrates handling damage zones,
  ammo types, and thresholds.
* **`LogZ_WorldLogger.WithActionData(...)`**: Example of serializing a
  custom `ActionData` DTO alongside the player and target.

You can inspect `scripts/3_Game/LogZ/Logger/GameLogger.c` and
`scripts/4_World/LogZ/Logger/WorldLogger.c` for more patterns.
