#!/bin/sh

rm server/server lib/*.o

for src in lib/*.c; do
    gcc "$src" -c -o "${src%.c}.o"
done

gcc server/server.c -o server/server lib/*.o -lcrypto
gcc client/client.c -o client/client lib/*.o -lcrypto
