/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright (c) 2025 WoozyMasta
    Source: https://github.com/woozymasta/logz
*/

#ifdef SERVER
modded class AreaDamageEvents
{
	override void OnEnterServerEvent(TriggerInsider insider)
	{
		super.OnEnterServerEvent(insider);

		Object obj = insider.GetObject();
		if (!obj)
			return;

		if (obj.IsMan())
			LogZ_GameLogger.WithObject(
			    obj, "player enter damage event area " + ClassName(),
			    LogZ_Level.INFO, LogZ_Event.EVENT_IN);
		else
			LogZ_GameLogger.WithObject(
			    obj, "enter damage event area " + ClassName(),
			    LogZ_Level.DEBUG, LogZ_Event.EVENT_IN);
	}

	override void OnLeaveServerEvent(TriggerInsider insider)
	{
		super.OnLeaveServerEvent(insider);

		Object obj = insider.GetObject();
		if (!obj)
			return;

		if (obj.IsMan())
			LogZ_GameLogger.WithObject(
			    obj, "player leave damage event " + ClassName(),
			    LogZ_Level.INFO, LogZ_Event.EVENT_OUT);
		else
			LogZ_GameLogger.WithObject(
			    obj, "leave damage event " + ClassName(),
			    LogZ_Level.DEBUG, LogZ_Event.EVENT_OUT);
	}
}
#endif
