{
  "targets": [
    {
      "target_name": "bsdiff",
      "sources": [
        "bsdiff.cc",
        "bsdiff/bsdiff.c",
        "bzip2/bzlib.c",
        "bzip2/compress.c",
        "bzip2/crctable.c",
        "bzip2/randtable.c",
        "bzip2/blocksort.c",
        "bzip2/huffman.c",
        "bzip2/decompress.c"
      ],
      "include_dirs": [
        "include", "./bzip2"
      ] 
    }
  ]
}