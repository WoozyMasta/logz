/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright (c) 2025 WoozyMasta
    Source: https://github.com/woozymasta/logz
*/

#ifdef SERVER
/**
    \brief Updates MetricZ counters on spawn/cleanup.
*/
modded class Weapon_Base
{
	int m_LogZ_LastFireTime;

	override void OnFire(int muzzle_index)
	{
		super.OnFire(muzzle_index);

		int time = g_Game.GetTime();
		if ((time - m_LogZ_LastFireTime) < LogZ_Config.s_WeaponFireThrottlingMs)
			return;

		m_LogZ_LastFireTime = time;

		LogZ_GameLogger.WithObject(
		    this, "weapon shoot",
		    LogZ_Level.DEBUG, LogZ_Event.PLAYER_ACTIVITY, "", true);
	}
}
#endif
