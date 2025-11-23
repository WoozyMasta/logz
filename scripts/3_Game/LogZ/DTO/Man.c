/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright (c) 2025 WoozyMasta
    Source: https://github.com/woozymasta/logz
*/

#ifdef SERVER
/**
    \brief Serializable view of a Man (player or eAI).
*/
class LogZ_DTO_Man : LogZ_DTO_Object
{
	float blood;
	float shock;
	int id;
	string player_name;
	string steam_id;

	/**
	    \brief Populate Man-specific fields.
	*/
	void FillMan(Object obj)
	{
		Man man;
		if (!Class.CastTo(man, obj))
			return;

		blood = man.GetHealth01("", "Blood");
		shock = man.GetHealth01("", "Shock");
		id = LogZ_Object.PersistentHash(man);

		PlayerIdentity identity = man.GetIdentity();
		if (!identity)
			return;

		player_name = identity.GetName();
		steam_id = identity.GetPlainId();

		if (player_name == string.Empty && steam_id == string.Empty)
			player_name = "AI " + obj.ClassName();
	}

	/**
	    \brief Construct DTO from object.
	*/
	void LogZ_DTO_Man(Object obj)
	{
		Fill(obj);
		FillMan(obj);
	}
}

/**
    \brief Serializable view of a Man with full object stats.
*/
class LogZ_DTO_ManStats : LogZ_DTO_ObjectStats
{
	float blood;
	float shock;
	int id;
	string player_name;
	string steam_id;

	/**
	    \brief Populate Man-specific fields.
	*/
	void FillMan(Object obj)
	{
		Man man;
		if (!Class.CastTo(man, obj))
			return;

		blood = man.GetHealth01("", "Blood");
		shock = man.GetHealth01("", "Shock");
		id = LogZ_Object.PersistentHash(man);

		PlayerIdentity identity = man.GetIdentity();
		if (!identity)
			return;

		player_name = identity.GetName();
		steam_id = identity.GetPlainId();
	}

	/**
	    \brief Construct DTO from object.
	*/
	void LogZ_DTO_ManStats(Object obj)
	{
		Fill(obj);
		FillStats(obj);
		FillMan(obj);
	}
}
#endif
