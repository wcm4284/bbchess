#ifndef MAGIC_H
#define MAGIC_H

#include <stdio.h>
#include "attack_table.h"
#include "bit.h"
#include "game.h"
#include "rand.h"

extern u64 bishop_magic_numbers[];
extern u64 rook_magic_numbers[];

extern void init_sliders_attacks(int bishop);

#endif