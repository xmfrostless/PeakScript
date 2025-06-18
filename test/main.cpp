#include <iostream>
#include <peak.h>

int main(int argc, char** argv) {
	peak::System::AddSearchSrcDir(std::filesystem::path(argv[0]).parent_path().string());

	std::shared_ptr<peak::Script> script { nullptr };
	if (argc > 1) {
		script = peak::Script::LoadFile(argv[1]);
	} else {
		script = peak::Script::LoadFile("./main.peak");
	}
	if (script) {
		script->Execute();
	}

	return 0;
}