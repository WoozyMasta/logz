/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright (c) 2025 WoozyMasta
    Source: https://github.com/woozymasta/logz
*/

#ifdef SERVER
modded class TriggerEvents
{
	override protected void Enter(TriggerInsider insider)
	{
		super.Enter(insider);

		Object obj = insider.GetObject();
		if (!obj)
			return;

		if (obj.IsMan())
			LogZ_GameLogger.WithObjectAndOwner(
			    obj, this, string.Format("player enter trigger %1", ClassName()),
			    LogZ_Level.INFO, LogZ_Event.TRIGGER_IN);
		else
			LogZ_GameLogger.WithObjectAndOwner(
			    obj, this, string.Format("enter trigger %1", ClassName()),
			    LogZ_Level.DEBUG, LogZ_Event.TRIGGER_IN);
	}

	override protected void Leave(TriggerInsider insider)
	{
		super.Leave(insider);

		Object obj = insider.GetObject();
		if (!obj)
			return;

		if (obj.IsMan())
			LogZ_GameLogger.WithObjectAndOwner(
			    obj, this, string.Format("player leave trigger %1", ClassName()),
			    LogZ_Level.INFO, LogZ_Event.TRIGGER_OUT);
		else
			LogZ_GameLogger.WithObjectAndOwner(
			    obj, this, string.Format("leave trigger %1", ClassName()),
			    LogZ_Level.DEBUG, LogZ_Event.TRIGGER_OUT);
	}
}
#endif
