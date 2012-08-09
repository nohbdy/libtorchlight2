#ifndef LIBTORCHLIGHT2_FILE_H_
#define LIBTORCHLIGHT2_FILE_H_

#include <stdint.h>
#include <string>
#include <memory>
#include <vector>

namespace torchlight2 {

enum FileType : uint8_t {
	FileType_Dat = 0, // Also .TEMPLATE
	FileType_Layout = 1,
	FileType_Mesh = 2,
	FileType_Skeleton = 3,
	FileType_DDS = 4,
	FileType_PNG = 5,
	FileType_Sound = 6,
	FileType_Directory = 7,
	FileType_Material = 8,
	FileType_Raw = 9,
	FileType_ImageSet = 11,
	FileType_TTF = 12,
	FileType_Font = 13,
	FileType_HIE = 17,
	FileType_Animation = 16,
	FileType_Scheme = 19,
	FileType_LookNFeel = 20,
	FileType_MPD = 21,
	FileType_Unknown = 0xFF
};

class File;
class Directory;

typedef std::unique_ptr<File> FilePtr;

class File {
public:
	static FilePtr Read(Directory* parent_dir, std::ifstream& pakman);

	const std::string& name() const { return name_; }

	std::string path() const;

	uint32_t file_offset() const { return file_offset_; }

	uint32_t size() const { return uncompressed_size_; }

	// uint32_t compressed_size() const { return compressed_size_; }

	FileType file_type() const { return file_type_; }

	Directory& directory() const { return *directory_; }

	std::vector<uint8_t> data() const;

private:
	Directory* directory_;

	std::string name_;

	uint32_t file_offset_;

	uint32_t uncompressed_size_;

	uint32_t compressed_size_;

	FileType file_type_;
};
} // namespace torchlight2

#endif
