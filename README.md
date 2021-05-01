# Mash

#### 256-bit Mashing Algorithm.
very gut.

```shell
./mash [input file] [difficulty]
```

## Mashing Procedure:
    1.1 Reads 64 bytes of the file.
    1.2 Splits it into 2 32-byte chunk.
    1.3 Adds them to Mashtree with key=0.
    1.4 Checks the whole tree for chunks with same keys, removes them from the tree, mashes them with each other, adds the mashed chunk to the tree with key + 1.
    2. Repeat step `1` until no chunk with identical keys remain.
    2.1 Then, mashes chunk 0 with all the other remaining chunks, no key testing.
    3. Then the program mashes the chunk 0 with random characters, and then with itself.
    4. If difficulty was given, it checks the mashed chunk with the difficulty, otherwise continues mashing.

Maybe using it as proof of work? :)))
