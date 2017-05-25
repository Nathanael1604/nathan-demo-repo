

#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include "system.h"

#define PACER_RATE 500
#define WALL_ROW 0
#define NUM_ROW 7
#define NUM_COL 5
#define COIN_MOVE_RATE 150
#define BONUS_MAX 3
#define ENOUGH_COINS 10
#define MAX_COUNT 50

static const uint8_t comms[] =
{
    0b10011000,
    0b01001000,
    0b00000000
};

/* Defines and groups all variables used into related lists. The struct 
 * variable_List is similar to a global struct that contains all the variables
 * used and allows access to them all through this single parent struct.*/
typedef struct position {
    uint8_t col;
    uint8_t row;
} Position;

typedef struct counter_List {
    uint8_t bonus;
    uint8_t coin;
    uint8_t score;
    uint8_t flash;
    uint8_t ir_count;
    uint8_t tick;
    uint8_t coin_freq;
} Counter_List;

typedef struct flag_List {
    bool end;
    bool no_wall;
    bool bonus;       
} Flag_List;

typedef struct object {
    Position pos;
} Object;

typedef struct variable_List
{
    Object player;
    Object bonus;
    Counter_List counters;
    Flag_List flags;
} Variable_List;

uint8_t GetRand(uint8_t min, uint8_t max);

uint8_t gen_Rand(uint8_t coin_freq);

Variable_List* ledmat_display (Variable_List* variable, uint8_t** coin_pos, uint8_t* wall_active_array);

Variable_List* destroy_wall (const uint8_t* comms, Variable_List* variable);

Variable_List* navswitch_controls (Variable_List* variable, uint8_t* wall_active_array);

Variable_List* bonus_process(Variable_List* variable);

Variable_List* coin_speed_increase(Variable_List* variable);

Variable_List* tick(Variable_List* variable, uint8_t** coin_pos);

Flag_List* check_game_end (uint8_t* wall_active_array, Flag_List* flag);

#endif
