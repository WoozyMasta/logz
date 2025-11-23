/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright (c) 2025 WoozyMasta
    Source: https://github.com/woozymasta/logz
*/

#ifdef SERVER
modded class ExplosivesBase
{
	override protected void OnExplode()
	{
		LogZ_GameLogger.WithObject(
		    this, "explosive detonated",
		    LogZ_Level.WARN, LogZ_Event.EXPLOSIVE);

		super.OnExplode();
	}

	override protected void SetArmed(bool state)
	{
		super.SetArmed(state);

		if (m_LogZ_InitDone && GetLifetime() > 0.1) {
			if (state)
				LogZ_GameLogger.WithObject(
				    this, "explosive armed",
				    LogZ_Level.INFO, LogZ_Event.EXPLOSIVE);
			else
				LogZ_GameLogger.WithObject(
				    this, "explosive disarmed",
				    LogZ_Level.DEBUG, LogZ_Event.EXPLOSIVE);
		}
	}

	override protected void SetDefused(bool state)
	{
		super.SetDefused(state);

		if (state)
			LogZ_GameLogger.WithObject(
			    this, "explosive defused",
			    LogZ_Level.INFO, LogZ_Event.EXPLOSIVE);
	}
}
#endif
