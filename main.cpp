#include <iostream>
#include "module/parser.h"
#include "module/database.h"

int main(int argc, char* argv[]) {

    try {
        ensureDataDirectory();

        if (argc == 1) {
            runInteractive();
        } else if (argc == 2) {
            runBatch(argv[1]);
        } else {
            std::cerr << "Usage:\n";
            std::cerr << "  ./prog\n";
            std::cerr << "  ./prog script.txt\n";
            return 1;
        }
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}