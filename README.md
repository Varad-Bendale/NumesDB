 _   _                               _  _     
| \ | |_   _ _ __ ___   ___  ___  __| || |__  
|  \| | | | | '_ ` _ \ / _ \/ __|/ _` || '_ \ 
| |\  | |_| | | | | | |  __/\__ \ (_| || |_) |
|_| \_|\__,_|_| |_| |_|\___||___/\__,_||_.__/

## Overview
Numes_db is a relational database engine that reads and writes directly to a file on disk — the same approach SQLite uses — 
built completely in C.

Databases run the entire tech industry, yet almost no one knows how this wonderful piece of software actually works. This is that 
black box, rebuilt in C, piece by piece.


## Numes_db supports
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

## Architecture Overview  
+----------------------------------------------------------------------+
|                          PREPROCESSING                               |
|                                                                      |
|   +---------------+        +----------------------+                  |
|   |   Tokenizer   |  --->  |   Parser (-> AST)    |                  |
|   +---------------+        +----------+-----------+                  |
+---------------------------------------|------------------------------+
                                        |
                                        v
+-----------------------------------------------------------------------+
|                         EXECUTION ENGINE                              |
|                                                                       |
|      +----------------------+                                         |
|      |  Bytecode Generator  |                                         |
|      +-----------+----------+                                         |
|                  |                                                    |
|                  v                                                    |
|      +----------------------+        +------------------------+       |
|      |    Virtual Machine   | <----> |   WHERE Clause Engine  |       |
|      +-----------+----------+        +------------------------+       |
|                  |                                                    |
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



## Repository Structure
Numes_db/
├── README.md                  # Project documentation
├── sql.c                      # Entire code 
│
├── Parser/                    
│   ├── select_parser.c        # Tokenizing + parsing Select queries into AST
│   └── Parser.txt             # Parser reference , design notes
│
└── Engine/                 
     ├── Engine.c               # Converting the queries to the bytecodes functions 
     ├── bytecode.c             # All bytecode instruction definitions
     └── engine.txt             # Opcode reference, VM design notes



## Prerequisites
gcc (or clang ) — C compiler
A Linux environment (Linux/macOS) — file I/O assumes POSIX-style paths


## Status 
Work in progress currently supports : 
[X] Repl
[X] Tokenizer
[X] Select queries parser 
[X] Bytecodes 
[X] Compilation of ( Select , From , Where , Groupby + having , Orderby ) 
[ ] Create queries parser 
[ ] Compilation of ( Case , limit , Join ) 
[ ] Where clause engine 
[ ] Pager
[ ] Btrees ( storage engine ) 
  






