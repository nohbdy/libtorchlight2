## Torchlight 2 PAK.MAN File Format

---

### Introduction

A PAK.MAN file corresponds to the PAK file of the same name.  The PAK.MAN file is used to determine the location of a particular file within a PAK file.

### PakMan Header

The archive's header is only four bytes long.

	typedef struct {
		uint16_t zero;
		uint16_t rootPathLen;
		char16_t rootPath[nameLen];
		uint32_t unknown;
		uint32_t numDirectories;
	} PakManHeader;

``zero`` is a 16-bit integer (equal to 0x0000 in the May preview)
``rootPathLen`` is the length of the rootPath string (in characters)
``rootPath`` is a string containing the path for the 'root' directory
``unknown`` is a 32-bit number of unknown importance
``numDirectories`` is the number of ``Directory`` structures in the PAK.MAN file

### FileInfo

A ``FileInfo`` is a structure that contains information about a file within the PAK archive.

	typedef struct {
		uint32_t unk_0;
		uint8_t fileType;
		uint16_t nameLen;
		char16_t name[nameLen];
		uint32_t offset;
		uint32_t uncompressedSize;
		FILETIME lastModified;
	} FileInfo;

``unk_0`` is 32-bits of data with an unknown purpose
``fileType`` is an 8-bit number specifying what kind of file this ``FileInfo`` references.
``nameLen`` is the length of the file name string (in characters)
``name`` is a string containing the name (and extension if any) for this file
``offset`` is the offset of a ``FileData`` within the associated PAK file that contains the compressed file data
``uncompressedSize`` is the size in bytes of the file after decompression
``lastModified`` is the last time this file was modified, using Windows FILETIME format (64-bits representing 100-nanosecond intervals since Jan 1, 1601)

### Directory

Immediately following the ``PakManHeader`` comes a number of ``Directory`` structures.  These occur one after another until the end of the file.

	typedef struct {
		uint16_t pathLen;
		char16_t path[pathLen];
		uint32_t numFiles;
		FileInfo files[numFiles]
	} FileData;

``pathLen`` is the length of the path string (in characters)
``path`` is a string containing the directory path (e.g. "MEDIA/DUNGEONS/TESTDUNGEONS/")
``numFiles`` is the number of ``FileInfo`` structures for this directory
``files`` is an array of ``numFiles`` ``FileInfo`` structures representing the files and subdirectories under this directory