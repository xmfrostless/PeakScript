#include <peak.h>
#include <iostream>
#include <string.h>

void show_help() {
    std::cout << "usage: peak [<filename>]" << std::endl;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        show_help();
        return 0;
    }

    for (int i = 1; i < argc; ++i) {
        const char* option = argv[i];
        if (strcmp(option, "--help") == 0) {
            show_help();
        } else {
            auto script = peak::Script::LoadFile(option);
            if (script) {
                script->Execute();
            }
        }
    }

    return 0;
}