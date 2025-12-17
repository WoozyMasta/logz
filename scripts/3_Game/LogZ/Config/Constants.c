/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright (c) 2025 WoozyMasta
    Source: https://github.com/woozymasta/logz
*/

#ifdef SERVER
/**
    \brief LogZ Constants
*/
class LogZ_Constants
{
	// Build information
	// Build information
	static const string NAME = "LogZ";
	static const string VERSION = "dev";
	static const string COMMIT_SHA = "fffffff";
	static const string BUILD_DATE = "1970-01-01T00:00:00+00:00";
	static const string SCHEMA_VERSION = "logz-v1beta"; // TODO upgrade on release

	// Working directory
	static const string WORK_DIR = "$profile:logz/";
	static const string CONFIG_FILE = WORK_DIR + "config.json";

	// Logs
	static const string LOGS_DIR = WORK_DIR + "logs/";
	static const string LOG_EXT = ".ndjson";

	// Legacy files support
	static const string LEGACY_LOG_FILE = "$profile:logz.ndjson";

	// Telemetry
	static const string TELEMETRY_URL = "https://zenit.woozymasta.ru";
	static const int TELEMETRY_DELAY = 600000; // 10-20 min
}
#endif
