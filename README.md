# Mash

### 256-bit Mashing Algorithm.
very gut.

```shell
./mash [input file] [difficulty]
```

## Procedure:
 1. Reads the file.
 2. Creates chunks of 32 bytes of it.
 3. XORs each chunk with random characters.
 4. XORs each two chunks with each other and replaces them with result.
 5. Repeats `4` until 1 chunk remains.
 6. Converts the chunk to hexadecimal format.
 7. If any difficulty declared, checks the chunk if the `difficulty` number of the first characters are '0' or not.
 8. If so, prints the chunk and exits, otherwise the program continues.
 9. If no difficulty declared, it just prints the chunk and repeats the process from instruction #3.

Maybe using it as proof of work? :)))
