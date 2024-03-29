extern "C" {
#include "dromozoa.h"
}

#include <iostream>

void command_png(int argc, char* argv[]) {
  for (int i = 0; i < argc; ++i) {
    if (i > 0) {
      std::cerr << " ";
    }
    std::cerr << argv[i];
  }
  std::cerr << "\n";
}
