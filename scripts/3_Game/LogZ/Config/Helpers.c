/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright (c) 2025 WoozyMasta
    Source: https://github.com/woozymasta/logz
*/

#ifdef SERVER
/**
    \brief Helpers for configuration
*/
class LogZ_Helpers
{
	/**
	    \brief Get instance ID if input empty string, use game config or port number
	*/
	static string GetInstanceID(string id)
	{
#ifdef DIAG
		ErrorEx("LogZ: requested instance ID with '" + id + "' input", ErrorExSeverity.WARNING);
#endif

		// get override from input
		if (id != string.Empty)
			return id;

		// get configured instance id in serverDZ.cfg
		id = g_Game.ServerConfigGetInt("instanceId").ToString();
		if (id != "0")
			return id;

		// use game port as instance id
		GetCLIParam("port", id);
		if (id != "0") {
			ErrorEx(
			    "LogZ: instanceId is 0. Used game port " + id + " as instanceId.",
			    ErrorExSeverity.INFO);

			return id;
		}

		// use steam query port as instance id
		id = g_Game.ServerConfigGetInt("steamQueryPort").ToString();
		if (id != "0") {
			ErrorEx(
			    "LogZ: instanceId and game port is 0. Used steam query port " + id + " as instanceId.",
			    ErrorExSeverity.INFO);

			return id;
		}

		// fallback to 0
		ErrorEx(
		    "LogZ: instanceId is 0. Set unique 'instanceId' in serverDZ.cfg or override 'instance_id' in logz.json",
		    ErrorExSeverity.WARNING);

		return "0";
	}
}
#endif
