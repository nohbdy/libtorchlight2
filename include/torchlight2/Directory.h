#ifndef LIBTORCHLIGHT2_DIRECTORY_H_
#define LIBTORCHLIGHT2_DIRECTORY_H_

#include <memory>
#include <vector>
#include <string>
#include <fstream>

#include "File.h"

namespace torchlight2 {

class Directory;
class PakFile;

typedef std::unique_ptr<Directory> DirectoryPtr;

// static const instance of a unique_ptr<File> used as a return type for functions that might not find a valid unique_ptr<File> to return
// This unique_ptr points to nothing (nullptr / NULL) and so conversion to bool will return false, etc...
// Anyone calling a function that might return null_file should check if the returned unique_ptr is valid (e.g.  if (!result) { /* It's a null_file */ })
static const FilePtr null_file;
static const DirectoryPtr null_dir;

class Directory {
public:
	~Directory();

	static DirectoryPtr Read(PakFile* pf, std::ifstream& pakman);

	const FilePtr& FindFile(const std::string& filename) const;

	const std::string& path() const { return path_; }

	const std::vector<FilePtr>& files() const { return files_; }

	PakFile& pakfile() const { return *pakfile_; }

private:
	PakFile* pakfile_;

	std::vector<FilePtr> files_;

	std::string path_;
};
} // namespace torchlight2

#endif
