/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright (c) 2025 WoozyMasta
    Source: https://github.com/woozymasta/logz
*/

#ifdef SERVER
modded class PlayerBase
{
	protected bool m_LogZ_InitDone;
	protected bool m_LogZ_Killed;

	bool LogZ_ShouldLogged()
	{
		return (m_PlayerSelected && m_LogZ_InitDone);
	}

	bool LogZ_IsAlreadyKilled()
	{
		return m_LogZ_Killed;
	}

	// * --- create ---
	override void EEOnCECreate()
	{
		super.EEOnCECreate();

		LogZ_GameLogger.WithObject(
		    this, "player created from CE",
		    LogZ_Level.DEBUG, LogZ_Event.SYSTEM_WORLD);
	}

	// * --- load ---
	override bool OnStoreLoad(ParamsReadContext ctx, int version)
	{
		if (!super.OnStoreLoad(ctx, version))
			return false;

		LogZ_GameLogger.WithObject(
		    this, "player loaded from DB",
		    LogZ_Level.DEBUG, LogZ_Event.SYSTEM_WORLD);

		return true;
	}

	// * --- kill ---
	override void EEKilled(Object killer)
	{
		if (!LogZ_IsAlreadyKilled())
			LogZ_WorldLogger.WithKiller(this, killer, LogZ_Level.INFO);
		m_LogZ_Killed = true;

		super.EEKilled(killer);
	}

	override void OnDamageDestroyed(int oldLevel)
	{
		super.OnDamageDestroyed(oldLevel);

		if (!LogZ_IsAlreadyKilled())
			LogZ_GameLogger.WithObject(
			    this, "player death",
			    LogZ_Level.INFO, LogZ_Event.PLAYER_KILL);
		m_LogZ_Killed = true;
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
		if (LogZ_ShouldLogged())
			LogZ_GameLogger.WithObjectAndOwner(
			    item, this, "added item to player",
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
		if (LogZ_ShouldLogged())
			LogZ_GameLogger.WithObjectAndOwner(
			    item, this, "removed item from player",
			    LogZ_Level.INFO, LogZ_Event.INVENTORY_OUT);
	}

	// * --- attach ---
	override void EEItemAttached(EntityAI item, string slot_name)
	{
		super.EEItemAttached(item, slot_name);

		LogZ_EEItemAttached(item, slot_name);
	}

	private void LogZ_EEItemAttached(EntityAI item, string slot_name)
	{
		if (LogZ_ShouldLogged())
			LogZ_GameLogger.WithObjectAndOwner(
			    item, this, "attached item to player",
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
		if (LogZ_ShouldLogged())
			LogZ_GameLogger.WithObjectAndOwner(
			    item, this, "detached item from player",
			    LogZ_Level.INFO, LogZ_Event.INVENTORY_OUT, slot_name);
	}

	// * --- hands in ---
	override void EEItemIntoHands(EntityAI item)
	{
		super.EEItemIntoHands(item);

		LogZ_EEItemIntoHands(item);
	}

	private void LogZ_EEItemIntoHands(EntityAI item)
	{
		if (LogZ_ShouldLogged())
			LogZ_GameLogger.WithObjectAndOwner(
			    item, this, "received item into player hands",
			    LogZ_Level.INFO, LogZ_Event.INVENTORY_IN, "Hands");
	}

	// * --- hands out ---
	override void EEItemOutOfHands(EntityAI item)
	{
		super.EEItemOutOfHands(item);

		LogZ_EEItemOutOfHands(item);
	}

	private void LogZ_EEItemOutOfHands(EntityAI item)
	{
		if (item)
			LogZ_GameLogger.WithObjectAndOwner(
			    item, this, "out item from player hands",
			    LogZ_Level.INFO, LogZ_Event.INVENTORY_OUT, "Hands");
	}

	// * --- session ---
	override void OnSelectPlayer()
	{
		super.OnSelectPlayer();

		LogZ_WorldLogger.WithPlayer(
		    this, "player selected",
		    LogZ_Level.DEBUG, LogZ_Event.PLAYER_SESSION);
	}

	override void OnConnect()
	{
		super.OnConnect();

		m_LogZ_InitDone = true;
		LogZ_GameLogger.WithObject(
		    this, "player connected",
		    LogZ_Level.INFO, LogZ_Event.PLAYER_SESSION);
	}

	override void OnReconnect()
	{
		super.OnReconnect();

		LogZ_GameLogger.WithObject(
		    this, "player reconnected",
		    LogZ_Level.INFO, LogZ_Event.PLAYER_SESSION);
	}

	override void OnDisconnect()
	{
		super.OnDisconnect();

		m_LogZ_InitDone = false;
		LogZ_GameLogger.WithObject(
		    this, "player disconnected",
		    LogZ_Level.INFO, LogZ_Event.PLAYER_SESSION);
	}

	// * --- unconscious ---
	override void OnUnconsciousStart()
	{
		super.OnUnconsciousStart();

		LogZ_GameLogger.WithObject(
		    this, "player start unconscious",
		    LogZ_Level.INFO, LogZ_Event.PLAYER_ACTIVITY);
	}

	override void OnUnconsciousStop(int pCurrentCommandID)
	{
		super.OnUnconsciousStop(pCurrentCommandID);

		LogZ_GameLogger.WithObject(
		    this, "player stop unconscious",
		    LogZ_Level.INFO, LogZ_Event.PLAYER_ACTIVITY);
	}

	// * --- restrain ---
	override void SetRestrained(bool is_restrained)
	{
		super.SetRestrained(is_restrained);

		if (is_restrained)
			LogZ_GameLogger.WithObject(
			    this, "player restrained",
			    LogZ_Level.INFO, LogZ_Event.PLAYER_ACTIVITY);
		else
			LogZ_GameLogger.WithObject(
			    this, "player stop restrain",
			    LogZ_Level.INFO, LogZ_Event.PLAYER_ACTIVITY);
	}

	// * --- movement ---
	override void OnCommandFallStart()
	{
		super.OnCommandFallStart();

		LogZ_GameLogger.WithObject(
		    this, "player start falling down",
		    LogZ_Level.DEBUG, LogZ_Event.PLAYER_ACTIVITY);
	}

	override void OnCommandFallFinish()
	{
		super.OnCommandFallFinish();

		LogZ_GameLogger.WithObject(
		    this, "player falling down",
		    LogZ_Level.INFO, LogZ_Event.PLAYER_ACTIVITY);
	}

	override void OnCommandSwimStart()
	{
		super.OnCommandSwimStart();

		LogZ_GameLogger.WithObject(
		    this, "player start swimming",
		    LogZ_Level.INFO, LogZ_Event.PLAYER_ACTIVITY);
	}

	override void OnCommandSwimFinish()
	{
		super.OnCommandSwimFinish();

		LogZ_GameLogger.WithObject(
		    this, "player finish swimming",
		    LogZ_Level.INFO, LogZ_Event.PLAYER_ACTIVITY);
	}

	override void OnJumpOutVehicleFinish(float carSpeed)
	{
		super.OnJumpOutVehicleFinish(carSpeed);

		LogZ_GameLogger.WithObject(
		    this, string.Format("player jump out from vehicle on speed %1", carSpeed),
		    LogZ_Level.INFO, LogZ_Event.PLAYER_ACTIVITY);
	}
}
#endif
