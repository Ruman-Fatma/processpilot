# Architecture

`processpilot` is the daemon. It loads `.service` files, owns service processes, supervises them, applies dependency order and restart policy, and serves commands over a Unix-domain socket.

`processpilot-cli` is a thin client. It sends one command per connection.

Process supervision uses `waitpid(..., WNOHANG)` in the daemon loop. Resource monitoring reads `/proc/<pid>/stat` and `/proc/<pid>/status`.
