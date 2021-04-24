# Mash

This is my mashing algorithm app. Gets an input (file, or stdin), and outs a mashed input (in a file or stdout).

```shell
./mash [input file]
```

## Procedure:
 1. Reads the file.
 2. Creates chunks of 32 bytes of it.
 3. XORs each chunk with random characters.
 4. XORs each two chunks with each other and replaces them with result.
 5. Repeats `4` until 1 chunk remains.
 6. Prints.

