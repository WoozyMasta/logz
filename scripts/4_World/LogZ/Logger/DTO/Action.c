/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright (c) 2025 WoozyMasta
    Source: https://github.com/woozymasta/logz
*/

#ifdef SERVER
/**
    \brief Serializable view of ActionData for action logging.
*/
class LogZ_DTO_ActionData
{
	string action;
	int action_id;
	string recipe;
	int recipe_id;
	// int variant_id;
	int condition_mask;
	int state;
	// bool started;
	// bool executed;
	// bool stopped;

	/**
	    \brief Extract DTO from ActionData instance.
	    \param action_data Source ActionData.
	*/
	void LogZ_DTO_ActionData(ActionData action_data)
	{
		if (!action_data)
			return;

		if (action_data.m_Action) {
			action = action_data.m_Action.ClassName();
			action_id = action_data.m_Action.GetID();
			// variant_id = action_data.m_Action.GetVariantID();
			condition_mask = action_data.m_Action.m_ConditionMask;
		}

		WorldCraftActionData action_data_wc;
		if (Class.CastTo(action_data_wc, action_data)) {
			recipe_id = action_data_wc.m_RecipeID;

			PluginRecipesManager prm = PluginRecipesManager.Cast(GetPlugin(PluginRecipesManager));
			if (prm && recipe_id < prm.m_RecipeList.Count() && prm.m_RecipeList[recipe_id])
				recipe = prm.m_RecipeList[recipe_id].ClassName();
		}

		state = action_data.m_State;
		// started = action_data.m_WasActionStarted;
		// executed = action_data.m_WasExecuted;
		// stopped = action_data.m_ReciveEndInput;
	}
}
#endif
