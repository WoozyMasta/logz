/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright (c) 2025 WoozyMasta
    Source: https://github.com/woozymasta/logz
*/

#ifdef SERVER
/**
    \brief Structured logger writing NDJSON lines to profile file.
*/
class LogZ
{
	private static FileHandle s_FH;
	private static ref JsonSerializer s_JS;

#ifdef METRICZ
	// logging time spent
	protected static ref MetricZ_MetricFloat s_MetricLogTime;
	// count log events by level
	protected static ref MetricZ_MetricInt s_MetricTrace;
	protected static ref MetricZ_MetricInt s_MetricDebug;
	protected static ref MetricZ_MetricInt s_MetricInfo;
	protected static ref MetricZ_MetricInt s_MetricWarn;
	protected static ref MetricZ_MetricInt s_MetricError;
	protected static ref MetricZ_MetricInt s_MetricFatal;
	// metrics registry
	protected static ref array<ref MetricZ_MetricBase> s_MetricsRegistry = new array<ref MetricZ_MetricBase>();
#endif

	/**
	    \brief Initialize logger from config.
	*/
	static void Init()
	{
		if (!LogZ_Config.IsLoaded())
			LogZ_Config.Get();

		if (s_FH)
			CloseFile(s_FH);

		s_FH = OpenFile(LogZ_Config.Get().file.full_path, FileMode.APPEND);

		if (!s_JS)
			s_JS = new JsonSerializer();

#ifdef METRICZ
		InitMetrics();
#endif
	}

	/**
	    \brief Access shared JsonSerializer instance.
	*/
	static JsonSerializer GetSerializer()
	{
		return s_JS;
	}

	/**
	    \brief Emit a log line if level is allowed.
	    \details Serializes LogZ_DTO_Root and merges "extra" map as additional JSON fields.
	           Values that look like JSON are inserted raw, otherwise quoted and escaped.
	    \warning Serializer lacks polymorphism and cannot merge object + map<string,string>.
	           Workaround: write base JSON to string, trim "}", append fields:
	           if value looks like JSON insert raw, otherwise escape and quote.
	           Fast and reliable, but the whole flow is a maintenance nightmare.
	           Greetings to the game authors.
	    \param lvl   Severity level.
	    \param msg   Message string.
	    \param extra Optional map of extra fields.
	*/
	static void Log(string msg, LogZ_Level lvl, LogZ_Event eventType = 0, map<string, string> extra = null)
	{
		if (!s_FH || !s_JS || !LogZ_Levels.IsEnabled(lvl) || !LogZ_Events.IsEnabled(eventType))
			return;

#ifdef METRICZ
		float t0 = g_Game.GetTickTime();
#endif

		LogZ_DTO_Root base = new LogZ_DTO_Root(lvl, msg, eventType);

		string result;
		if (!s_JS.WriteToString(base, false, result))
			return;

		if (extra && extra.Count() > 0) {
			// drop trailing "}"
			result = result.Substring(0, result.Length() - 1);

			foreach (string key, string value : extra) {
				if (key == string.Empty || value == string.Empty)
					continue;

				if (LogZ_Json.IsValue(value))
					result += ",\"" + LogZ_Json.Escape(key) + "\":" + value;
				else
					result += ",\"" + LogZ_Json.Escape(key) + "\":\"" + LogZ_Json.Escape(value) + "\"";
			}

			result += "}";
		}

		// TODO (array<string>) buffer
		FPrintln(s_FH, result);

#ifdef METRICZ
		s_MetricLogTime.Add(g_Game.GetTickTime() - t0);

		switch (lvl) {
		case LogZ_Level.TRACE:
			s_MetricTrace.Inc();
			break;

		case LogZ_Level.DEBUG:
			s_MetricDebug.Inc();
			break;

		case LogZ_Level.INFO:
			s_MetricInfo.Inc();
			break;

		case LogZ_Level.WARN:
			s_MetricWarn.Inc();
			break;

		case LogZ_Level.ERROR:
			s_MetricError.Inc();
			break;

		case LogZ_Level.FATAL:
			s_MetricFatal.Inc();
			break;
		}
#endif
	}

	/**
	     \brief Shorthand for TRACE.
	*/
	static void Trace(string msg, LogZ_Event eventType = 0, map<string, string> extra = null)
	{
		Log(msg, LogZ_Level.TRACE, eventType, extra);
	}

	/**
	     \brief Shorthand for DEBUG.
	*/
	static void Debug(string msg, LogZ_Event eventType = 0, map<string, string> extra = null)
	{
		Log(msg, LogZ_Level.DEBUG, eventType, extra);
	}

	/**
	     \brief Shorthand for INFO.
	*/
	static void Info(string msg, LogZ_Event eventType = 0, map<string, string> extra = null)
	{
		Log(msg, LogZ_Level.INFO, eventType, extra);
	}

	/**
	     \brief Shorthand for WARN.
	*/
	static void Warn(string msg, LogZ_Event eventType = 0, map<string, string> extra = null)
	{
		Log(msg, LogZ_Level.WARN, eventType, extra);
	}

	/**
	    \brief Shorthand for ERROR.
	*/
	static void Error(string msg, LogZ_Event eventType = 0, map<string, string> extra = null)
	{
		Log(msg, LogZ_Level.ERROR, eventType, extra);
	}

	/**
	    \brief Shorthand for FATAL.
	*/
	static void Fatal(string msg, LogZ_Event eventType = 0, map<string, string> extra = null)
	{
		Log(msg, LogZ_Level.FATAL, eventType, extra);
	}

	/**
	    \brief Close file handle and log shutdown marker.
	*/
	static void Close()
	{
		if (!s_FH)
			return;

		CloseFile(s_FH);
		s_FH = null;

		ErrorEx("LogZ stopped", ErrorExSeverity.INFO);
	}

#ifdef METRICZ
	/**
	    \brief Flush all registered LogZ metrics into given file.
	    \param fh Open file handle for metrics output.
	*/
	static void FlushMetrics(MetricZ_SinkBase sink)
	{
		if (s_MetricsRegistry.Count() < 1)
			return;

		foreach (MetricZ_MetricBase metric : s_MetricsRegistry) {
			if (metric == s_MetricLogTime || metric == s_MetricTrace)
				metric.FlushWithHead(sink);
			else
				metric.Flush(sink);
		}
	}

	/**
	    \brief Create and register LogZ metrics in private registry.
	    \details Metrics created once; subsequent calls are no-op.
	*/
	protected static void InitMetrics()
	{
		if (s_MetricsRegistry.Count() > 0)
			return;

		s_MetricLogTime = new MetricZ_MetricFloat(
		    "logz_processing_seconds_total",
		    "Total cumulative time in seconds spent collecting and writing logs",
		    MetricZ_MetricType.COUNTER);
		s_MetricsRegistry.Insert(s_MetricLogTime);

		s_MetricTrace = NewLogMetric(LogZ_Level.TRACE);
		s_MetricsRegistry.Insert(s_MetricTrace);

		s_MetricDebug = NewLogMetric(LogZ_Level.DEBUG);
		s_MetricsRegistry.Insert(s_MetricDebug);

		s_MetricInfo = NewLogMetric(LogZ_Level.INFO);
		s_MetricsRegistry.Insert(s_MetricInfo);

		s_MetricWarn = NewLogMetric(LogZ_Level.WARN);
		s_MetricsRegistry.Insert(s_MetricWarn);

		s_MetricError = NewLogMetric(LogZ_Level.ERROR);
		s_MetricsRegistry.Insert(s_MetricError);

		s_MetricFatal = NewLogMetric(LogZ_Level.FATAL);
		s_MetricsRegistry.Insert(s_MetricFatal);
	}

	/**
	    \brief Factory for log level metrics with static "level" label.
	    \param level LogZ_Level bucket for the metric.
	    \return MetricZ_MetricInt
	*/
	protected static MetricZ_MetricInt NewLogMetric(LogZ_Level level)
	{
		MetricZ_MetricInt metric = new MetricZ_MetricInt(
		    "logz_events",
		    "Total events logged with LogZ by level",
		    MetricZ_MetricType.COUNTER);

		string labelValue = EnumTools.EnumToString(LogZ_Level, level);
		labelValue.ToLower();
		metric.MakeLabel("level", labelValue);

		return metric;
	}
#endif
}
#endif
