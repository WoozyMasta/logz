/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright (c) 2025 WoozyMasta
    Source: https://github.com/woozymasta/logz
*/

#ifdef SERVER
modded class ActionData
{
	// Maybe can help for override in some noisy actions.
	bool LogZ_IsAllowed()
	{
		return true;
	}
}

modded class ActionManagerBase
{
	override void OnActionEnd()
	{
		if (m_CurrentActionData)
			LogZ_WorldLogger.WithActionData(m_CurrentActionData, false, LogZ_Level.INFO);

		super.OnActionEnd();
	}
}

modded class ActionBase
{
	override bool SetupAction(PlayerBase player, ActionTarget target, ItemBase item, out ActionData action_data, Param extra_data = NULL)
	{
		if (!super.SetupAction(player, target, item, action_data, extra_data))
			return false;

		if (action_data)
			LogZ_WorldLogger.WithActionData(action_data, true, LogZ_Level.INFO);

		return true;
	}
}
#endif
