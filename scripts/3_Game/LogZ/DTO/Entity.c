/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright (c) 2025 WoozyMasta
    Source: https://github.com/woozymasta/logz
*/

#ifdef SERVER
/**
    \brief Serializable view of EntityAI object.
*/
class LogZ_DTO_Entity : LogZ_DTO_Object_Base
{
	int id;
	float lifetime;

	/**
	    \brief Populate EntityAI-specific fields.
	*/
	void FillEntity(Object obj)
	{
		EntityAI eai;
		if (!Class.CastTo(eai, obj))
			return;

		id = LogZ_Object.PersistentHash(eai);
		lifetime = eai.GetLifetime();
	}

	/**
	    \brief Construct DTO from object.
	*/
	void LogZ_DTO_Entity(Object obj)
	{
		Fill(obj);
		FillEntity(obj);
	}
}

/**
    \brief Extended EntityAI stats.
*/
class LogZ_DTO_EntityStats : LogZ_DTO_Entity
{
	float quantity;
	float wet;
	float temperature;
	int liquid;
	int clean;
	int agent;
	float em;
	bool em_on;

	/**
	    \brief Populate EntityAI stats if object is EntityAI.
	*/
	void FillEntityStats(Object obj)
	{
		EntityAI eai;
		if (!Class.CastTo(eai, obj))
			return;

		int capacity = eai.GetQuantityMax() - eai.GetQuantityMin();
		if (capacity > 0)
			quantity = eai.GetQuantity() / capacity;
		else
			quantity = 0;

		wet = eai.GetWet();
		temperature = eai.GetTemperature();
		liquid = eai.GetLiquidType();
		clean = eai.GetCleanness();
		agent = eai.GetAgents();

		if (eai.HasEnergyManager()) {
			em = eai.GetCompEM().GetEnergy();
			em_on = eai.GetCompEM().IsSwitchedOn();
		}
	}

	/**
	    \brief Construct DTO with base object, entity and stat fields.
	*/
	void LogZ_DTO_EntityStats(Object obj)
	{
		Fill(obj);
		FillEntity(obj);
		FillEntityStats(obj);
	}
}
#endif
