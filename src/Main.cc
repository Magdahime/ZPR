#ifndef LINUX_PRAGMA
#pragma warning(push, 0)
#endif //LINUX_PRAGMA

#include <iostream>

#ifndef LINUX_PRAGMA
#pragma warning(pop)
#endif //LINUX_PRAGMA

#include "Program.h"

using namespace zpr;

int main()
{
    Program program;
    program.run();
    return 0;
}