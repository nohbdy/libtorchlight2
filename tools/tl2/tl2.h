#ifndef LIBTORCHLIGHT2_TOOLS_TL2_H_
#define LIBTORCHLIGHT2_TOOLS_TL2_H_

#include "torchlight2/PakFile.h"

namespace tl2_options {
	enum CommandOptions {
		None,
		Info,
		List,
		Extract,
		Dumpall
	};
}

namespace torchlight2 {
	class PakFile;
}

int doNone(const torchlight2::PakFilePtr& archive);
int doInfo(const torchlight2::PakFilePtr& archive, const char* path);
int doList(const torchlight2::PakFilePtr& archive);
int doExtract(const torchlight2::PakFilePtr& archive, const char* path, const char* outPath);
int doDumpall(const torchlight2::PakFilePtr& archive, std::string outPath);

#endif