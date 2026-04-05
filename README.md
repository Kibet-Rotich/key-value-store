# Key-Value Store in C

A lightweight in-memory key-value store implemented in C, with a simple TCP server and Redis-like text commands.

## Features

- Hash table storage with separate chaining for collisions
- `SET`, `GET`, and `DEL` command support
- Quoted-token parsing (supports values with spaces)
- Single-binary TCP server (listens on port `8080`)
- Standalone test programs for store logic and parser behavior
- Simple benchmark utility for insert/get performance

## Project Structure

- `kv_store.c`, `kv_store.h` — core hash table implementation
- `parser.c`, `parser.h` — command tokenizer and command execution
- `server.c` — TCP server using the store + parser
- `test.c` — functional test for the hash table API
- `test parser.c` — functional test for command parsing/execution
- `benchmark.c` — insert/get micro-benchmark
- `test.py` — Python socket client script (see Known Issues)
- `kv_server` — prebuilt server binary

## Requirements

- Linux (or any POSIX-like OS)
- `gcc`
- `make` is optional (commands below use `gcc` directly)

## Build

From the project root:

```bash
gcc -Wall -Wextra -std=c11 server.c kv_store.c parser.c -o kv_server
```

Build test and benchmark binaries:

```bash
gcc -Wall -Wextra -std=c11 test.c kv_store.c -o test_kv
gcc -Wall -Wextra -std=c11 "test parser.c" parser.c kv_store.c -o test_parser
gcc -O2 -std=c11 benchmark.c kv_store.c -o benchmark
```

## Run the Server

```bash
./kv_server
```

Server output:

- `KV Store Server started on port 8080`
- `Waiting for connection...`

The server processes **one command per TCP connection** and replies with plain text.

## Command Protocol

Supported commands:

```text
SET <key> <value>
GET <key>
DEL <key>
```

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

Start server in one terminal, then run each command in another terminal:

```bash
printf "SET name Alice\n" | nc 127.0.0.1 8080
printf "GET name\n" | nc 127.0.0.1 8080
printf "DEL name\n" | nc 127.0.0.1 8080
```

Expected responses are `OK`, `Alice`, and `(integer) 1` respectively.

## Run Tests

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

## Known Issues / Current Limitations

- In-memory only (no persistence to disk)
- Single-threaded server
- Fixed port (`8080`) compiled into `server.c`
- No authentication/encryption
- The server sends a short banner on connect so clients that read a greeting first can proceed

## Next Improvements

- Add configurable host/port via CLI flags
- Add request loop per client connection (instead of one command per connection)
- Add graceful shutdown and signal handling
- Add persistence (append-only log or snapshot)
- Add automated tests (e.g., `ctest` or lightweight assertion framework)
