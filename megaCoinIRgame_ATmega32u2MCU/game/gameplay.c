/*comm.c 
 * Created by Nathanael Park and Tony Booth
 * Game play Module
 * */

#include <stdlib.h>
#include "gameplay.h"
#include "system.h"
#include "display.h"
#include "pio.h"
#include <avr/io.h>
#include "pacer.h"
#include "ledmat.h"
#include "navswitch.h"
#include "timer.h"
#include "led.h"
#include "comm.h"
#include "ir_uart.h"

uint8_t GetRand(uint8_t min, uint8_t max)
{
  uint8_t rc;
  rc = (rand() % (max - min + 1) + min);
  
  return (rc);
}

uint8_t gen_Rand(uint8_t coin_freq)
{
  uint8_t r;
  r = GetRand(0, coin_freq);
  return(r);
}

/* Function for displaying on LED Matrix */
Variable_List* ledmat_display (Variable_List* variable, uint8_t** coin_pos, uint8_t* wall_active_array) 
{
    Counter_List* counter = &variable->counters;
    Flag_List* flag = &variable->flags;
    Object* player = &variable->player;
    Object* bonus = &variable->bonus;
    uint8_t k=0;
    pacer_wait();
    display_clear ();
    display_pixel_set (player->pos.col, player->pos.row, 1);
    for (k = 0; k < (NUM_ROW); k++) {
        display_pixel_set (coin_pos[0][k], coin_pos[1][k], 1);
    }
    for (k = 0; k < NUM_COL; k++){ 
        display_pixel_set (k, WALL_ROW, wall_active_array[k]);
    }
    if (flag->bonus && (counter->flash > 2)) {
        counter->flash = 0;
        display_pixel_set (bonus->pos.col, bonus->pos.row, 1);
    }
    counter->flash++;
    display_update ();
    pacer_wait ();
    return variable;
}

/* Function for destroying wall */
Variable_List* destroy_wall (const uint8_t* comms, Variable_List* variable) 
{
    Object* player = &variable->player;
    Counter_List* counter = &variable->counters;
    if (counter->ir_count < MAX_COUNT) {
        comm_transmit(player,comms[0]);
        counter->ir_count++;
    }
    return variable;
}

/* Function for Navswitch Controls */
Variable_List* navswitch_controls (Variable_List* variable, uint8_t* wall_active_array) {
    Object* player = &variable->player;
    Counter_List* counter = &variable->counters;
    if ((navswitch_push_event_p (NAVSWITCH_EAST)) && (player->pos.col < (NUM_COL-1))) {
        player->pos.col += 1;
    }
    else if ((navswitch_push_event_p (NAVSWITCH_WEST)) && (player->pos.col > 0)) {
        player->pos.col -= 1;
    }
    else if ((navswitch_push_event_p (NAVSWITCH_EAST)) && (player->pos.col == (NUM_COL-1))) {
        player->pos.col = 0;
    }
    else if ((navswitch_push_event_p (NAVSWITCH_WEST)) && (player->pos.col == 0)) {
        player->pos.col = NUM_COL-1;
    }
    else if (navswitch_push_event_p (NAVSWITCH_NORTH) && (counter->bonus != 0)) {
        wall_active_array[player->pos.col] = 1;
        counter->bonus--;
    }
    return variable;
}

/* Function for checking, tracking, and processing for bonus */
Variable_List* bonus_process(Variable_List* variable) {
    Counter_List* counter = &variable->counters;
    Flag_List* flag = &variable->flags;
    Object* player = &variable->player;
    Object* bonus = &variable->bonus;
    if (counter->coin == ENOUGH_COINS){
        counter->coin = 0;
        if (counter->bonus < BONUS_MAX) {
            flag->bonus = 1;
        }
    }
    
    if (((bonus->pos.row == player->pos.row) && (bonus->pos.col == player->pos.col)) || (bonus->pos.row == 0)) {
        if ((bonus->pos.row == player->pos.row) && (bonus->pos.col == player->pos.col)) {
            counter->bonus++;
        }
        bonus->pos.col = gen_Rand (NUM_COL-1);
        bonus->pos.row= NUM_ROW;
        flag->bonus = 0;
    }
    
    if (counter->bonus > 0) {
        led_set(LED1,1);
    }
    else {
        led_set(LED1,0);
    }
    return variable;
}

/* The frequency of the coins increases every 10 coins collected till it hits max.*/
Variable_List* coin_speed_increase(Variable_List* variable) {
    Counter_List* counter = &variable->counters;
    if (counter->score % 10 == 0) {
        if (counter->coin_freq > 4) {
            counter->coin_freq--;
        }
    }
    return variable;
}

/* Function for clock ticking to scroll down coins and bonuses at 1 second intervals */
Variable_List* tick(Variable_List* variable, uint8_t** coin_pos) {
    Counter_List* counter = &variable->counters;
    Flag_List* flag = &variable->flags;
    Object* bonus = &variable->bonus;
    uint8_t k = 0;
    if (counter->tick > COIN_MOVE_RATE) {
        counter->tick = 0;
        for (k = 0; k < (NUM_ROW); k++) {
            coin_pos[1][k] -= 1;
        }
        if (flag->bonus) {
            bonus->pos.row-=1;
        }
        variable = coin_speed_increase(variable);
    }
    counter->tick++;
    return variable;
}

/* Function for checking end of game */
Flag_List* check_game_end (uint8_t* wall_active_array, Flag_List* flag) {
    uint8_t k = 0;
    for (k = 0; k < NUM_COL; k++)
    {
        if (wall_active_array[k] == 1){
            flag->no_wall = 0;
        }
    }
    if (flag->no_wall){
        for (k = 0; k < MAX_COUNT; k++) {
            pacer_wait();
            comm_transmit(comms[2], comms[1]);
        }
        flag->end = 1;
    }
    return flag;
}
