/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright (c) 2025 WoozyMasta
    Source: https://github.com/woozymasta/logz
*/

#ifdef SERVER
class LogZ_Test
{
	static void Run()
	{
		LogZ_Level oldLevel = LogZ_Config.s_Level;
		int oldEvent = LogZ_Config.s_EventsMask;

		LogZ_Config.s_Level = LogZ_Level.TRACE;
		LogZ_Config.s_EventsMask = LogZ_Event.MAX;

		ref map<string, string> m = new map<string, string>();

		LogZ.Info("test none simple");
		LogZ.Info("test none", LogZ_Event.NONE);

		m.Insert("num_int", "42");
		m.Insert("num_float", "3.1415");
		m.Insert("num_exp", "-1.23e-4");
		m.Insert("num_plus", "+5"); // must be a string
		LogZ.Trace("test numbers", LogZ_Event.SYSTEM_GAME, m);

		m.Clear();
		vector pos = "1.01 0 -0.6";
		m.Insert("pos", pos.ToString());
		m.Insert("ori", "<1.01, 0, -0.6>");
		LogZ.Debug("test vector", LogZ_Event.SYSTEM_WORLD, m);

		m.Clear();
		m.Insert("obj", "{\"a\":1,\"b\":[2,3]}");
		LogZ.Info("test object", LogZ_Event.SYSTEM_MISSION, m);

		m.Clear();
		m.Insert("arr", "[10,20,30]");
		LogZ.Warn("test array", LogZ_Event.ADMIN_ACTIVITY, m);

		m.Clear();
		m.Insert("text", "hello \"world\" \\path\nnew\tline");
		LogZ.Error("test string", LogZ_Event.PLAYER_ACTIVITY, m);

		m.Clear();
		m.Insert("s", "ok");
		m.Insert("n", "123");
		m.Insert("v", "<0, 0, 1>");
		m.Insert("o", "{\"k\":\"v\"}");
		m.Insert("a", "[1,2,3]");
		LogZ.Fatal("test mixed", LogZ_Event.EXPLOSIVE, m);

		LogZ.Info("test wrong max event", LogZ_Event.MAX);
		LogZ.Info("test wrong event", LogZ_Event.MAX + 1);

		LogZ.Log("test off log", LogZ_Level.OFF, LogZ_Event.SYSTEM_GAME);
		LogZ.Log("test wrong log negative", -1, LogZ_Event.SYSTEM_GAME);
		LogZ.Log("test wrong log positive", 1024, LogZ_Event.SYSTEM_GAME);

		string megaLine;
		for (int i = 0; i < 100; i++) {
			megaLine += "=================================== ";
			megaLine += "----------------------------------- ";
			megaLine += "................................... ";
		}
		LogZ.Error(megaLine, LogZ_Event.SYSTEM_GAME);

		LogZ_Config.s_Level = oldLevel;
		LogZ_Config.s_EventsMask = oldEvent;
	}
}
#endif
