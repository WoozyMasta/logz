/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright (c) 2025 WoozyMasta
    Source: https://github.com/woozymasta/logz
*/

#ifdef SERVER
/**
    \brief Serializable view of a transport (Car/Boat/Heli etc.).
*/
class LogZ_DTO_Transport : LogZ_DTO_Object
{
	int id;
	float lifetime;
	float speed;
	ref array<ref LogZ_DTO_Man> members;

	/**
	    \brief Populate transport-specific fields and crew.
	*/
	void FillTransport(Object obj)
	{
		Transport veh;
		if (!Class.CastTo(veh, obj))
			return;

		id = LogZ_Object.PersistentHash(veh);
		lifetime = veh.GetLifetime();
		speed = GetVelocity(veh).Length();

		members = new array<ref LogZ_DTO_Man>();
		int crewSize = veh.CrewSize();
		for (int i = 0; i < crewSize; ++i) {
			Man man = veh.CrewMember(i);
			if (man)
				members.Insert(new LogZ_DTO_Man(man));
		}
	}

	/**
	    \brief Construct DTO from object.
	*/
	void LogZ_DTO_Transport(Object obj)
	{
		Fill(obj);
		FillTransport(obj);
	}
}

/**
    \brief Serializable view of a transport with extended stats.
*/
class LogZ_DTO_TransportStats : LogZ_DTO_ObjectStats
{
	int id;
	float lifetime;
	float speed;
	ref array<ref LogZ_DTO_ManStats> members;

	/**
	    \brief Populate transport stats and crew stats.
	*/
	void FillTransport(Object obj)
	{
		Transport veh;
		if (!Class.CastTo(veh, obj))
			return;

		id = LogZ_Object.PersistentHash(veh);
		lifetime = veh.GetLifetime();
		speed = GetVelocity(veh).Length();

		members = new array<ref LogZ_DTO_ManStats>();
		int crewSize = veh.CrewSize();
		for (int i = 0; i < crewSize; ++i) {
			Man man = veh.CrewMember(i);
			if (man)
				members.Insert(new LogZ_DTO_ManStats(man));
		}
	}

	/**
	    \brief Construct DTO from object.
	*/
	void LogZ_DTO_TransportStats(Object obj)
	{
		Fill(obj);
		FillStats(obj);
		FillTransport(obj);
	}
}
#endif
