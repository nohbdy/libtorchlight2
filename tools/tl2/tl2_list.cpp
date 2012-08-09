#include <vector>
#include <iostream>

#include "tl2.h"
#include "torchlight2/PakFile.h"

using namespace std;

// Recursively traverse the ggpk directory structure (depth-first) and print out all valid file paths
void PrintNode(const torchlight2::PakFilePtr& archive, const torchlight2::DirectoryPtr& node) {
	for (auto it = node->files().begin(); it != node->files().end(); it++) {
		auto& c = *it;
		if (c->file_type() != torchlight2::FileType_Directory) {
			cout << c->path() << endl;
		} else {
			auto subdirPath = node->path() + c->name();
			auto& subdir = archive->FindDirectory(subdirPath);
			if (subdir) {
				PrintNode(archive, subdir);
			}
		}
	}
}

// Print a list of all files that exist within the ggpk Archive
int doList(const torchlight2::PakFilePtr& archive) {
	auto& root = archive->root();
	PrintNode(archive, root);
	return 0;
}