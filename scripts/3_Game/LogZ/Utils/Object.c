/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright (c) 2025 WoozyMasta
    Source: https://github.com/woozymasta/logz
*/

#ifdef SERVER
/**
    \brief Helpers for deriving normalized object names and types.
*/
class LogZ_Object
{
	// Known suffixes to strip from type names (lowercase, without leading "_").
	// Suffixes collected by grep|awk|sort|uniq on all vanilla game config.cpp files
	// ~5-10 words added with AI like most popular for modders
	static const ref array<string> NAME_SUFFIXES = {
		"base",
		"colorbase",

		// state
		"opened", "closed",
		"dirty", "rust", "damaged", "wet", "damp", "dry", "clean",

		// colors
		"black", "white",
		"grey", "gray", "lightgrey", "lightgray", "darkgrey", "darkgray", "silver",
		"red", "pink", "magenta", "gold",
		"orange", "tan",
		"yellow",
		"green", "lightgreen", "darkgreen", "olive", "lime",
		"cyan", "teal", "aqua",
		"blue", "lightblue", "darkblue", "cobalt",
		"violet", "purple", "lavender",
		"brown", "lightbrown", "darkbrown", "beige",

		// seasons
		"winter",
		"summer",
		"spring",
		"autumn",

		// patterns / styles
		"natural",
		"stripes", "greenchecks",
		"camo", "multicam", "digital",
		"snow", "arctic",
		"woodland", "forest", "jungle", "mossy",
		"tropic", "tropical",
		"khaki",
		"navy",
		"metal",
		"desert", "sand",
		"flat",
	};

	/**
	    \brief Derive a readable object type/name.
	    \param obj       Source object.
	    \param stripBase If true, try to strip known postfix after last "_".
	    \return string Non-empty name or "unknown"/"none" for invalid objects.
	*/
	static string GetName(Object obj, bool stripBase = false)
	{
		if (!obj)
			return "none";

		string typeName = obj.GetType();
		if (typeName != string.Empty) {
			if (stripBase)
				typeName = StripSuffix(typeName);

			return typeName;
		}

		string className = obj.ClassName();
		if (className != string.Empty) {
			if (className != "Object" && className != "House") {
				if (stripBase)
					className = StripSuffix(className);

				return className;
			}
		}

		string dbg = obj.GetDebugNameNative();

		// drop "NOID " prefix for house proxies
		if (dbg.IndexOf("NOID ") == 0)
			dbg = dbg.Substring(5, dbg.Length() - 5);

		if (dbg == string.Empty)
			return "unknown";

		array<string> parts = new array<string>();
		dbg.Split(":", parts);

		string part;
		if (parts.Count() == 2) {
			if (parts[1].IndexOf(".p3d") > 0)
				part = parts[1].Substring(0, parts[1].Length() - 4);
			else
				part = parts[0];
		} else
			part = dbg;

		part.TrimInPlace();

		return part;
	}

	/**
	    \brief Map Object to high-level type string.
	*/
	static string GetType(Object obj)
	{
		if (obj.IsTransport()) {
			Transport veh = Transport.Cast(obj);
			if (!veh)
				return "object";

			string vehType = veh.GetVehicleType();
			vehType.Replace("VehicleType", "");
			if (vehType == "Undefined")
				return "vehicle";

			vehType.ToLower();
			return vehType;
		}

		if (obj.CanUseConstruction())
			return "base building";

		if (obj.IsBuilding() || obj.IsFuelStation())
			return "building";

		if (obj.IsContainer())
			return "container";

		if (obj.IsMagazine())
			return "magazine";

		if (obj.IsAmmoPile())
			return "ammo pile";

		if (obj.IsClothing())
			return "clothing";

		if (obj.IsTree())
			return "tree";

		if (obj.IsRock())
			return "rock";

		if (obj.IsBush())
			return "bush";

		if (obj.IsHologram())
			return "hologram";

		if (obj.IsFood())
			return "food";

		if (obj.IsWeapon())
			return "weapon";

		if (obj.IsEntityAI()) {
			EntityAI eai = EntityAI.Cast(obj);
			if (!eai)
				return "object";

			if (eai.IsPlayer())
				return "player";

			if (eai.IsZombieMilitary())
				return "zombie military";

			if (eai.IsZombie())
				return "zombie";

			if (eai.IsAnimal())
				return "animal";

			if (eai.IsCookware())
				return "cookware";

			if (eai.IsBasebuildingKit())
				return "building kit";

			if (eai.IsHeavyBehaviour())
				return "heavy entity";

			if (eai.IsOneHandedBehaviour())
				return "one handed entity";

			if (eai.IsTwoHandedBehaviour())
				return "two handed entity";
		}

		if (obj.IsEntityAI())
			return "entity";

		return "object";
	}

	/**
	    \brief Generate stable hash from entity persistent ID.
	    \param entity EntityAI reference.
	    \return int Stable hash per persistent identity or per runtime for non-persistent.
	*/
	static int PersistentHash(EntityAI entity)
	{
		if (!entity)
			return 0;

		int p1, p2, p3, p4;
		entity.GetPersistentID(p1, p2, p3, p4);

		// Non-persistent objects assign random but consistent per runtime
		if (p1 == 0 && p2 == 0 && p3 == 0 && p4 == 0)
			return string.Format("%1_%2", entity.GetType(), Math.RandomInt(1, int.MAX)).Hash();

		return string.Format("%1_%2_%3_%4", p1, p2, p3, p4).Hash();
	}

	/**
	    \brief Strip a single known suffix from a normalized name.
	    \param name Normalized type name.
	    \return string Name without the last known suffix, or original name if nothing matches.
	*/
	static string StripSuffix(string name)
	{
		name.TrimInPlace();
		if (name == string.Empty)
			return name;

		int sep = name.LastIndexOf("_");
		if (sep <= 0 || sep >= name.Length() - 1)
			return name;

		string suffix = name.Substring(sep + 1, name.Length() - (sep + 1));
		if (suffix == string.Empty)
			return name;

		for (int i = 0; i < NAME_SUFFIXES.Count(); ++i) {
			if (suffix == NAME_SUFFIXES[i])
				return name.Substring(0, sep);
		}

		// generic <digits>rnd, e.g. 5rnd, 20rnd, 100rnd, 1000rnd...
		int len = suffix.Length();
		if (len > 3 && suffix.Substring(len - 3, 3) == "rnd") {
			if (LogZ_Utils.IsDigitsOnly(suffix.Substring(0, len - 3)))
				return name.Substring(0, sep);
		}

		return name;
	}
}
