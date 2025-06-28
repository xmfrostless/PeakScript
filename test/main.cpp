#include <iostream>
#include <peak.h>

void add_search_path(const std::filesystem::path& path) {
    std::cout << path << std::endl;
    peak::System::AddSearchSrcDir(path.string());
    peak::System::AddSearchSrcDir((path / "test/scripts").string());
    peak::System::AddSearchSrcDir((path / "scripts").string());
}

int main(int, char** argv) {
    auto path = std::filesystem::path(argv[0]);
    while (!path.empty() && path.has_parent_path() && path != path.parent_path()) {
        path = path.parent_path();
        add_search_path(path);
    }
    auto script = peak::Script::LoadFile("./test_main.peak");
    if (script) {
        script->Execute();
        std::cout << "\n test over.\n" << std::endl;
    } else {
        std::cout << "\nnot found: test_main.peak\n" << std::endl;
    }
    getchar();
    return 0;
}