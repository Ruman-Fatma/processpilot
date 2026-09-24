# ProcessPilot

A lightweight Linux service manager and process supervisor written in modern C++17.

## Build

```bash
cmake -S . -B build
cmake --build build -j$(nproc)
ctest --test-dir build --output-on-failure
```

## Run demo

From the project root:

```bash
export PATH="$PWD/build:$PATH"
./build/processpilot configs /tmp/processpilot.sock
```

In another terminal:

```bash
./build/processpilot-cli --socket /tmp/processpilot.sock status
./build/processpilot-cli --socket /tmp/processpilot.sock start backend
./build/processpilot-cli --socket /tmp/processpilot.sock status
./build/processpilot-cli --socket /tmp/processpilot.sock start crash
```

`backend` automatically starts its `database` dependency. `crash` exits after five seconds and is automatically restarted according to its policy.

## Notes

The demo service commands are intentionally relative to the project root. Run the daemon from the project root so `./sample_server` and `./crash_service` can be resolved after building, or change `command=` to absolute paths.
