# ProcessPilot Requirements

- C++17 Linux application.
- CLI commands: start, stop, restart, status/list, ping.
- Background process creation with fork/exec and termination with signals.
- Automatic recovery using configurable restart policy.
- Dependency-aware startup and cycle detection.
- CPU and memory reporting from /proc.
- Unix-domain socket IPC between CLI and daemon.
- Signal handling for daemon shutdown and service termination.
- Persistent daemon and service logs.
