/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright (c) 2025 WoozyMasta
    Source: https://github.com/woozymasta/logz
*/

#ifdef SERVER
modded class EffectArea
{
	override void OnPlayerEnterServer(PlayerBase player, EffectTrigger trigger)
	{
		super.OnPlayerEnterServer(player, trigger);

		if (player)
			LogZ_GameLogger.WithObjectAndOwner(
			    player, trigger,
			    string.Format("player enter effect area %1", ClassName()),
			    LogZ_Level.INFO, LogZ_Event.EVENT_IN);
	}

	override void OnPlayerExitServer(PlayerBase player, EffectTrigger trigger)
	{
		super.OnPlayerExitServer(player, trigger);

		if (player)
			LogZ_GameLogger.WithObjectAndOwner(
			    player, trigger,
			    string.Format("player leave effect area %1", ClassName()),
			    LogZ_Level.INFO, LogZ_Event.EVENT_OUT);
	}
}
#endif
