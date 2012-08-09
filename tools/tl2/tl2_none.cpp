#include <iostream>

#include "tl2.h"
#include "torchlight2/PakFile.h"

using namespace std;

int doNone(const torchlight2::PakFilePtr& archive) {
	auto& root = archive->root();
	cout << "Archive " << archive->filename() << endl;
	cout << "Root Folder: " << root->path() << endl;

	return 0;
}