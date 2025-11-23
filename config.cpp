/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright (c) 2025 WoozyMasta
    Source: https://github.com/woozymasta/logz
*/

class CfgMods
{
	class LogZ
	{
		type = "mod";
		dir = "logz";
		name = "NDJSON Structure Logger for DayZ";
		version = "0.1.0";
		credits = "WoozyMasta";
		author = "WoozyMasta";
		authorID = "76561198037610867";
		hideName = 1;
		hidePicture = 1;
		defines[] = {"LOGZ"};
		dependencies[] = {"game", "world", "mission"};

		class defs
		{
			class gameScriptModule
			{
				files[] = {"logz/scripts/3_game"};
			};

			class worldScriptModule
			{
				files[] = { "logz/scripts/4_world" };
			};

			class missionScriptModule
			{
				files[] = { "logz/scripts/5_mission" };
			};
		};
	};
};

class CfgPatches
{
	class LogZ
	{
		requiredAddons[] = {
			"DZ_Data",
			"DZ_Scripts",
		};
	};
};
