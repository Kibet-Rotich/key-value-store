# Key-Value Store in C

A lightweight in-memory key-value store implemented in C, with a TCP server and Redis-like text commands.

## Features

- Hash table storage with separate chaining for collisions
- `SET`, `GET`, and `DEL` command support
- Quoted-token parsing (supports values with spaces)
- Multi-threaded TCP server (thread-per-client)
- Per-client request loop (clients can send multiple commands on one connection)
- Mutex-protected store operations for thread-safe access (`SET`, `GET`, `DEL`)
- Concurrency stress and integrity test scripts
- Simple benchmark utility for insert/get performance

## Project Structure

- `kv_store.c`, `kv_store.h` — core hash table implementation
- `parser.c`, `parser.h` — command tokenizer and command execution
- `server.c` — threaded TCP server using the store + parser
- `run.sh` — build-and-run helper for the server
- `test.c` — functional test for the hash table API
- `test parser.c` — functional test for command parsing/execution
- `benchmark.c` — insert/get micro-benchmark
- `test.py` — Python socket client test
- `stress_test.py` — concurrent stress/load test
- `integrity_test.py` — concurrent correctness/integrity test
- `kv_server` — prebuilt server binary

## Requirements

- Linux (or any POSIX-like OS)
- `gcc`
- `cmake`
- `python3` (for Python test scripts)

## Build

Recommended (from project root):

```bash
cmake -S . -B build
cmake --build build --target kv_server
```

Or use the helper script:

```bash
./run.sh
```

Manual `gcc` build (alternative):

```bash
gcc -Wall -Wextra -std=c11 server.c kv_store.c parser.c -pthread -o kv_server
```

Build test and benchmark binaries:

```bash
gcc -Wall -Wextra -std=c11 test.c kv_store.c -o test_kv
gcc -Wall -Wextra -std=c11 "test parser.c" parser.c kv_store.c -o test_parser
gcc -O2 -std=c11 benchmark.c kv_store.c -o benchmark
```

## Run the Server

Using CMake output:

```bash
./build/kv_server
```

Or build + run in one command:

```bash
./run.sh
```

Server output:

- `KV Store Server started on port 8080`
- `Waiting for connection...`

Each new client gets a worker thread. A connected client can issue multiple commands until it disconnects.

## Command Protocol

Supported commands:

```text
SET <key> <value>
GET <key>
DEL <key>
```

Behavior notes:

- Server sends a banner immediately on connect: `KV Store Server started on port 8080`
- Commands are line-based text requests over TCP
- A single TCP connection may carry multiple commands

Responses:

- `SET` success: `OK`
- `GET` found: `<value>`
- `GET` missing: `(nil)`
- `DEL` found: `(integer) 1`
- `DEL` missing: `(integer) 0`
- Unknown/invalid syntax: `-ERR ...`

### Quoted Values

Values can be quoted to include spaces:

```text
SET greeting "hello world"
GET greeting
```

## Quick Manual Test (with netcat)

Start server in one terminal, then run in another terminal:

```bash
printf "SET name Alice\nGET name\nDEL name\n" | nc 127.0.0.1 8080
```

Expected responses include the banner, then `OK`, `Alice`, and `(integer) 1`.

## Run Tests

Start the server first (`./run.sh` or `./build/kv_server`), then run tests from another terminal.

Hash table test:

```bash
./test_kv
```

Parser/command test:

```bash
./test_parser
```

Benchmark:

```bash
./benchmark
```

Python functional socket test:

```bash
python3 test.py
```

Concurrency stress test:

```bash
python3 stress_test.py
```

Integrity stress test:

```bash
python3 integrity_test.py
```

## Known Issues / Current Limitations

- In-memory only (no persistence to disk)
- Fixed port (`8080`) compiled into `server.c`
- No authentication or encryption
- No durable logging / audit trail yet

## Next Improvements

- Add configurable host/port via CLI flags
- Add graceful shutdown and coordinated thread cleanup
- Add persistence to disk (append-only log and snapshots)
- Add recovery from persisted state on startup
- Add authentication and transport encryption
- Add structured logging and metrics
- Add automated tests (e.g., `ctest` or lightweight assertion framework)
