#include <iostream>
#include <iomanip>
#include <string>

#include "tl2.h"
#include "torchlight2/PakFile.h"

using namespace std;

void PrintHelpMessage() {
	cout << "Extract information from or about Torchlight 2 PAK archives" << endl << endl;
	cout << "tl2 [-ilxd] pak_path [source_path] [destination_path]" << endl << endl;
	cout << "Options:" << endl;
	cout << " -i | --info : Print information about a file within the archive.  source_path specifies the path of the file within the archive." << endl;
	cout << " -l | --list : Print a list of all the files which are contained within the archive" << endl;
	cout << " -x | --extract : Extract a file from the archive.  sourch_path specifies the path of the file within the archive.  destination_path specifies the path to where the file data should be saved" << endl;
	cout << " -d | --dump : Extract all files from the archive. destination_path specifies the base folder. e.g. tl2 -d \"c:\\games\\Torchlight II\\\" \"c:\\dumphere\\\" will create c:\\dumphere\\MEDIA\\AFFIXES.RAW" << endl;
}

bool ParseCommandLine(int argc, char* argv[], tl2_options::CommandOptions* option, const char** path_to_archive, const char** source_path, const char** destination_path) {
	if (argc < 2) {
		PrintHelpMessage();
		return false;
	}

	*source_path = NULL;
	*destination_path = NULL;

	if (argv[1][0] != '-') {
		(*option) = tl2_options::None;
		(*path_to_archive) = argv[1];
		return true;
	}

	string cmd(argv[1]);
	int reqargc;

	if ((cmd == "-i") || (cmd == "--info")) {
		*option = tl2_options::Info;
		reqargc = 4; // exe cmd archive source
	} else if ((cmd == "-l") || (cmd == "--list")) {
		*option = tl2_options::List;
		reqargc = 3; // exe cmd archive
	} else if ((cmd == "-x") || (cmd == "--extract")) {
		*option = tl2_options::Extract;
		reqargc = 5; // exe cmd archive source dest
	} else if ((cmd == "-d") || (cmd == "--dump")) {
		*option = tl2_options::Dumpall;
		reqargc = 4;
	} else {
		PrintHelpMessage();
		return false;
	}

	if (argc < reqargc) {
		PrintHelpMessage();
		return false;
	}

	*path_to_archive = argv[2];

	if (reqargc < 4) { return true; }
	*source_path = argv[3];

	if (reqargc < 5) { return true; }
	*destination_path = argv[4];

	return true;
}

int main(int argc, char* argv[])
{
	tl2_options::CommandOptions op;
	const char* game_folder;
	const char* source_path;
	const char* destination_path;

	if (!ParseCommandLine(argc, argv, &op, &game_folder, &source_path, &destination_path)) {
		return 1;
	}

	torchlight2::PakFilePtr archive = torchlight2::PakFile::Open(game_folder);
	if (archive == NULL) {
		cerr << "Unable to open PAK archive under " << game_folder << endl;
		return 1;
	}

	int result;
	switch (op) {
	case tl2_options::None:
		result = doNone(archive);
		break;
	case tl2_options::Info:
		result = doInfo(archive, source_path);
		break;
	case tl2_options::List:
		result = doList(archive);
		break;
	case tl2_options::Extract:
		result = doExtract(archive, source_path, destination_path);
		break;
	case tl2_options::Dumpall:
		result = doDumpall(archive, source_path);
		break;
	}

	return result;
}

