/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright (c) 2025 WoozyMasta
    Source: https://github.com/woozymasta/logz
*/

#ifdef SERVER
/**
    \brief Serializable damage snapshot for hit/kill events.
*/
class LogZ_DTO_Damage
{
	float damage;
	string damage_zone;
	string damage_type;
	string ammo_type;

	/**
	    \brief Construct DTO from TotalDamageResult, type and zone.
	*/
	void LogZ_DTO_Damage(TotalDamageResult damageResult, int damageType, string dmgZone, string ammo)
	{
		if (damageResult)
			damage = damageResult.GetDamage(dmgZone, "");
		else
			damage = 0.0;

		damage_zone = dmgZone;
		damage_type = EnumTools.EnumToString(DamageType, damageType);
		ammo_type = ammo;
	}
}
#endif
