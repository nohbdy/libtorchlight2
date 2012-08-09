#ifndef LIBTORCHLIGHT2_PAKFILE_H_
#define LIBTORCHLIGHT2_PAKFILE_H_

#include <stdint.h>
#include <string>
#include <fstream>
#include <unordered_map>
#include <memory>

#include "Directory.h"
#include "File.h"

namespace torchlight2 {

class PakFile;

typedef std::unique_ptr<PakFile> PakFilePtr;

// Encapsulates a Torchlight 2 PAK archive
class PakFile {
public:
	~PakFile();

	// Create a PakFile object for the specified .PAK archive
	static PakFilePtr Open(const std::string& pak_file_path);

	// Attempt to locate a file with the given path.  Returns NULL if no file exists with the given filename
	const FilePtr& FindFile(const std::string& path) const;

	// Return a pointer to the root directory
	const DirectoryPtr& root() const { return directory_map_.at(root_); }

	// Return the filename of the PAK file this object is encapsulating
	const std::string& filename() const { return pak_file_path_; }

	const DirectoryPtr& FindDirectory(const std::string& path) const;

	std::ifstream& pakfile() { return pakfile_; }

private:
	explicit PakFile(const std::string& pak_file_path) : pak_file_path_(pak_file_path) { }

	PakFile(const PakFile& pf) { }

	// Initialize the PakFile object by opening the PAK and PAK.MAN files and reading some initial data
	int Init();

	// Input file stream for DATA.PAK
	std::ifstream pakfile_;

	// Path to the game's folder (the directory that contains torchlight2.exe)
	std::string pak_file_path_;

	// Pointer to the root directory
	std::string root_;

	// HashMap to lookup directories by path
	std::unordered_map<std::string, DirectoryPtr> directory_map_;
};

} // namespace torchlight2

#endif
