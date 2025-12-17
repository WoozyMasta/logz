/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright (c) 2025 WoozyMasta
    Source: https://github.com/woozymasta/logz
*/

#ifdef SERVER
#ifdef METRICZ
class MetricZ_Collector_LogZ : MetricZ_CollectorBase
{
	override string GetName()
	{
		return "logz";
	}

	override void Flush(MetricZ_SinkBase sink)
	{
		LogZ.FlushMetrics(sink);
	}
}
#endif
#endif
