/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright (c) 2025 WoozyMasta
    Source: https://github.com/woozymasta/logz
*/

#ifdef SERVER
/**
    \brief Log severity levels in ascending order.
*/
enum LogZ_Level {
	TRACE, // 0
	DEBUG, // 1
	INFO, // 2
	WARN, // 3
	ERROR, // 4
	FATAL, // 5
	OFF // 6
}

/**
    \brief Mapping helpers between enum levels and strings.
*/
class LogZ_Levels
{
	/**
	    \brief Check if level passes current threshold.
	*/
	static bool IsEnabled(LogZ_Level lvl)
	{
		if (lvl < 0 || lvl >= LogZ_Level.OFF)
			return false;

		return (lvl >= LogZ_Config.s_Level);
	}

	/**
	    \brief Convert enum to lowercase string.
	    \return "trace|debug|info|warn|error|fatal|off"
	*/
	static string ToString(LogZ_Level lvl)
	{
		if (lvl < LogZ_Level.TRACE || lvl >= LogZ_Level.OFF)
			return "off";

		string logLevel = EnumTools.EnumToString(LogZ_Level, lvl);
		logLevel.ToLower();

		return logLevel;
	}

	/**
	    \brief Parse level from string or integer literal.
	    \details Accepts numbers or first-letter fast path for names.
	    \return Parsed level or INFO on invalid input.
	*/
	static LogZ_Level FromString(string lvl)
	{
		if (lvl == string.Empty)
			return LogZ_Level.INFO;

		lvl.ToLower();
		lvl.TrimInPlace();
		int ascii = lvl.Get(0).ToAscii();

		// numeric fast path
		if (ascii >= 48 && ascii <= 57) {
			int numLvl = lvl.ToInt();
			if (numLvl >= LogZ_Level.TRACE && numLvl <= LogZ_Level.OFF)
				return numLvl;

			return LogZ_Level.INFO;
		}

		switch (ascii) {
		case 116: // 't' -> "trace"
			return LogZ_Level.TRACE;

		case 100: // 'd' -> "debug"
			return LogZ_Level.DEBUG;

		case 105: // 'i' -> "info"
			return LogZ_Level.INFO;

		case 119: // 'w' -> "warn"
			return LogZ_Level.WARN;

		case 101: // 'e' -> "error"
			return LogZ_Level.ERROR;

		case 102: // 'f' -> "fatal"
			return LogZ_Level.FATAL;

		case 111: // 'o' -> "off"
			return LogZ_Level.OFF;
		}

		return LogZ_Level.INFO;
	}
}
#endif
