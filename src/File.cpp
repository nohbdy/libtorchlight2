#include <fstream>
#include <iostream>
#include <cvt/wstring>
#include <codecvt>

#define ZLIB_WINAPI
#include "zlib.h"

#include "File.h"
#include "Directory.h"
#include "PakFile.h"

namespace {
void PrintIfUnknown(uint8_t fileType, const std::string& fileName) {
	if (fileType != torchlight2::FileType_Dat &&
		fileType != torchlight2::FileType_Layout &&
		fileType != torchlight2::FileType_Mesh &&
		fileType != torchlight2::FileType_Skeleton &&
		fileType != torchlight2::FileType_DDS &&
		fileType != torchlight2::FileType_PNG &&
		fileType != torchlight2::FileType_Sound &&
		fileType != torchlight2::FileType_Directory &&
		fileType != torchlight2::FileType_Material &&
		fileType != torchlight2::FileType_Raw &&
		fileType != torchlight2::FileType_ImageSet &&
		fileType != torchlight2::FileType_TTF &&
		fileType != torchlight2::FileType_Font &&
		fileType != torchlight2::FileType_HIE &&
		fileType != torchlight2::FileType_Animation &&
		fileType != torchlight2::FileType_Scheme &&
		fileType != torchlight2::FileType_LookNFeel &&
		fileType != torchlight2::FileType_MPD) {
			std::cerr << "Unknown FileType " << (int)fileType << " for " << fileName << std::endl;
	}
};
}

namespace torchlight2 {

std::string File::path() const {
	return directory_->path() + name_;
}

FilePtr File::Read(Directory* parent_dir, std::ifstream& pakman) {
	FilePtr file(new File());
	file->directory_ = parent_dir;

	uint32_t unk_0;
	uint8_t fileType;
	uint16_t nameLen;
	char16_t* name;
	uint32_t offset;
	uint32_t uncompressedSize;
	uint64_t unk_1;

	// std::cout << "Read File @ " << std::hex << pakman.tellg() << std::dec;

	pakman.read((char*)&unk_0, 4);
	pakman.read((char*)&fileType, 1);
	pakman.read((char*)&nameLen, 2);
	name = new char16_t[nameLen+1];
	pakman.read((char*)name, 2 * nameLen);
	name[nameLen] = 0;
	pakman.read((char*)&offset, 4);
	pakman.read((char*)&uncompressedSize, 4);
	pakman.read((char*)&unk_1, 8);

	// Convert name to utf8
	stdext::cvt::wstring_convert<std::codecvt_utf8_utf16<char16_t>,char16_t> convert;
	file->name_ = convert.to_bytes(name);

#ifdef _DEBUG
	PrintIfUnknown(fileType, file->name_);
#endif

	// std::cout << " FileName = " << file->name_ << std::endl;

	// Copy data
	file->uncompressed_size_ = uncompressedSize;
	file->file_type_ = (FileType)fileType;
	file->file_offset_ = offset;

	return std::move(file);
}

std::vector<uint8_t> File::data() const {
	auto& file = directory().pakfile().pakfile();

	file.seekg(file_offset_);

	uint32_t uncompressed_size;
	uint32_t compressed_size;

	file.read((char*)&uncompressed_size, 4);
	file.read((char*)&compressed_size, 4);

	if (uncompressed_size != this->uncompressed_size_) {
		std::cerr << "Uncompressed size mismatch for " << this->path() << "! PAK = " << uncompressed_size << " && PAK.MAN = " << this->uncompressed_size_ << std::endl;
		return std::vector<uint8_t>();
	}

	uint8_t* compressedBuffer = new uint8_t[compressed_size];
	file.read((char*)compressedBuffer, compressed_size);

	std::vector<uint8_t> result(uncompressed_size);
	
	z_stream strm;
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
	strm.avail_in = 0;
	strm.next_in = Z_NULL;
	int ret = inflateInit(&strm);
	if (ret != Z_OK) {
		std::cerr << "zlib init failed" << std::endl;
	}

	strm.avail_in = compressed_size;
	strm.next_in = compressedBuffer;

	strm.avail_out = uncompressed_size;
	strm.next_out = result.data();

	ret = inflate(&strm, Z_FINISH);

	if (!((ret == Z_STREAM_END) || (strm.avail_in == 0))) {
		std::cerr << "Some data remaining in compressedBuffer or didn't receive Z_STREAM_END" << std::endl;
	}

	inflateEnd(&strm);

	return result;
}

} // namespace torchlight2