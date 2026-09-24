# ProcessPilot

**ProcessPilot** is a Linux-based service manager and process supervisor built using modern C++.

It is designed to manage background services, monitor their health, handle service dependencies, detect unexpected process failures, and automatically restart services according to their configured restart policy.

## Features

* Start, stop, restart, and check the status of services
* Continuous process supervision
* Automatic recovery after unexpected crashes
* Service dependency management
* CPU and memory resource monitoring
* Linux signal handling
* Process logging
* Unix socket based IPC
* Configuration-based service management
* Sample services for testing and demonstration

## Project Structure

```text
processpilot/
│
├── src/
│   ├── daemon/
│   ├── process/
│   ├── config/
│   ├── monitor/
│   ├── dependency/
│   ├── ipc/
│   └── logging/
│
├── cli/
│   └── processpilot_cli.cpp
│
├── examples/
│   ├── sample_service.cpp
│   └── crash_service.cpp
│
├── tests/
│   ├── process_tests.cpp
│   ├── config_tests.cpp
│   └── dependency_tests.cpp
│
├── configs/
│   └── demo.service
│
├── docs/
│   ├── architecture.md
│   └── requirements.md
│
├── CMakeLists.txt
├── README.md
└── .gitignore
```

## Technologies

* C++
* Linux
* CMake
* POSIX processes and signals
* Unix Domain Sockets
* Linux `/proc` system information
* Git and GitHub

## Build

Clone the repository and enter the project directory:

```bash
git clone https://github.com/Ruman-Fatma/processpilot.git
cd processpilot
```

Create the build directory:

```bash
cmake -S . -B build
```

Build the project:

```bash
cmake --build build -j$(nproc)
```

## Testing

Run the tests using:

```bash
ctest --test-dir build --output-on-failure
```

## Running ProcessPilot

Start the ProcessPilot daemon:

```bash
./build/processpilot configs /tmp/processpilot.sock
```

In another terminal, use the CLI:

```bash
./build/processpilot-cli --socket /tmp/processpilot.sock status
```

Start a service:

```bash
./build/processpilot-cli --socket /tmp/processpilot.sock start sample
```

Check service status:

```bash
./build/processpilot-cli --socket /tmp/processpilot.sock status
```

Stop a service:

```bash
./build/processpilot-cli --socket /tmp/processpilot.sock stop sample
```

## Crash Recovery Demonstration

The `examples/` directory contains a sample service and a crash service.

The crash service intentionally exits with a non-zero status so that ProcessPilot can demonstrate unexpected process termination and automatic recovery.

## Project Goal

The goal of ProcessPilot is to demonstrate how Linux service supervision works using operating-system concepts such as:

* Process creation and management
* Signals
* Inter-process communication
* Process monitoring
* Resource monitoring
* Dependency management
* Automatic process recovery

## Author

**Ruman Fatma**

Computer Science & Information Technology
ITER University, Bhubaneswar
