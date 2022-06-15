#include "window.hpp"

#include <cstdlib>

int main(int argc, char* argv[])
{
    auto w = Window{};
    w.loop();

    return EXIT_SUCCESS;
}
