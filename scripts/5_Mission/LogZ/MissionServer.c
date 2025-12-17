/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright (c) 2025 WoozyMasta
    Source: https://github.com/woozymasta/logz
*/

#ifdef SERVER
/**
    \brief Mission hooks that initialize LogZ.
*/
modded class MissionServer
{
	/**
	    \brief Initialize LogZ before base OnInit.
	*/
	override void OnInit()
	{
		LogZ.Init();

		super.OnInit();

#ifdef METRICZ
		MetricZ_Exporter.Register(new MetricZ_Collector_LogZ());
#endif

		// LogZ_Test.Run();
	}

	/**
	    \brief Close LogZ on mission finish after base handler.
	*/
	override void OnMissionFinish()
	{
		LogZ.Close();

		super.OnMissionFinish();
	}

	/**
	    \brief Log player chat messages.
	*/
	override void OnEvent(EventType eventTypeId, Param params)
	{
		if (eventTypeId != ChatMessageEventTypeID) {
			super.OnEvent(eventTypeId, params);
			return;
		}

		ChatMessageEventParams chatParams = ChatMessageEventParams.Cast(params);
		if (chatParams) {
			string msg = string.ToString(chatParams.param3, false, false, false).Trim();
			string sender = string.ToString(chatParams.param2, false, false, false);

			Man man = LogZ_Utils.GetManByName(sender);
			if (man)
				LogZ_GameLogger.WithObject(man, msg, LogZ_Level.INFO, LogZ_Event.PLAYER_CHAT);
			else
				LogZ.Info("chat message sender " + sender + " say: " + msg, LogZ_Event.PLAYER_CHAT);
		}

		super.OnEvent(eventTypeId, params);
	}
}
#endif
