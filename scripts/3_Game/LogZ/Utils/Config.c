/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright (c) 2025 WoozyMasta
    Source: https://github.com/woozymasta/logz
*/

#ifdef SERVER
/**
    \brief Runtime and CLI configuration for LogZ.
*/
class LogZ_Config
{
	private static const string CFG_OPT_PREFIX = "LogZ_";
	private static const string CLI_FLAG_PREFIX = "logz-";
	private static const string FILE_BASE = "$profile:logz";
	private static const string EXTENSION = ".ndjson";

	static const string LOG_FILE = FILE_BASE + EXTENSION;
	static const string SCHEMA_VERSION = "logz-v1beta"; // TODO upgrade on release

	// Instance ID and World name for labels
	private static int s_InstanceID;
	private static string s_WorldName;

	// resolved settings
	static bool s_LogAppend; // false/0=flush, true/1=append
	static int s_FilesKeep; // files to keep, 0=off
	static LogZ_Level s_Level;
	static int s_EventsMask;
	static bool s_OnlyPlayerInventoryInput;
	static bool s_OnlyPlayerInventoryOutput;
	static bool s_OnlyPlayerSuicide;
	static int s_WeaponFireThrottlingMs;
	static int s_EntityHitDamageThreshold;
	static int s_EntityVehicleHitDamageThreshold;

	/**
	    \brief Load and apply configuration overrides.
	*/
	static void Load()
	{
		// Enable append mode for base log file.
		// Config: non-zero enables,
		// CLI: `true`, `1`, or empty enables.
		s_LogAppend = Toggle("FileAppend", "file-append");

		// Number of rotated log files to keep [0..100];
		// `1` or less disables rotation.
		s_FilesKeep = IntOpt("FilesKeep", "files-keep", 1, 0, 100);

		// Minimum severity for logging.
		// Accepts enum number or: `trace|debug|info|warn|error|fatal|off`
		// (can be one letter like `i` or `e`).
		// Default is `INFO`.
		s_Level = LevelOpt("Level", "level", LogZ_Level.INFO);

		// Event bitmask for logging.
		// Config: numeric int or -1 to disable all.
		// CLI: numeric int or letters A-Z/a-z (e.g. `ABKqz`).
		s_EventsMask = EventsMaskOpt("EventsMask", "events-mask");

		// Log `INVENTORY_IN` only when item parent is player.
		s_OnlyPlayerInventoryInput = Toggle("OnlyPlayerInventoryInput", "only-player-inventory-input");

		// Log `INVENTORY_OUT` only when item parent is player.
		s_OnlyPlayerInventoryOutput = Toggle("OnlyPlayerInventoryOutput", "only-player-inventory-output");

		// Log suicides (when killer is same object) only for players.
		s_OnlyPlayerSuicide = Toggle("OnlyPlayerSuicide", "only-player-suicide");

		// Weapon `OnFire()` throttling window in milliseconds.
		s_WeaponFireThrottlingMs = IntOpt("WeaponFireThrottlingMs", "weapon-fire-throttling", 250, 50, 5000);

		// Minimum damage to log in `EEHitBy()`;
		// -1 disables threshold.
		s_EntityHitDamageThreshold = IntOpt("EntityHitDamageThreshold", "entity-hit-damage-threshold", 1, -1, 100);

		// Minimum vehicle-hit damage to log in `EEHitBy()`;
		// -1 disables threshold.
		s_EntityVehicleHitDamageThreshold = IntOpt("EntityVehicleHitDamageThreshold", "entity-vehicle-hit-damage-threshold", 15, -1, 100);


		if (s_FilesKeep > 0)
			RotateLogs();

		if (s_FilesKeep > 0 || !s_LogAppend) {
			FileHandle fh = OpenFile(LOG_FILE, FileMode.WRITE); // truncate
			if (fh)
				CloseFile(fh);
		}

		MakeWorldName();
		MakeInstanceID();
	}

	/**
	    \brief Get cached instance id.
	*/
	static int GetInstanceID()
	{
		return s_InstanceID;
	}

	/**
	    \brief Get cached world name.
	*/
	static string GetWorldName()
	{
		return s_WorldName;
	}

	/**
	    \brief Initialize cached instance id from server config/CLI.
	*/
	private static void MakeInstanceID()
	{
		s_InstanceID = g_Game.ServerConfigGetInt("instanceId");
		if (s_InstanceID != 0)
			return;

		string gamePort;
		GetCLIParam("port", gamePort);
		if (gamePort.ToInt() != 0) {
			s_InstanceID = gamePort.ToInt();
			ErrorEx(
			    "LogZ instanceId is 0. Used game port " + s_InstanceID + " as instanceId.",
			    ErrorExSeverity.INFO);
			return;
		}

		s_InstanceID = g_Game.ServerConfigGetInt("steamQueryPort");
		if (s_InstanceID != 0) {
			ErrorEx(
			    "LogZ instanceId and game port is 0. Used steam query port " + s_InstanceID + " as instanceId.",
			    ErrorExSeverity.INFO);
			return;
		}

		ErrorEx(
		    "LogZ instanceId is 0. Set unique 'instanceId' in server.cfg!",
		    ErrorExSeverity.WARNING);
	}

	/**
	    \brief Initialize cached world name from game.
	*/
	private static void MakeWorldName()
	{
		g_Game.GetWorldName(s_WorldName);
		s_WorldName.TrimInPlace();
		s_WorldName.ToLower();
	}

	/**
	    \brief Read bounded integer option from config and CLI.
	    \param cfgKey  Key suffix in serverDZ.cfg without "LogZ_".
	    \param cliFlag CLI flag suffix without "logz-".
	    \param defVal  Default value when not provided.
	    \param minVal  Minimum allowed value.
	    \param maxVal  Maximum allowed value.
	    \return int Value clamped to [minVal, maxVal].
	*/
	private static int IntOpt(string cfgKey, string cliFlag, int defVal, int minVal, int maxVal)
	{
		int value = g_Game.ServerConfigGetInt(CFG_OPT_PREFIX + cfgKey);
		if (value == 0)
			value = defVal;

		string cliVal;
		if (GetCLIParam(CLI_FLAG_PREFIX + cliFlag, cliVal))
			value = cliVal.ToInt();

		if (value < minVal)
			value = minVal;
		else if (value > maxVal)
			value = maxVal;

		return value;
	}

	/**
	    \brief Read log level from config and CLI.
	*/
	private static LogZ_Level LevelOpt(string cfgKey, string cliFlag, LogZ_Level defLvl)
	{
		LogZ_Level lvl = defLvl;

		int cfgInt = g_Game.ServerConfigGetInt(CFG_OPT_PREFIX + cfgKey);
		if (cfgInt >= LogZ_Level.TRACE && cfgInt <= LogZ_Level.OFF)
			lvl = cfgInt;

		string cliVal;
		if (GetCLIParam(CLI_FLAG_PREFIX + cliFlag, cliVal))
			lvl = LogZ_Levels.FromString(cliVal);

		return lvl;
	}

	/**
	    \brief Read boolean toggle from config and CLI.
	*/
	private static bool Toggle(string cfgKey, string cliFlag)
	{
		int configVal = g_Game.ServerConfigGetInt(CFG_OPT_PREFIX + cfgKey);
		bool result = (configVal != 0);

		string cliVal;
		if (GetCLIParam(CLI_FLAG_PREFIX + cliFlag, cliVal))
			result = (cliVal == "true" || cliVal == "1" || cliVal == string.Empty);

		return result;
	}

	/**
	    \brief Read events mask from config and CLI.
	*/
	private static int EventsMaskOpt(string cfgKey, string cliFlag)
	{
		// base from config (numeric only)
		int value = g_Game.ServerConfigGetInt(CFG_OPT_PREFIX + cfgKey);
		if (value == 0)
			value = LogZ_Event.MAX;
		else if (value <= 0)
			value = LogZ_Event.NONE;

		// CLI override (string)
		string cliVal;
		if (GetCLIParam(CLI_FLAG_PREFIX + cliFlag, cliVal)) {
			if (LogZ_Utils.HasAlpha(cliVal))
				value = LogZ_Events.FromString(cliVal);
			else
				value = cliVal.ToInt();
		}

		// clamp
		if (value <= LogZ_Event.NONE)
			value = LogZ_Event.NONE;
		else if (value >= LogZ_Event.MAX)
			value = LogZ_Event.MAX;

		return value;
	}

	/**
	    \brief Rotate log files.
	*/
	private static void RotateLogs()
	{
		if (s_FilesKeep <= 0)
			return;

		// delete last
		string last = FILE_BASE + "." + (s_FilesKeep - 1).ToString() + EXTENSION;
		if (FileExist(last))
			DeleteFile(last);

		// shift .N-1 -> .N, ..., .0 -> .1, base -> .0
		for (int i = s_FilesKeep - 2; i >= 0; i--) {
			string srcLog = LOG_FILE;
			if (i != 0)
				srcLog = FILE_BASE + "." + i.ToString() + EXTENSION;

			string dstLog = FILE_BASE + "." + (i + 1).ToString() + EXTENSION;
			if (FileExist(srcLog)) {
				CopyFile(srcLog, dstLog);
				DeleteFile(srcLog);
			}
		}
	}
}
#endif
