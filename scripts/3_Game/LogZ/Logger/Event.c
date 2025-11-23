/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright (c) 2025 WoozyMasta
    Source: https://github.com/woozymasta/logz
*/

#ifdef SERVER
/**
    \brief Event types bitmask (A..Z).
    \details
        Each enum value is a single bit (1 << N) and maps to a letter A..Z:
        Some people might find the event management format odd.
        Firstly, I was too lazy to create multiple keys in the config file,
        and secondly, it's just nostalgia for the way permissions were
        configured in MetaMod for Counter-Strike 1.6.
        If anyone wants to improve this, please send me a PR.
*/
enum LogZ_Event {
	NONE = 0, // Nothing

	SYSTEM_GAME = 1, // 3_Game scope logs level
	SYSTEM_WORLD = 2, // 4_World scope logs level
	SYSTEM_MISSION = 4, // 5_Mission scope logs level

	ADMIN_ACTIVITY = 8, // Some admin activity (COT/VPP/commands)

	PLAYER_ACTIVITY = 16, // OnFire, unconscious, restrain, swim, fall, gesture, etc
	PLAYER_SESSION = 32, // Connect, disconnect, respawn, select character
	PLAYER_CHAT = 64, // Chat messages

	PLAYER_HIT = 128, // Player received damage
	PLAYER_KILL = 256, // Player died

	CREATURE_HIT = 512, // Infected, Animal and eAI received damage
	CREATURE_KILL = 1024, // Infected or Animal died

	TRANSPORT_HIT = 2048, // Vehicle received damage
	TRANSPORT_KILL = 4096,// Vehicle destroyed

	BUILDING_HIT = 8192, // Building or base object damaged
	BUILDING_KILL = 16384,// Building or base object destroyed

	ENTITY_HIT = 32768, // Generic item damaged
	ENTITY_KILL = 65536, // Generic item destroyed

	ACTION_START = 131072, // Action started
	ACTION_END = 262144, // Action finished

	EVENT_IN = 524288, // Enter dynamic event (contamination area)
	EVENT_OUT = 1048576, // Leave dynamic event

	TRIGGER_IN = 2097152, // Enter script trigger (contamination area + other triggers)
	TRIGGER_OUT = 4194304, // Leave script trigger

	INVENTORY_IN = 8388608, // Item taken/attached/cargo-in
	INVENTORY_OUT = 16777216, // Item dropped/detached/cargo-out

	EXPLOSIVE = 33554432, // Explosive arming and detonation

	MAX = 67108863, // All combined (ABCDEFGHIJKLMNOPQRSTUVWXYZ)
}

/**
    \brief Event tools and classification helpers.
*/
class LogZ_Events
{
	// player related mask (session, activity, chat, hit, kill)
	static const int PLAYER_MASK = LogZ_Event.PLAYER_SESSION | LogZ_Event.PLAYER_ACTIVITY | LogZ_Event.PLAYER_CHAT | LogZ_Event.PLAYER_HIT | LogZ_Event.PLAYER_KILL;

	// hit-only mask (for any victim type)
	static const int HIT_MASK = LogZ_Event.PLAYER_HIT | LogZ_Event.CREATURE_HIT | LogZ_Event.TRANSPORT_HIT | LogZ_Event.BUILDING_HIT | LogZ_Event.ENTITY_HIT;

	// kill-only mask (for any victim type)
	static const int KILL_MASK = LogZ_Event.PLAYER_KILL | LogZ_Event.CREATURE_KILL | LogZ_Event.TRANSPORT_KILL | LogZ_Event.BUILDING_KILL | LogZ_Event.ENTITY_KILL;

	// damage mask (hit or kill)
	static const int DAMAGE_MASK = HIT_MASK | KILL_MASK;

	/**
	    \brief Check if event type is enabled by current config bitmask.
	*/
	static bool IsEnabled(LogZ_Event eventType)
	{
		if (eventType <= LogZ_Event.NONE || eventType >= LogZ_Event.MAX)
			return false;

		return (LogZ_Config.s_EventsMask & eventType) != 0;
	}

	/**
	    \brief Convert event type to a lowercase "namespace.name" string.
	*/
	static string ToString(LogZ_Event eventType)
	{
		if (eventType <= LogZ_Event.NONE || eventType >= LogZ_Event.MAX)
			return "none";

		string eventName = EnumTools.EnumToString(LogZ_Event, eventType);
		eventName.ToLower();
		eventName.Replace("_", ".");

		return eventName;
	}

	/**
	    \brief Build events bitmask from letters A-Z or a-z.
	    \return LogZ_Event Bit mask (0..MAX).
	*/
	static LogZ_Event FromString(string letters)
	{
		if (letters == string.Empty)
			return LogZ_Event.NONE;

		letters.ToLower();
		letters.TrimInPlace();

		LogZ_Event mask = 0;
		int len = letters.Length();

		for (int i = 0; i < len; i++) {
			int c = letters.Get(i).ToAscii();
			if (c < 97 || c > 122) // not 'a'..'z'
				continue;

			int bit = c - 97; // 'a' -> 0, 'b' -> 1, ..., 'z' -> 25
			if (bit < 0 || bit > 25)
				continue;

			mask |= (1 << bit);
		}

		return mask;
	}

	/**
	    \brief Check if event is player-related (session/activity/chat/hit/kill).
	*/
	static bool IsPlayer(LogZ_Event eventType)
	{
		return ((eventType & PLAYER_MASK) != 0);
	}

	/**
	    \brief Check if event is a hit (any victim type).
	*/
	static bool IsHit(LogZ_Event eventType)
	{
		return ((eventType & HIT_MASK) != 0);
	}

	/**
	    \brief Check if event is a kill (any victim type).
	*/
	static bool IsKill(LogZ_Event eventType)
	{
		return ((eventType & KILL_MASK) != 0);
	}

	/**
	    \brief Check if event is any damage event (hit or kill).
	*/
	static bool IsDamage(LogZ_Event eventType)
	{
		return ((eventType & DAMAGE_MASK) != 0);
	}
}
#endif
