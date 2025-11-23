/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright (c) 2025 WoozyMasta
    Source: https://github.com/woozymasta/logz
*/

#ifdef SERVER
/**
    \brief High-level helpers for game/object logging.
*/
class LogZ_GameLogger
{
	/**
	    \brief Log message with single object payload.
	    \details
	        - Skips log if object null, level disabled or event disabled.
	        - Serializes object into appropriate DTO (optionally with detailed stats).
	        - Optionally serializes hierarchy root as "object_parent".
	    \param obj         Subject object.
	    \param msg         Message string.
	    \param lvl         LogZ_Level Log level.
	    \param ev          LogZ_Event Event type.
	    \param slot        Optional slot name (filtered by IsAllowedSlotName).
	    \param withParent  When true, include hierarchy root as "object_parent".
	    \param withStats   When true, use *Stats DTO variant if available.
	*/
	static void WithObject(Object obj, string msg, LogZ_Level lvl, LogZ_Event ev, string slot = "", bool withParent = false, bool withStats = false)
	{
		if (!obj || !LogZ_Levels.IsEnabled(lvl) || !LogZ_Events.IsEnabled(ev))
			return;

		ref map<string, string> dto = new map<string, string>();
		string json;

		if (SerializeObject(obj, json, withStats))
			dto.Insert("object", json);

		if (IsAllowedSlotName(slot))
			dto.Insert("slot", slot);

		if (withParent && SerializeParentObject(obj, json, withStats))
			dto.Insert("object_parent", json);

		LogZ.Log(msg, lvl, ev, dto);
	}

	/**
	    \brief Log message with object and owner/parent container.
	    \details
	        - Typical use: inventory operations, attachments, triggers.
	        - "object" is usually item, "owner" is container/player/vehicle.
	        - Optionally includes parent hierarchy for both.
	    \param obj          Primary object (item, victim, etc.).
	    \param owner        Owner or related object (container, trigger, vehicle...).
	    \param msg          Message string.
	    \param lvl          LogZ_Level Log level.
	    \param ev           LogZ_Event Event type.
	    \param slot         Optional slot name (filtered by IsAllowedSlotName).
	    \param withParents  When true, serialize hierarchy root for both object and owner.
	    \param withStats    When true, use *Stats DTO where possible.
	*/
	static void WithObjectAndOwner(Object obj, Object owner, string msg, LogZ_Level lvl, LogZ_Event ev, string slot = "", bool withParents = false, bool withStats = false)
	{
		if (!obj || !LogZ_Levels.IsEnabled(lvl) || !LogZ_Events.IsEnabled(ev))
			return;

		ref map<string, string> dto = new map<string, string>();
		string json;

		if (SerializeObject(obj, json, withStats))
			dto.Insert("object", json);

		if (withParents && SerializeParentObject(obj, json, withStats))
			dto.Insert("object_parent", json);

		if (IsAllowedSlotName(slot))
			dto.Insert("slot", slot);

		if (owner && obj != owner) {
			if (SerializeObject(owner, json, withStats))
				dto.Insert("owner", json);

			if (withParents && SerializeParentObject(obj, json, withStats))
				dto.Insert("owner_parent", json);
		}

		LogZ.Log(msg, lvl, ev, dto);
	}

	/**
	    \brief Serialize object into a JSON DTO string.
	    \details
	        - Selects DTO type by object runtime type:
	            Man -> LogZ_DTO_Man / LogZ_DTO_ManStats
	            Transport -> LogZ_DTO_Transport / LogZ_DTO_TransportStats
	            EntityAI -> LogZ_DTO_Entity / LogZ_DTO_EntityStats
	            other -> LogZ_DTO_Object / LogZ_DTO_ObjectStats
	        - Returns false if serializer or object invalid.
	    \param obj       Source object.
	    \param json[out] Resulting JSON string.
	    \param withStats When true, use *Stats DTO variant.
	    \return bool     True on success.
	*/
	static bool SerializeObject(Object obj, out string json, bool withStats = false)
	{
		if (!obj)
			return false;

		if (obj.IsMan()) {
			if (withStats) {
				LogZ_DTO_ManStats manStatsDTO = new LogZ_DTO_ManStats(obj);
				return LogZ.GetSerializer().WriteToString(manStatsDTO, false, json);
			}

			LogZ_DTO_Man manDTO = new LogZ_DTO_Man(obj);
			return (LogZ.GetSerializer().WriteToString(manDTO, false, json));
		}

		if (obj.IsTransport()) {
			if (withStats) {
				LogZ_DTO_TransportStats vehicleStatsDTO = new LogZ_DTO_TransportStats(obj);
				return LogZ.GetSerializer().WriteToString(vehicleStatsDTO, false, json);
			}

			LogZ_DTO_Transport vehicleDTO = new LogZ_DTO_Transport(obj);
			return (LogZ.GetSerializer().WriteToString(vehicleDTO, false, json));
		}

		if (obj.IsEntityAI()) {
			if (withStats) {
				LogZ_DTO_EntityStats entityStatsDTO = new LogZ_DTO_EntityStats(obj);
				return LogZ.GetSerializer().WriteToString(entityStatsDTO, false, json);
			}

			LogZ_DTO_Entity entityDTO = new LogZ_DTO_Entity(obj);
			return LogZ.GetSerializer().WriteToString(entityDTO, false, json);
		}

		if (withStats) {
			LogZ_DTO_ObjectStats objStatsDTO = new LogZ_DTO_ObjectStats(obj);
			return LogZ.GetSerializer().WriteToString(objStatsDTO, false, json);
			return true;
		}

		LogZ_DTO_Object objDTO = new LogZ_DTO_Object(obj);
		return LogZ.GetSerializer().WriteToString(objDTO, false, json);
	}

	/**
	    \brief Serialize hierarchy root of an EntityAI as parent.
	    \details
	        - Only works for EntityAI.
	        - Skips when root equals the object itself.
	    \param obj       Child object.
	    \param json[out] JSON for parent object.
	    \param withStats Use *Stats DTO for parent if true.
	    \return bool     True when parent exists and serialized.
	*/
	static bool SerializeParentObject(Object obj, out string json, bool withStats = false)
	{
		if (!obj || !obj.IsEntityAI())
			return false;

		EntityAI eai = EntityAI.Cast(obj);
		if (!eai)
			return false;

		EntityAI parent = eai.GetHierarchyRoot();
		if (!parent || parent == eai)
			return false;

		return SerializeObject(parent, json, withStats);
	}

	/**
	    \brief Whitelist filter for slot names.
	    \return bool True if slot name can be logged.
	*/
	protected static bool IsAllowedSlotName(string slot)
	{
		if (slot == string.Empty)
			return false;

		if (slot == "RevolverCylinder" || slot == "RevolverEjector")
			return false;

		return true;
	}
}
#endif
