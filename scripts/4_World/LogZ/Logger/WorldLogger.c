/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright (c) 2025 WoozyMasta
    Source: https://github.com/woozymasta/logz
*/

#ifdef SERVER
/**
    \brief High-level world logger helpers.
*/
class LogZ_WorldLogger
{
	/**
	    \brief Log full player snapshot with optional message.
	    \param player    PlayerBase subject.
	    \param msg       Message to log (optional).
	    \param lvl       Log level (default INFO).
	    \param eventType Event type (must be enabled).
	*/
	static void WithPlayer(PlayerBase player, string msg = "", LogZ_Level lvl = 2, LogZ_Event eventType = 0)
	{
		if (!player || !LogZ_Levels.IsEnabled(lvl) || !LogZ_Events.IsEnabled(eventType))
			return;

		if (msg == string.Empty)
			msg = player.ClassName();

		ref map<string, string> dto = new map<string, string>();
		string json;

		LogZ_DTO_Player playerDTO = new LogZ_DTO_Player(player);
		if (LogZ.GetSerializer().WriteToString(playerDTO, false, json))
			dto.Insert("player", json);

		LogZ.Log(msg, lvl, eventType, dto);
	}

	/**
	    \brief Log kill/death event with victim and killer context.
	    \param victim Victim object.
	    \param killer Attacker object or null.
	    \param lvl    Log level (default INFO).
	*/
	static void WithKiller(Object victim, Object killer, LogZ_Level lvl = 2)
	{
		if (!LogZ_Config.IsLoaded() || !victim || !LogZ_Levels.IsEnabled(lvl))
			return;

		if (LogZ_Config.Get().filters.only_player_suicide && killer == victim)
			return;

		LogZ_Event eventType = ResolveVictimEvent(victim, false);
		if (!LogZ_Events.IsEnabled(eventType))
			return;

		ref map<string, string> dto = new map<string, string>();
		string json;

		if (LogZ_GameLogger.SerializeObject(victim, json))
			dto.Insert("victim", json);

		if (LogZ_GameLogger.SerializeParentObject(victim, json))
			dto.Insert("victim_parent", json);

		if (!killer) {
			LogZ.Log(LogZ_Object.GetType(victim) + " died", lvl, eventType, dto);
			return;
		}

		if (killer == victim) {
			LogZ.Log(LogZ_Object.GetType(victim) + " death or suicide", lvl, eventType, dto);
			return;
		}

		dto.Insert("distance", LogZ_Utils.Distance(killer, victim));

		EntityAI killerEntity = EntityAI.Cast(killer);

		if (LogZ_GameLogger.SerializeObject(killer, json))
			dto.Insert("attacker", json);

		if (LogZ_GameLogger.SerializeParentObject(killer, json))
			dto.Insert("attacker_parent", json);

		LogZ.Log(LogZ_Object.GetType(victim) + " killed", lvl, eventType, dto);
	}

	/**
	    \brief Log hit / damage event with victim, attacker and damage payload.
	    \param victim       Damaged object.
	    \param source       Damage source (EntityAI) or null.
	    \param damageResult TotalDamageResult or null.
	    \param damageType   DamageType enum value.
	    \param dmgZone      Damage zone name.
	    \param ammo         Ammo type name.
	    \param lvl          Log level (default INFO).
	*/
	static void WithHit(Object victim, EntityAI source, TotalDamageResult damageResult, int damageType, string dmgZone, string ammo, LogZ_Level lvl = 2)
	{
		if (!LogZ_Config.IsLoaded() || !victim || victim.IsDamageDestroyed() || !LogZ_Levels.IsEnabled(lvl))
			return;

		LogZ_Event eventType = ResolveVictimEvent(victim, true);
		if (!LogZ_Events.IsEnabled(eventType))
			return;

		if (damageResult) {
			float damage = damageResult.GetDamage(dmgZone, "");
			if (damage < LogZ_Config.Get().thresholds.hit_damage)
				return;

			if (source && source.IsTransport() && damage < LogZ_Config.Get().thresholds.hit_damage_vehicle)
				return;
		}

		ref map<string, string> dto = new map<string, string>();
		string json;

		if (LogZ_GameLogger.SerializeObject(victim, json))
			dto.Insert("victim", json);

		if (LogZ_GameLogger.SerializeParentObject(victim, json))
			dto.Insert("victim_parent", json);

		LogZ_DTO_Damage damageDTO = new LogZ_DTO_Damage(damageResult, damageType, dmgZone, ammo);
		if (LogZ.GetSerializer().WriteToString(damageDTO, false, json))
			dto.Insert("damage", json);

		if (!source) {
			LogZ.Log(LogZ_Object.GetType(victim) + " damaged", lvl, eventType, dto);
			return;
		}

		if (source == victim) {
			LogZ.Log(LogZ_Object.GetType(victim) + " hit self", lvl, eventType, dto);
			return;
		}

		dto.Insert("distance", LogZ_Utils.Distance(source, victim));

		if (LogZ_GameLogger.SerializeObject(source, json))
			dto.Insert("attacker", json);

		if (LogZ_GameLogger.SerializeParentObject(source, json))
			dto.Insert("attacker_parent", json);

		LogZ.Log(LogZ_Object.GetType(victim) + " hit", lvl, eventType, dto);
	}

	/**
	    \brief Log action start/end with attached context.
	    \param action_data ActionData instance.
	    \param isStart     True for start, false for end.
	    \param lvl         Log level.
	*/
	static void WithActionData(ActionData action_data, bool isStart, LogZ_Level lvl)
	{
		if (!action_data || !action_data.LogZ_IsAllowed() || !LogZ_Levels.IsEnabled(lvl))
			return;

		LogZ_Event eventType;
		string msg;

		if (isStart) {
			eventType = LogZ_Event.ACTION_START;
			msg = "action start";
		} else {
			eventType = LogZ_Event.ACTION_END;
			msg = "action end";
		}

		ref map<string, string> dto = new map<string, string>();
		string json;

		LogZ_DTO_ActionData actionDTO = new LogZ_DTO_ActionData(action_data);
		if (LogZ.GetSerializer().WriteToString(actionDTO, false, json))
			dto.Insert("action_data", json);

		if (action_data.m_Player) {
			if (LogZ_GameLogger.SerializeObject(action_data.m_Player, json, true))
				dto.Insert("player", json);
		}

		if (action_data.m_MainItem) {
			if (LogZ_GameLogger.SerializeObject(action_data.m_MainItem, json, true))
				dto.Insert("item", json);
		}

		if (action_data.m_Target) {
			Object targetObj = action_data.m_Target.GetObject();
			if (targetObj && targetObj != action_data.m_MainItem) {
				if (action_data.m_MainItem)
					dto.Insert("distance", LogZ_Utils.Distance(targetObj, action_data.m_MainItem));
				else if (action_data.m_Player)
					dto.Insert("distance", LogZ_Utils.Distance(targetObj, action_data.m_Player));

				if (LogZ_GameLogger.SerializeObject(targetObj, json))
					dto.Insert("target", json);
			}

			Object targetParentObj = action_data.m_Target.GetParent();
			if (targetParentObj && targetParentObj != action_data.m_Player) {
				if (!targetObj && action_data.m_MainItem)
					dto.Insert("distance", LogZ_Utils.Distance(targetParentObj, action_data.m_MainItem));
				else if (!targetObj && action_data.m_Player)
					dto.Insert("distance", LogZ_Utils.Distance(targetParentObj, action_data.m_Player));

				if (LogZ_GameLogger.SerializeObject(targetParentObj, json))
					dto.Insert("target_parent", json);
			}
		}

		LogZ.Log(msg, lvl, eventType, dto);
	}

	/**
	    \brief Resolve hit/kill event type by victim and hit flag.
	    \param victim Victim object.
	    \param isHit  True for hit, false for kill.
	    \return LogZ_Event Event type for this victim/hit combo.
	*/
	protected static LogZ_Event ResolveVictimEvent(Object victim, bool isHit)
	{
		if (victim.IsDayZCreature()) {
			if (isHit)
				return LogZ_Event.CREATURE_HIT;

			return LogZ_Event.CREATURE_KILL;
		}

		if (victim.IsTransport()) {
			if (isHit)
				return LogZ_Event.TRANSPORT_HIT;

			return LogZ_Event.TRANSPORT_KILL;
		}

		if (victim.CanUseConstruction() || victim.IsBuilding() || victim.IsFuelStation()) {
			if (isHit)
				return LogZ_Event.BUILDING_HIT;

			return LogZ_Event.BUILDING_KILL;
		}

		if (victim.IsMan()) {
#ifdef EXPANSIONMODAI
			if (victim.IsInherited(eAIBase)) {
				if (isHit)
					return LogZ_Event.CREATURE_HIT;

				return LogZ_Event.CREATURE_KILL;
			}
#endif

			if (isHit)
				return LogZ_Event.PLAYER_HIT;

			return LogZ_Event.PLAYER_KILL;
		}

		if (isHit)
			return LogZ_Event.ENTITY_HIT;

		return LogZ_Event.ENTITY_KILL;
	}
}
#endif
