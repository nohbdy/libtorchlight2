#include <iostream>
#include <fstream>

#include <direct.h> // for _mkdir

#include "tl2.h"
#include "torchlight2/PakFile.h"
#include "torchlight2/File.h"

using namespace std;

namespace {
void EnsureDirectoryExists(const std::string& outFilePath) {
	auto slashPos = outFilePath.find_first_of('\\');
	while (slashPos != std::string::npos) {
		auto subDir = outFilePath.substr(0, slashPos);
		
		if (0 == _mkdir(subDir.c_str())) {
			cout << "Creating Directory " << subDir << endl;
		} else {
			// cout << "Failed to make " << subDir << endl;
		}
		slashPos = outFilePath.find_first_of('\\', slashPos+1);
	}
}

void DumpFile(const torchlight2::FilePtr& file, const std::string& outPath) {
	auto filePath = file->path();
	auto outFilePath = outPath + filePath;
	if (file->size() == 0) {
		cerr << "Failed to extract " << filePath << ": There's nothing to extract! (uncompressed_size == 0)." << endl;
		return;
	}

	// Fixup outFilePath
	for (auto i = 0; i < outFilePath.length(); i++) {
		if (outFilePath[i] == '/') {
			outFilePath[i] = '\\';
		}
	}

	// Create/truncate the destination file
	ofstream outFile;
	outFile.open(outFilePath, ios::out | ios::binary | ios::trunc);

	if (outFile.fail()) {
		EnsureDirectoryExists(outFilePath);
		outFile.open(outFilePath, ios::out | ios::binary | ios::trunc);
	}

	if (!outFile.is_open()) {
		cerr << "Failed to extract " << filePath << ": Could not open " << outFilePath << " for writing" << endl;
		return;
	}

	// Read the data from the archive into a buffer
	auto fileData = file->data();
	
	// Write the buffer to the output
	outFile.write((const char*)fileData.data(), fileData.size());
	outFile.close();

	// cout << "Saved \"" << filePath << "\" to \"" << outFilePath << "\"" << endl;
}

// Recursively traverse the PAK directory structure
void DumpDirectory(const torchlight2::PakFilePtr& archive, const torchlight2::DirectoryPtr& node, const std::string& outPath, int& filesSaved, int& directoriesScanned) {
	directoriesScanned++;
	for (auto it = node->files().begin(); it != node->files().end(); it++) {
		auto& c = *it;
		if (c->file_type() != torchlight2::FileType_Directory) {
			filesSaved++;
			DumpFile(c, outPath);
		} else {
			auto subdirPath = node->path() + c->name();
			auto& subdir = archive->FindDirectory(subdirPath);
			if (subdir) {
				DumpDirectory(archive, subdir, outPath, filesSaved, directoriesScanned);
			}
		}
	}
}
} // namespace

// Extract all files from the archive and save them to disk -- outPath is the base of all output paths
//  For example, if outPath is c:\dumphere\ then MEDIA/AFFIXES.RAW will be saved to c:\dumphere\MEDIA\AFFIXES.RAW
int doDumpall(const torchlight2::PakFilePtr& archive, std::string outPath) {
	// Ensure our outPath ends with a path separator
	auto lastChar = outPath[outPath.length() - 1];
	if (lastChar != '/' && lastChar != '\\') {
		outPath = outPath + '\\';
	}

	// Recursively dump all files
	auto& root = archive->root();
	int filesSaved = 0;
	int directoriesScanned = 0;
	DumpDirectory(archive, root, outPath, filesSaved, directoriesScanned);
	
	cout << "All files saved to " << outPath << endl;

	return 0;
}