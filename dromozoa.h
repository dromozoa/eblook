#ifndef DROMOZOA_H
#define DROMOZOA_H

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

#ifdef __cplusplus
extern "C" {
#endif

struct command_table_t;
struct command_table_t* dromozoa_command_table(struct command_table_t*);

#ifdef __cplusplus
}
#endif

#endif
