/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright (c) 2025 WoozyMasta
    Source: https://github.com/woozymasta/logz
*/

#ifdef SERVER
modded class ZombieBase
{
	protected bool m_LogZ_InitDone;
	protected bool m_LogZ_Killed;

	bool LogZ_ShouldLogged(bool input)
	{
		if (!m_LogZ_InitDone)
			return false;

		if (input)
			return !LogZ_Config.s_OnlyPlayerInventoryInput;

		return !LogZ_Config.s_OnlyPlayerInventoryOutput;
	}

	bool LogZ_IsAlreadyKilled()
	{
		return m_LogZ_Killed;
	}

	// * --- create ---
	override void EEOnCECreate()
	{
		super.EEOnCECreate();

		m_LogZ_InitDone = true;
		LogZ_GameLogger.WithObject(
		    this, "zombie created from CE",
		    LogZ_Level.DEBUG, LogZ_Event.SYSTEM_WORLD);
	}

	// * --- load ---
	override bool OnStoreLoad(ParamsReadContext ctx, int version)
	{
		if (!super.OnStoreLoad(ctx, version))
			return false;

		m_LogZ_InitDone = true;
		LogZ_GameLogger.WithObject(
		    this, "zombie loaded from DB",
		    LogZ_Level.DEBUG, LogZ_Event.SYSTEM_WORLD);

		return true;
	}

	// * --- kill ---
	override void EEKilled(Object killer)
	{
		if (!m_LogZ_Killed)
			LogZ_WorldLogger.WithKiller(this, killer, LogZ_Level.INFO);
		m_LogZ_Killed = true;

		super.EEKilled(killer);
	}

	// * --- hit ---
	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		LogZ_WorldLogger.WithHit(this, source, damageResult, damageType, dmgZone, ammo, LogZ_Level.INFO);

		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
	}

	// * --- cargo in ---
	override void EECargoIn(EntityAI item)
	{
		super.EECargoIn(item);

		LogZ_EECargoIn(item);
	}

	private void LogZ_EECargoIn(EntityAI item)
	{
		if (LogZ_ShouldLogged(true))
			LogZ_GameLogger.WithObjectAndOwner(
			    item, this, "added item to zombie",
			    LogZ_Level.INFO, LogZ_Event.INVENTORY_IN);
	}

	// * --- cargo out ---
	override void EECargoOut(EntityAI item)
	{
		super.EECargoOut(item);

		LogZ_EECargoOut(item);
	}

	private void LogZ_EECargoOut(EntityAI item)
	{
		if (LogZ_ShouldLogged(false))
			LogZ_GameLogger.WithObjectAndOwner(
			    item, this, "removed item from zombie",
			    LogZ_Level.DEBUG, LogZ_Event.INVENTORY_OUT);
	}

	// * --- attach ---
	override void EEItemAttached(EntityAI item, string slot_name)
	{
		super.EEItemAttached(item, slot_name);

		LogZ_EEItemAttached(item, slot_name);
	}

	private void LogZ_EEItemAttached(EntityAI item, string slot_name)
	{
		if (LogZ_ShouldLogged(true))
			LogZ_GameLogger.WithObjectAndOwner(
			    item, this, "attached item to zombie",
			    LogZ_Level.INFO, LogZ_Event.INVENTORY_IN, slot_name);
	}

	// * --- detach ---
	override void EEItemDetached(EntityAI item, string slot_name)
	{
		super.EEItemDetached(item, slot_name);

		LogZ_EEItemDetached(item, slot_name);
	}

	private void LogZ_EEItemDetached(EntityAI item, string slot_name)
	{
		if (LogZ_ShouldLogged(false))
			LogZ_GameLogger.WithObjectAndOwner(
			    item, this, "detached item from zombie",
			    LogZ_Level.DEBUG, LogZ_Event.INVENTORY_OUT, slot_name);
	}
}
#endif
