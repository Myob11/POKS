# DN 3 Case – Multithreaded UUID/CRC Server

This lab implements a threaded TCP server that hands out sequential UUID-based serial numbers with CRC32 protection to multiple clients. The protocol mirrors the requirements in `dn_3-case_solving.txt`.

## Files
- server.c – threaded TCP server listening on port 5095
- client.c – interactive client targeting 127.0.0.1:5095
- dn_3-case_solving.txt – original assignment brief

## Build
Use GCC on Linux (pthread is required by the server):

```bash
gcc server.c -o server -pthread
gcc client.c -o client
```

## Run
1) Start the server (binds to all interfaces on port 5095):
```bash
./server
```

2) In another terminal start a client (connects to 127.0.0.1:5095):
```bash
./client
```

3) At the client prompt, type commands (see protocol below). Type `X` to close the client.

To use a remote server, update the hard-coded address/port in client.c.

## Protocol (server ↔ client)
- `GET` → Server replies with: `<seq>-<UUID> <CRC32>`
  - `seq` starts at 00000001 and increments only after a valid acknowledgment.
  - `UUID` format: 8-4-4-4-12 lowercase hex (version 4, variant 9 in the provided code).
  - `CRC32` is computed over the UUID string (8 hex digits, uppercase).
- Client checks CRC32 over the received UUID:
  - If valid, client sends `PREJETO <CRC32>` using the same CRC it calculated.
  - If invalid, client still sends `PREJETO <its_CRC>` but will log the mismatch locally.
- Server handling of the acknowledgment:
  - If the CRC matches what the server computed, it increments the global serial counter.
  - If the CRC does not match, it replies `NAPAKA 4900B4DB` and does **not** increment the counter.
- Any other client message → Server replies `NEPREPOZNAVEN UKAZ`.

## Concurrency rules
- Each client connection runs in its own thread.
- Only one `GET` is processed at a time. While a thread is waiting for the `PREJETO ...` reply, other threads block before issuing their own serial/UUID response.

## Example flow
```
Client: GET
Server: 00000001-xxxxxxxx-xxxx-Mxxx-Nxxx-xxxxxxxxxxxx 86E0E903
Client validates CRC32 over the UUID
Client: PREJETO 86E0E903
Server increments its serial counter
```

## Notes
- CRC32 implementation is included in both server and client; no external libraries are needed beyond pthread for threading.
- The port (5095) and address (127.0.0.1 in the client) are hard-coded; adjust in source if needed.
