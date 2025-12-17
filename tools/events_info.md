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
