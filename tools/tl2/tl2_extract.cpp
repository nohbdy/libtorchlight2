#include <iostream>
#include <fstream>

#include "tl2.h"
#include "torchlight2/PakFile.h"
#include "torchlight2/File.h"

using namespace std;

// Extract a file from the archive and save it to disk
int doExtract(const torchlight2::PakFilePtr& archive, const char* path, const char* outPath) {
	// Ensure the source file exists in the archive
	auto& file = archive->FindFile(path);
	if (!file) {
		cerr << "Failed to extract " << path << ": File not found" << endl;
		return 2;
	}

	if (file->size() == 0) {
		cerr << "Failed to extract " << path << ": There's nothing to extract! (uncompressed_size == 0)." << endl;
		return 4;
	}

	// Create/truncate the destination file
	ofstream outFile;
	outFile.open(outPath, ios::out | ios::binary | ios::trunc);

	if (!outFile.is_open()) {
		cerr << "Failed to extract " << path << ": Could not open " << outPath << " for writing" << endl;
		return 3;
	}

	// Read the data from the archive into a buffer
	auto fileData = file->data();
	
	// Write the buffer to the output
	outFile.write((const char*)fileData.data(), fileData.size());
	outFile.close();

	cout << path << " saved to " << outPath << endl;

	return 0;
}