# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog][],
and this project adheres to [Semantic Versioning][].

<!--
## Unreleased

### Added
### Changed
### Removed
-->

## Unreleased

### Added

* buffered file writing system; logs are now accumulated in memory and
  flushed to disk based on size or time interval to reduce disk I/O overhead
* `buffer_size` and `flush_interval` options to control the buffering behavior
* metric **`dayz_metricz_logz_disk_flushes_total_total`** (`COUNTER`) —
  Total number of buffer flushes to disk
* metric **`dayz_metricz_logz_disk_written_bytes_total_total`** (`COUNTER`) —
  Total size of logs written to disk in bytes
* new options `settings.instance_id` and `settings.host_name` for overriding
  autodetected values

### Changed

* migrate configuration to `$profile:logz/config.json` file
* migrate logs storage path to `$profile:logz/logs/`
* log filenames now include `instance_id` by default (e.g., `logz_1.ndjson`)

## [0.1.0][] - 2025-11-23

### Added

* First public release

[0.1.0]: https://github.com/WoozyMasta/logz/tree/0.1.0

<!--links-->
[Keep a Changelog]: https://keepachangelog.com/en/1.1.0/
[Semantic Versioning]: https://semver.org/spec/v2.0.0.html
