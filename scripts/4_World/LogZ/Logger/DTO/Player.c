/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright (c) 2025 WoozyMasta
    Source: https://github.com/woozymasta/logz
*/

#ifdef SERVER
/**
    \brief Serializable view of a player with full stats snapshot.
*/
class LogZ_DTO_Player : LogZ_DTO_ManStats
{
	// Player-specific stats snapshot.
	ref LogZ_DTO_PlayerStats stats;

	/**
	    \brief Populate player-specific stats if object is PlayerBase.
	*/
	void FillPlayer(Object obj)
	{
		PlayerBase player;
		if (!Class.CastTo(player, obj))
			return;

		stats = new LogZ_DTO_PlayerStats(player);
	}

	/**
	    \brief Construct full player DTO from object.
	*/
	void LogZ_DTO_Player(Object obj)
	{
		Fill(obj);
		FillStats(obj);
		FillMan(obj);
		FillPlayer(obj);
	}
}

/**
    \brief Build stats snapshot from PlayerBase.
    \param player Source PlayerBase.
*/
class LogZ_DTO_PlayerStats
{
	string blood_type;
	bool blood_type_visible;
	int bleeding_bits;
	int blood_hands;

	int agents;
	int life_span;
	int broken_leg;
	int last_shaved;

	bool immunity_boost;
	bool unconscious;
	bool restrained;
	bool third_person;

	// EPlayerStats
	float water;
	float toxicity;
	float energy;
	float heat_comfort;
	float tremor;
	float diet;
	float stamina;
	float specialty;
	float heat_buffer;

	// AnalyticsManagerServer
	float playtime;
	float distance;
	float longest_survivor_hit;
	int players_killed;
	int infected_killed;

	void LogZ_DTO_PlayerStats(PlayerBase player)
	{
		if (!player)
			return;

		bool _p; string _t;
		blood_type = BloodTypes.GetBloodTypeName(player.GetStatBloodType().Get(), _t, _p);
		blood_type_visible = player.HasBloodTypeVisible();
		bleeding_bits = player.GetBleedingBits();
		blood_hands = player.HasBloodyHandsEx();

		agents = player.GetAgents();
		life_span = player.GetLifeSpanState();
		broken_leg = player.GetBrokenLegs();
		last_shaved = player.GetLastShavedSeconds();
		immunity_boost = player.m_ImmunityBoosted;
		unconscious = player.IsUnconscious();
		restrained = player.IsRestrained();
		third_person = player.IsInThirdPerson();

		water = player.GetStatWater().Get();
		toxicity = player.GetStatToxicity().Get();
		energy = player.GetStatEnergy().Get();
		heat_comfort = player.GetStatHeatComfort().Get();
		tremor = player.GetStatTremor().Get();
		diet = player.GetStatDiet().Get();
		stamina = player.GetStatStamina().Get();
		specialty = player.GetStatSpecialty().Get();
		heat_buffer = player.GetStatHeatBuffer().Get();

		playtime = player.StatGet(AnalyticsManagerServer.STAT_PLAYTIME);
		distance = player.StatGet(AnalyticsManagerServer.STAT_DISTANCE);
		longest_survivor_hit = player.StatGet(AnalyticsManagerServer.STAT_LONGEST_SURVIVOR_HIT);
		players_killed = player.StatGet(AnalyticsManagerServer.STAT_PLAYERS_KILLED);
		infected_killed = player.StatGet(AnalyticsManagerServer.STAT_INFECTED_KILLED);
	}
}
#endif
