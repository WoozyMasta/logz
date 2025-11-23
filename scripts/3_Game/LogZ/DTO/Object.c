/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright (c) 2025 WoozyMasta
    Source: https://github.com/woozymasta/logz
*/

#ifdef SERVER
/**
    \brief Common subset of object data for logging.
*/
class LogZ_DTO_Object_Base
{
	string name;
	string type;
	vector pos;
	int yaw;
	float health;

	/**
	    \brief Populate object fields from Object if non-null.
	*/
	void Fill(Object obj)
	{
		if (!obj)
			return;

		name = LogZ_Object.GetName(obj);
		type = LogZ_Object.GetType(obj);
		pos = obj.GetPosition();
		yaw = Math.Round(obj.GetOrientation()[0]);
		health = obj.GetHealth01("", "Health");
	}
}

/**
    \brief Serializable view of any Object using base subset.
*/
class LogZ_DTO_Object : LogZ_DTO_Object_Base
{
	/**
	    \brief Construct DTO from object.
	*/
	void LogZ_DTO_Object(Object obj)
	{
		Fill(obj);
	}
}

/**
    \brief Extended object stats with allow_damage and damage zones.
*/
class LogZ_DTO_ObjectStats : LogZ_DTO_Object_Base
{
	bool allow_damage;
	ref map<string, float> zones;

	/**
	    \brief Populate damage-related stats from Object.
	*/
	void FillStats(Object obj)
	{
		if (!obj)
			return;

		allow_damage = obj.GetAllowDamage();

		zones = new map<string, float>();
		array<string> damageZones = new array<string>();
		obj.GetDamageZones(damageZones);

		foreach (string dz : damageZones)
			zones.Insert(dz, obj.GetHealth01(dz, "Health"));
	}

	/**
	    \brief Construct DTO from object with basic and damage stats.
	*/
	void LogZ_DTO_ObjectStats(Object obj)
	{
		Fill(obj);
		FillStats(obj);
	}
}
#endif
