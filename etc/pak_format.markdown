## Torchlight 2 PAK File Format

---

### Introduction

The PAK file used in Torchlight 2 has a simple format.  There is a small header for the file and then all of the files are stored one after another, each with a short header.

### Archive Header

The archive's header is only four bytes long.

	typedef struct {
		uint32_t unknown;
	} ArchiveHeader;

``unknown`` is 0x00101073 in the May Preview version of the game.  It may be 2 separate 16-bit numbers (0x1073 and 0x10).

### File Data

Immediately following the ``ArchiveHeader`` comes a number of ``FileData``s.  These occur one after another until the end of the file.

	typedef struct {
		uint32_t uncompressed_size;
		uint32_t compressed_size;
		uint8_t data[compressed_size];
	} FileData;

``uncompressed_size`` is the size in bytes of the file after decompression  
``compressed_size`` is the size in bytes of the compressed file  
``data`` is the compressed file data and is ``compressed_size`` bytes in length.  The compressed data is a zlib stream.  