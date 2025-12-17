# MetricZ Scraped Metrics Reference

> Automatically generated list of metrics from the source code.

This document lists all metrics exposed by the **MetricZ** mod for
DayZ server. Each metric includes its identifier, type
(`GAUGE` or `COUNTER`), and description as defined in the source code.

## [./scripts/3_Game/LogZ/Logger/Log.c](./scripts/3_Game/LogZ/Logger/Log.c)

* **`dayz_metricz_logz_processing_seconds_total_total`** (`COUNTER`) —
  Total cumulative time in seconds spent collecting and writing logs
* **`dayz_metricz_logz_disk_flushes_total_total`** (`COUNTER`) —
  Total number of buffer flushes to disk
* **`dayz_metricz_logz_disk_written_bytes_total_total`** (`COUNTER`) —
  Total size of logs written to disk in bytes
* **`dayz_metricz_logz_events_total`** (`COUNTER`) —
  Total events logged with LogZ by level
