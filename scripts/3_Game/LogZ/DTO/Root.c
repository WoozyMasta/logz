/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright (c) 2025 WoozyMasta
    Source: https://github.com/woozymasta/logz
*/

#ifdef SERVER
/**
    \brief Root log record envelope.
*/
class LogZ_DTO_Root
{
	int ts; // UTC time (epoch seconds)
	int uptime_ms; // server uptime (ms)
	int world_time; // game world time (epoch seconds)
	string instance; // instance id (or GamePort or QueryPort as fallback)
	string level; // LogZ_Levels level
	string schema; // LogZ schema version
	string world; // game world name
	string event_type; // LogZ_Events type
	string msg;

	/**
	    \brief Construct envelope from level, message and event type.
	*/
	void LogZ_DTO_Root(LogZ_Level lvl, string message, LogZ_Event eventType)
	{
		if (!LogZ_Config.IsLoaded())
			return;

		LogZ_ConfigDTO cfg = LogZ_Config.Get();
		if (!cfg)
			return;

		ts = LogZ_Time.EpochSecondsUTC();
		uptime_ms = g_Game.GetTime();
		world_time = LogZ_Time.GameEpochSeconds();
		instance = cfg.settings.instance_id_resolved;
		level = LogZ_Levels.ToString(lvl);
		schema = LogZ_Constants.SCHEMA_VERSION;
		world = cfg.geo.world_name;
		event_type = LogZ_Events.ToString(eventType);
		msg = message;
	}
}
#endif
