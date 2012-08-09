#include <iostream>
#include <cvt/wstring>
#include <codecvt>

#include "PakFile.h"

namespace {
typedef struct PakManHeader {
	uint16_t zero;
	uint16_t rootPathLen;
	char16_t* rootPath;
	uint32_t unknown;
	uint32_t numDirectories;

	PakManHeader() : zero(0), rootPathLen(0), rootPath(NULL), unknown(0), numDirectories(0) { }
} PakManHeader;
}

namespace torchlight2 {

PakFilePtr PakFile::Open(const std::string& pak_file_path) {
	std::unique_ptr<PakFile> result(new PakFile(pak_file_path));
	if (result->Init() >= 0) {
		return std::move(result);
	} else {
		return std::unique_ptr<PakFile>(); // null pointer
	}
}

PakFile::~PakFile() {
	if (pakfile_.is_open()) {
		pakfile_.close();
	}

	// root_ is owned by the unique_ptr in the directory_map_
	// it gets deleted when the directory_map_ is destroyed (automatically)
}

const FilePtr& PakFile::FindFile(const std::string& path) const {
	auto last_slash = path.find_last_of('/');
	if (last_slash == std::string::npos) {
		return null_file;
	}

	auto dir = path.substr(0, last_slash+1); // Get the directory path including the last slash
	auto filename = path.substr(last_slash+1); // Get the filename + extension (excluding the last slash)

	auto found = directory_map_.find(dir);
	if (found == directory_map_.end()) {
		return null_file;
	}

	auto& filePtr = found->second->FindFile(filename);

	return filePtr;
}

const DirectoryPtr& PakFile::FindDirectory(const std::string& path) const {
	auto found = directory_map_.find(path);
	if (found == directory_map_.end()) {
		return null_dir;
	}

	return found->second;
}

int PakFile::Init() {
	// Open .PAK file
	pakfile_.open(pak_file_path_, std::ifstream::in | std::ifstream::binary);
	if (!pakfile_.is_open()) {
		std::cerr << "Cannot find pak file: " << pak_file_path_ << std::endl;
		return -1;
	}

	// Open the associated .PAK.MAN file
	std::string pakman_path = this->pak_file_path_ + ".MAN";
	std::ifstream pakman;
	pakman.open(pakman_path, std::ifstream::in | std::ifstream::binary);
	if (!pakman.is_open()) {
		std::cerr << "Cannot open manifest file: " << pakman << std::endl;
		return -1;
	}

	// Read in the PakManHeader
	PakManHeader pakManHeader;
	pakman.seekg(0);
	pakman.read((char*)&pakManHeader.zero, 2);
	pakman.read((char*)&pakManHeader.rootPathLen, 2);
	pakManHeader.rootPath = new char16_t[pakManHeader.rootPathLen+1];
	pakman.read((char*)pakManHeader.rootPath, 2 * pakManHeader.rootPathLen);
	pakManHeader.rootPath[pakManHeader.rootPathLen] = 0;
	pakman.read((char*)&pakManHeader.unknown, 4);
	pakman.read((char*)&pakManHeader.numDirectories, 4);

	stdext::cvt::wstring_convert<std::codecvt_utf8_utf16<char16_t>,char16_t> convert;
	root_ = convert.to_bytes(pakManHeader.rootPath);

	// Read in all the directories from PakMan
	for (uint32_t i = 0; i < pakManHeader.numDirectories; i++) {
		auto dir = Directory::Read(this, pakman);
		directory_map_.insert(std::make_pair<std::string, DirectoryPtr>(dir->path(), std::move(dir)));
	}

	pakman.close();
	return 0;
}

} // namespace torchlight2