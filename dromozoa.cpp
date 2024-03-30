/*
 *  This file is part of eblook.
 *  Copyright (C) 2024 Tomoyuki Fujimori <moyu@dromozoa.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "dromozoa.h"

#include <ebu/defs.h>
#include <ebu/error.h>
#include <ebu/text.h>

#include <png.h>

#include <cstddef>
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

  class png_writer_t {
  public:
    png_writer_t(const png_writer_t&) = delete;
    png_writer_t& operator=(const png_writer_t&) = delete;

    explicit png_writer_t(const char* path) : handle_(std::fopen(path, "wb"), std::fclose), png_(), info_() {
      try {
        if (!handle_) {
          handle_error(nullptr, "cannot fopen");
        }

        png_ = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, handle_error, nullptr);
        if (!png_) {
          handle_error(nullptr, "cannot png_create_write_struct");
        }

        info_ = png_create_info_struct(png_);
        if (!info_) {
          png_error(png_, "cannot png_create_info_struct");
        }

        png_init_io(png_, handle_.get());
      } catch (...) {
        png_destroy_write_struct(&png_, &info_);
        throw;
      }
    }

    png_structp png() {
      return png_;
    }

    png_infop info() {
      return info_;
    }

    ~png_writer_t() {
      png_destroy_write_struct(&png_, &info_);
    }

  private:
    std::unique_ptr<std::FILE, decltype(&std::fclose)> handle_;
    png_structp png_;
    png_infop info_;

    static void handle_error(png_structp, const char* message) {
      throw std::runtime_error(message);
    }
  };

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

      int width = std::stoi(argv[2]);
      if (width <= 0 || width % 8 != 0) {
        throw std::invalid_argument("invalid width");
      }
      int height = std::stoi(argv[3]);
      if (height <= 0) {
        throw std::invalid_argument("invalid height");
      }

      png_writer_t png_writer(argv[4]);
      png_set_IHDR(png_writer.png(), png_writer.info(),
          width,
          height,
          1,
          PNG_COLOR_TYPE_GRAY,
          PNG_INTERLACE_NONE,
          PNG_COMPRESSION_TYPE_DEFAULT,
          PNG_FILTER_TYPE_DEFAULT);

      std::vector<png_byte> buffer(width * height / 8);
      ssize_t buffer_size = 0;
      check(eb_read_rawtext(&current_book, buffer.size(), reinterpret_cast<char*>(buffer.data()), &buffer_size));

      std::vector<png_bytep> rows(height);
      for (int i = 0; i < height; ++i) {
        rows[i] = &buffer[width * i / 8];
      }
      png_set_rows(png_writer.png(), png_writer.info(), rows.data());
      png_write_png(png_writer.png(), png_writer.info(), PNG_TRANSFORM_INVERT_MONO, nullptr);

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
