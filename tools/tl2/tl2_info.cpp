#include <iostream>
#include <iomanip>

#include "tl2.h"
#include "torchlight2/PakFile.h"

using namespace std;

// Translate a NodeType enum value into string for printing
const char* fileTypeToName(torchlight2::FileType t) {
	switch (t) {
	case torchlight2::FileType_Dat:
		return "Dat or Template";
	case torchlight2::FileType_Layout:
		return "Layout";
	case torchlight2::FileType_Mesh:
		return "Mesh";
	case torchlight2::FileType_Skeleton:
		return "Skeleton";
	case torchlight2::FileType_DDS:
		return "DDS";
	case torchlight2::FileType_PNG:
		return "PNG";
	case torchlight2::FileType_Sound:
		return "Sound (Ogg/Wav)";
	case torchlight2::FileType_Directory:
		return "Directory";
	case torchlight2::FileType_Material:
		return "Material";
	case torchlight2::FileType_Raw:
		return "Raw";
	case torchlight2::FileType_ImageSet:
		return "ImageSet";
	case torchlight2::FileType_TTF:
		return "TrueType Font";
	case torchlight2::FileType_Font:
		return "Font";
	case torchlight2::FileType_HIE:
		return "HIE";
	case torchlight2::FileType_Animation:
		return "Animation";
	case torchlight2::FileType_Scheme:
		return "Scheme";
	case torchlight2::FileType_LookNFeel:
		return "Look and Feel";
	case torchlight2::FileType_MPD:
		return "MPD";
	default:
		return "???";
	}
}

// Write information about a given file within the archive
int doInfo(const torchlight2::PakFilePtr& archive, const char* path) {
	// Open the node if it exists
	auto& file = archive->FindFile(path);
	if (!file) {
		cerr << "File not found" << endl;
		return 2;
	}

	// Print the following information:
	// <path>
	// offset <file_offset>
	// file_type <File|Directory|Unknown>
	// data_size <uncompressed_size>
	cout << path << endl;
	cout << "offset " << file->file_offset() << endl;
	cout << "file_type " << fileTypeToName(file->file_type()) << endl;
	cout << "data_size " << file->size() << endl;

	/*
	cout << "hash " << hex << setfill('0');
	const unsigned char* hash = file->hash();
	for (int i = 0; i < 32; i++) {
		cout << setw(2) << (unsigned)hash[i];
	}
	cout << endl;
	*/

	return 0;
}