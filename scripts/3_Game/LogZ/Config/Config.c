/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright (c) 2025 WoozyMasta
    Source: https://github.com/woozymasta/logz
*/

#ifdef SERVER
/**
    \brief Configuration Manager for LogZ.
    \details Handles the lifecycle of the configuration: loading from JSON, version migration,
             creating default files, and initializing logging paths and rotation.
             Implements the Singleton pattern.
*/
class LogZ_Config
{
	protected static ref LogZ_Config s_Instance; // singleton instance
	protected static ref LogZ_ConfigDTO s_Config; // singleton state
	protected static bool s_Loaded;
	protected static bool s_TelemetrySend;

	/**
	    \brief Retrieves the singleton configuration instance.
	    \details Lazy-loads the configuration file on the first call.
	    \return Global LogZ_ConfigDTO instance.
	*/
	static LogZ_ConfigDTO Get()
	{
		if (!s_Instance)
			s_Instance = new LogZ_Config();

		if (!s_Config && !s_Loaded) {
			s_Config = new LogZ_ConfigDTO();
			s_Instance.Load();
		}

		return s_Config;
	}

	/**
	    \brief Drop instance and force reload on next call.
	*/
	static void Reset()
	{
		if (s_Config)
			s_Config = null;

		s_Loaded = false;
		ErrorEx("LogZ: configuration reset", ErrorExSeverity.INFO);
	}

	/**
	    \brief Checks if the configuration is successfully loaded.
	*/
	static bool IsLoaded()
	{
		return (s_Instance && s_Config && s_Loaded);
	}

	/**
	    \brief Orchestrates the loading process.
	*/
	protected void Load()
	{
		// Read or create/update JSON config
		LoadConfigFile();

		// Setup logs paths and rotation
		InitLogging();

		// Init geo cache (requires resolved world_effective_size)
		LogZ_Geo.Init();

		// Finalize loading (only now we are truly loaded)
		SetLoaded();
	}

	/**
	    \brief Handles JSON file operations (Load/Save/Upgrade).
	    \details If the file exists, it attempts to load and validate it.
	             If the version mismatches, it performs an upgrade and saves the file.
	             If the file is missing, it creates a new default configuration.
	*/
	protected void LoadConfigFile()
	{
		if (!FileExist(LogZ_Constants.WORK_DIR))
			MakeDirectory(LogZ_Constants.WORK_DIR);

		string cfgFile = LogZ_Constants.CONFIG_FILE;
		string error;

		if (FileExist(cfgFile)) {
			if (!JsonFileLoader<LogZ_ConfigDTO>.LoadFile(cfgFile, s_Config, error)) {
				ErrorEx("LogZ: JSON Load Error: " + error, ErrorExSeverity.ERROR);
				return;
			}

			s_Config.Normalize();

			// Version check & migration (if needed in future)
			if (s_Config.version != LogZ_Constants.VERSION) {
				s_Config.version = LogZ_Constants.VERSION;

				if (!JsonFileLoader<LogZ_ConfigDTO>.SaveFile(cfgFile, s_Config, error)) {
					ErrorEx("LogZ: JSON Save Error: " + error, ErrorExSeverity.ERROR);
					return;
				}

				ErrorEx("LogZ: saved upgraded config file: " + cfgFile, ErrorExSeverity.INFO);
			}

			return;
		}

		// Write initial config
		s_Config.version = LogZ_Constants.VERSION;
		// Set defaults for normalized fields
		s_Config.Normalize();

		if (!JsonFileLoader<LogZ_ConfigDTO>.SaveFile(cfgFile, s_Config, error)) {
			ErrorEx("LogZ: JSON Save Error: " + error, ErrorExSeverity.ERROR);
			return;
		}

		ErrorEx("LogZ: saved new config file: " + cfgFile, ErrorExSeverity.INFO);
	}

	/**
	    \brief Initializes log directories and handles file rotation.
	*/
	protected void InitLogging()
	{
		// Check for legacy single file support
		if (FileExist(LogZ_Constants.LEGACY_LOG_FILE)) {
			s_Config.file.full_path = LogZ_Constants.LEGACY_LOG_FILE;
			// Base path for rotation: $profile:logz.ndjson -> $profile:logz
			s_Config.file.base_path = "$profile:logz";

			string msg = "LogZ: Legacy log file detected!\n\n";
			msg += "========================================= LEGACY WARNING =========================================\n";
			msg += "| FILE:      '" + LogZ_Constants.LEGACY_LOG_FILE + "'\n";
			msg += "| REQUIRED:  Please delete this file to enable the new 'logs/' directory structure.\n";
			msg += "| NOTE:      You also need to update the path in the external log collector to the new destination.\n";
			msg += "| WARNING:   Support for legacy paths will be removed in future releases.\n";
			msg += "==================================================================================================\n\n";
			ErrorEx(msg, ErrorExSeverity.INFO);
		} else {
			// New directory structure
			string logsDir = LogZ_Constants.LOGS_DIR;
			if (!FileExist(logsDir))
				MakeDirectory(logsDir);

			string fileName = s_Config.file.file_name;
			if (fileName == string.Empty)
				fileName = "logz_" + s_Config.settings.instance_id;

			s_Config.file.base_path = logsDir + fileName;
			s_Config.file.full_path = s_Config.file.base_path + LogZ_Constants.LOG_EXT;
		}

		// Handle Rotation (works for both legacy and new structure)
		if (s_Config.file.rotation_keep > 1)
			RotateLogs();

		// Handle Truncate if not appending and rotation is off (or after rotation)
		if (!s_Config.file.append) {
			FileHandle fh = OpenFile(s_Config.file.full_path, FileMode.WRITE); // truncate
			if (fh)
				CloseFile(fh);
		}
	}

	/**
	    \brief Rotates log files shifting .N -> .N+1
	*/
	protected void RotateLogs()
	{
		string ext = LogZ_Constants.LOG_EXT;

		// delete last
		string last = s_Config.file.base_path + "." + (s_Config.file.rotation_keep - 1).ToString() + ext;
		if (FileExist(last))
			DeleteFile(last);

		// shift .N-1 -> .N, ..., .0 -> .1, base -> .0
		for (int i = s_Config.file.rotation_keep - 2; i >= 0; i--) {
			string srcLog = s_Config.file.full_path;
			if (i != 0)
				srcLog = s_Config.file.base_path + "." + i.ToString() + ext;

			string dstLog = s_Config.file.base_path + "." + (i + 1).ToString() + ext;

			if (FileExist(srcLog)) {
				CopyFile(srcLog, dstLog);
				DeleteFile(srcLog);
			}
		}
	}

	/**
	    \brief Finalizes the loading process.
	*/
	protected void SetLoaded()
	{
		if (s_Config)
			s_Config.Normalize();

		s_Loaded = true;

#ifdef DIAG
		DebugConfig();
#else
		if (!s_Config.settings.disable_telemetry && !s_TelemetrySend) {
			s_TelemetrySend = true;
			int delay = Math.RandomInt(LogZ_Constants.TELEMETRY_DELAY, LogZ_Constants.TELEMETRY_DELAY * 2);
			g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(SendTelemetry, delay, false);
		}
#endif

		string ver = string.Format(
		                 "%1 (%2) build %3, log_level=%4, events_mask=0x%5",
		                 LogZ_Constants.VERSION,
		                 LogZ_Constants.COMMIT_SHA,
		                 LogZ_Constants.BUILD_DATE,
		                 LogZ_Levels.ToString(s_Config.settings.level_enum),
		                 s_Config.settings.events_mask_int);

		ErrorEx("LogZ: loaded " + ver, ErrorExSeverity.INFO);
	}

	/**
	    \brief Telemetry sender
	*/
	protected void SendTelemetry()
	{
		RestApi api = GetRestApi();
		if (!api)
			api = CreateRestApi();
		if (!api) {
			s_TelemetrySend = false;
			return;
		}

		RestContext ctx = api.GetRestContext(LogZ_Constants.TELEMETRY_URL);
		if (!ctx) {
			s_TelemetrySend = false;
			return;
		}

		string body = string.Format(
		                  "{\"application\":\"%1\",\"version\":\"%2\",\"type\":\"steam\",\"port\":%3}",
		                  LogZ_Constants.NAME,
		                  LogZ_Constants.VERSION,
		                  g_Game.ServerConfigGetInt("steamQueryPort"));

		ctx.SetHeader("application/json");
		ctx.POST(null, "/api/telemetry", body);
	}

	/**
	    \brief Dumps the current configuration to the script log
	*/
	protected void DebugConfig()
	{
		string json;
		JsonSerializer().WriteToString(s_Config, true, json);
		ErrorEx("LogZ: configuration trace:\n" + json, ErrorExSeverity.INFO);
	}
}
#endif
