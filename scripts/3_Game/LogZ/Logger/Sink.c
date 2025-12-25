/*
    SPDX-License-Identifier: GPL-3.0-or-later
    Copyright (c) 2025 WoozyMasta
    Source: https://github.com/woozymasta/logz
*/

#ifdef SERVER
/**
    \brief Manages log output to file with buffering and time-based flushing.
*/
class LogZ_Sink
{
	private static ref LogZ_Sink s_Instance;

	private FileHandle m_FH;
	private ref array<string> m_Buffer;
	private int m_BufferSize;
	private int m_FlushIntervalMs;
	private int m_LastFlushTime;
	private bool m_IsDirect;
	private bool m_IsShutdown;

	/**
	    \brief Constructor
	*/
	void LogZ_Sink()
	{
		m_Buffer = new array<string>();
	}

	/**
	    \brief Destructor
	*/
	void ~LogZ_Sink()
	{
		Close();
	}

	/**
	    \brief Singleton instance.
	*/
	static LogZ_Sink Get()
	{
		if (!s_Instance)
			s_Instance = new LogZ_Sink();

		return s_Instance;
	}

	/**
	    \brief Initialize the sink using global configuration.
	*/
	void Init()
	{
		if (m_FH)
			Close();

		if (!LogZ_Config.IsLoaded()) {
			ErrorEx("LogZ: Config not loaded, sink init failed", ErrorExSeverity.ERROR);
			return;
		}

		LogZ_ConfigDTO_File cfgFile = LogZ_Config.Get().file;

		// Setup
		m_BufferSize = cfgFile.buffer_size;
		m_FlushIntervalMs = cfgFile.flush_interval * 1000;
		m_IsDirect = (m_BufferSize <= 0);
		m_IsShutdown = false;
		m_LastFlushTime = g_Game.GetTime();

		// Open file
		FileMode mode = FileMode.WRITE;
		if (cfgFile.append)
			mode = FileMode.APPEND;

		m_FH = OpenFile(cfgFile.full_path, mode);
		if (!m_FH) {
			ErrorEx("LogZ: Failed to open log file: " + cfgFile.full_path, ErrorExSeverity.ERROR);
			return;
		}

		// Setup buffer and timer
		if (!m_IsDirect) {
			m_Buffer.Reserve(m_BufferSize);
			g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(OnTimerTick, m_FlushIntervalMs, true);
		}
	}

	/**
	    \brief Accepts a raw JSON line string.
	*/
	void Write(string line)
	{
		if (!m_FH)
			return;

		if (m_IsDirect || m_IsShutdown) {
			FPrintln(m_FH, line);

#ifdef METRICZ
			LogZ.MetricBytesAdd(line.Length());
#endif

			return;
		}

		m_Buffer.Insert(line);

		if (m_Buffer.Count() >= m_BufferSize) {
#ifdef DIAG
			ErrorEx("LogZ: [Sink] Flush triggered by BUFFER LIMIT (" + m_Buffer.Count() + "/" + m_BufferSize + ")", ErrorExSeverity.INFO);
#endif
			Flush();
		}
	}

	/**
	    \brief Forces a write of the current buffer to disk.
	*/
	void Flush()
	{
		// Reset timer even if empty to prevent double-checks
		m_LastFlushTime = g_Game.GetTime();

		if (!m_FH || m_Buffer.Count() == 0)
			return;

		string chunk = "";
		int count = m_Buffer.Count();

		for (int i = 0; i < count; ++i)
			chunk += string.Format("%1\n", m_Buffer[i]);

		FPrint(m_FH, chunk);

#ifdef METRICZ
		LogZ.MetricBytesAdd(chunk.Length());
		LogZ.MetricFlushesInc();
#endif

		m_Buffer.Clear();
	}

	/**
	    \brief Background timer callback.
	*/
	private void OnTimerTick()
	{
		if (m_IsDirect || m_IsShutdown || !m_FH)
			return;

		if ((g_Game.GetTime() - m_LastFlushTime) >= m_FlushIntervalMs) {
			if (m_Buffer.Count() > 0) {
				Flush();
#ifdef DIAG
				ErrorEx("LogZ: [Sink] Flush triggered by TIMER (" + (g_Game.GetTime() - m_LastFlushTime) + "ms)", ErrorExSeverity.INFO);
#endif
			} else
				m_LastFlushTime = g_Game.GetTime();
		}
	}

	/**
	    \brief Handles server shutdown sequence.
	*/
	void Shutdown()
	{
		if (m_IsShutdown)
			return;

		Flush();
		g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(OnTimerTick);
		m_IsShutdown = true;

#ifdef DIAG
		ErrorEx("LogZ: [Sink] Flush triggered by SHUTDOWN", ErrorExSeverity.INFO);
#endif
	}

	/**
	    \brief Closes file handle.
	*/
	void Close()
	{
		Shutdown();

		if (m_FH) {
			CloseFile(m_FH);
			m_FH = null;
		}
	}
}
#endif
