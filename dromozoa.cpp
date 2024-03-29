#include "dromozoa.h"

#include <ebu/defs.h>
#include <ebu/error.h>
#include <ebu/text.h>

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

extern "C" {
  struct command_table_t {
    const char* name;
    const char* option_string;
    void (*func) (int, char*[]);
    const char* help;
  };

  int parse_entry_id(char*, EB_Position*);

  extern EB_Book current_book;
}

namespace {
  std::vector<struct command_table_t> command_table;

  void check(EB_Error_Code result) {
    if (result != EB_SUCCESS) {
      throw std::runtime_error(eb_error_message(result));
    }
  }

  // entry w h file
  void command_png(int argc, char* argv[]) {
    try {
      if (argc < 5) {
        throw std::invalid_argument("wrong number of arguments");
      }

      EB_Position position;
      if (!parse_entry_id(argv[1], &position)) {
        throw std::runtime_error("cannot parse_entry_id");
      }
      check(eb_seek_text(&current_book, &position));

      int w = std::stoi(argv[2]);
      if (w <= 0) {
        throw std::invalid_argument("invalid width");
      }
      int h = std::stoi(argv[3]);
      if (h <= 0) {
        throw std::invalid_argument("invalid height");
      }

      std::unique_ptr<std::FILE, decltype(&std::fclose)> handle(std::fopen(argv[4], "wb"), std::fclose);
      std::vector<std::uint8_t> buffer((w * h + 7) / 8);
      ssize_t buffer_size;
      check(eb_read_rawtext(&current_book, buffer.size(), reinterpret_cast<char*>(buffer.data()), &buffer_size));

    } catch (const std::exception& e) {
      std::cerr << e.what() << "\n";
    }
  }
}

struct command_table_t* dromozoa_command_table(struct command_table_t* source) {
  if (command_table.empty()) {
    for (auto* ptr = source; ptr->name; ++ptr) {
      command_table.push_back(*ptr);
    }
    command_table.push_back({ "png", "entry w h file", command_png, "dump mono image into file in PNG format.\n" });
    command_table.push_back({ nullptr, nullptr, nullptr, nullptr });
  }
  return command_table.data();
}
