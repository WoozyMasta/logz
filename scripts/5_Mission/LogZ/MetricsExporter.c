/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright (c) 2025 WoozyMasta
    Source: https://github.com/woozymasta/logz
*/

#ifdef SERVER
#ifdef METRICZ
modded class MetricZ_Exporter
{
	override bool Flush(FileHandle fh)
	{
		if (!super.Flush(fh))
			return false;

		LogZ.FlushMetrics(fh);

		return true;
	}
}
#endif
#endif
