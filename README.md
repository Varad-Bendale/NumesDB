## Numes_db

Numes_db is a relational database engine that reads and writes directly
to a file on disk — the same approach SQLite uses — built completely in C.

Databases run the entire tech industry, yet almost no one knows how this
wonderful piece of software actually works. This is that black box,
rebuilt in C, piece by piece.

## Numes_db Supports

```
SELECT [columns | CASE ... END]
  │
  ├── FROM <table>
  │     └── JOIN <table> ON <condition>
  │
  ├── WHERE <condition>
  │
  ├── GROUP BY <columns>
  │     └── HAVING <condition>
  │
  ├── ORDER BY <columns> [ASC|DESC]
  │
  └── LIMIT <n>
```

❌ No subqueries / nested SELECTs — every query is single-pass, no nesting.

## Architecture Overview

```
+----------------------------------------------------------------------+
|                          PREPROCESSING                               |
|                                                                      |
|   +---------------+        +----------------------+                  |
|   |   Tokenizer   |  --->  |   Parser (-> AST)    |                  |
|   +---------------+        +----------+-----------+                  |
+---------------------------------------|--------------------------------+
                                        v
+----------------------------------------------------------------------+
|                         EXECUTION ENGINE                             |
|                                                                      |
|      +----------------------+                                        |
|      |  Bytecode Generator  |                                        |
|      +-----------+----------+                                        |
|                  |                                                   |
|                  v                                                   |
|      +----------------------+        +------------------------+      |
|      |    Virtual Machine   | <----> |   WHERE Clause Engine  |      |
|      +-----------+----------+        +------------------------+      |
|                  |                                                   |
+------------------|----------------------------------------------------+
                   | Read/Write rows
                   v
+----------------------------------------------------------------------+
|                          STORAGE ENGINE                              |
|                                                                      |
|   +---------------+        +---------------+        +-----------+    |
|   |     Pager     | <----> |    B-Tree     | <----> |  .db file |    |
|   +---------------+        +---------------+        +-----------+    |
+----------------------------------------------------------------------+
```

## Repository Structure

```
Numes_db/
├── README.md                  # Project documentation
├── sql.c                      # Entire code
│
├── Parser/
│   ├── select_parser.c        # Tokenizing + parsing SELECT queries into AST
│   └── Parser.txt             # Parser reference, design notes
│
└── Engine/
    ├── Engine.c               # Converts queries into bytecode
    ├── bytecode.c             # All bytecode instruction definitions
    └── engine.txt             # Opcode reference, VM design notes
```

## Prerequisites

* `gcc` (or `clang`) — C compiler
* A Linux/macOS environment — file I/O assumes POSIX-style paths

## Status

 Work in progress. Currently supports:

- [x] REPL
- [x] Tokenizer
- [x] SELECT query parser
- [x] Bytecode generation
- [x] Compilation of SELECT, FROM, WHERE, GROUP BY + HAVING, ORDER BY
- [ ] CREATE query parser
- [ ] Compilation of CASE, LIMIT, JOIN
- [ ] WHERE clause engine
- [ ] Pager
- [ ] B-Tree (storage engine)

