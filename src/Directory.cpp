#include <iostream>
#include <cvt/wstring>
#include <codecvt>

#include "Directory.h"
#include "PakFile.h"

namespace torchlight2 {

Directory::~Directory() {
}

const std::unique_ptr<File>& Directory::FindFile(const std::string& filename) const {
	for (auto it = files_.begin(); it != files_.end(); it++) {
		if (filename == (*it)->name()) {
			return *it;
		}
	}

	return null_file;
}

DirectoryPtr Directory::Read(PakFile* pf, std::ifstream& pakman) {
	DirectoryPtr dir(new Directory());
	dir->pakfile_ = pf;

	// Read pathLen
	uint16_t pathLen;
	pakman.read((char*)&pathLen, 2);

	// Read path
	char16_t* path = new char16_t[pathLen+1];
	pakman.read((char*)path, 2 * pathLen);
	path[pathLen] = 0;

	// Convert path to std::string
	stdext::cvt::wstring_convert<std::codecvt_utf8_utf16<char16_t>,char16_t> convert;
	dir->path_ = convert.to_bytes(path);

	// std::cout << "Dir = " << dir->path_ << std::endl;

	// Read numFiles
	uint32_t numFiles;
	pakman.read((char*)&numFiles, 4);

	// Read Files
	for (uint32_t i = 0; i < numFiles; i++) {
		dir->files_.push_back(File::Read(dir.get(), pakman));
	}

	return std::move(dir);
}

} // namespace torchlight2