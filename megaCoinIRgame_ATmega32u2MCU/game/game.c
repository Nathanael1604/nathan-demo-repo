/*game.c 
 * Created by Nathanael Park and Tony Booth
 * Game "Mega Coins" where the objective is to collect coins
 * while protecting your wall. Bonuses are recieved if 10 coins are 
 * collected in a row without missing any, and they can be used to either
 * build your wall or destroy the other players.
 * */

#include "system.h"
#include "display.h"
#include "button.h"
#include "pio.h"
#include <avr/io.h>
#include "pacer.h"
#include "ledmat.h"
#include "navswitch.h"
#include "timer.h"
#include "startscreen.h"
#include "tinygl.h"
#include "task.h"
#include "led.h"
#include "ir_uart.h"
#include "../fonts/font3x5_1.h"
#include "comm.h"
#include "gameplay.h"

/* Initialises system. */
void init_function (void) {
    system_init ();
    pacer_init (PACER_RATE);
    ledmat_init ();
    button_init (); 
    display_init ();
    navswitch_init ();
    startscreen_init();
    ir_uart_init ();
    led_init ();
    led_set(LED1,0);
}
      
int main (void)
{
    /* Initialising all variables.*/
    uint8_t k = 0;
    uint8_t coin_col[NUM_ROW] = {5,5,5,5,5,5,5};
    uint8_t coin_row[NUM_ROW] = {0,1,2,3,4,5,6};
    uint8_t* coin_pos[2] = {coin_col, coin_row};
    uint8_t wall_active_array[NUM_COL] = {1,1,1,1,1};
    
    Variable_List variable_data;
    Variable_List* variable = &variable_data;
    
    Counter_List* counter = &variable->counters;
    counter->bonus = 0;
    counter->coin = 0;
    counter->score = 0;
    counter->flash = 0;  
    counter->ir_count = MAX_COUNT;
    counter->tick = 0;
    counter->coin_freq = 20;
    
    Flag_List* flag = &variable->flags;
    flag->end = 0;
    flag->bonus = 0;
    
    Object* player = &variable->player;
    player->pos.col = 2;
    player->pos.row = 2;
    
    Object* bonus = &variable->bonus;
    bonus->pos.col = gen_Rand (NUM_COL-1);
    bonus->pos.row = NUM_ROW;
    
    init_function ();
    startscreen_press_start();
    
    while (!flag->end) { /* While game has not ended, run game*/
        flag->no_wall = 1;
         if (ir_uart_read_ready_p ())
        {
            * wall_active_array = comm_receive (comms,wall_active_array);
        }
        variable = ledmat_display (variable, coin_pos, wall_active_array);
        navswitch_update ();
        variable = navswitch_controls (variable, wall_active_array);
        if ((navswitch_push_event_p (NAVSWITCH_SOUTH)) && (counter->bonus!= 0)) {
            counter->ir_count = 0;
            counter->bonus--;
        }
        variable = destroy_wall (comms, variable);
        for (k = 0; k < (NUM_ROW); k++) { /* Forloop to check and process all 7 coins present at any time */
            if ((coin_pos[1][k] == WALL_ROW) && (wall_active_array[coin_pos[0][k]] == 1)) { /* If a coin hits the wall */
                wall_active_array[coin_pos[0][k]] = 0;
            }
            if ((coin_pos[1][k] == player->pos.row) && (coin_pos[0][k] == player->pos.col)) { /* If a coin hits the player */
                coin_pos[0][k] = 5;
                counter->coin++;
                counter->score++;
            }
            else if (coin_pos[1][k] == 1 && ((0 <= coin_pos[0][k]) && (coin_pos[0][k] <= (NUM_COL-1)))) { /* If a coin passes the player */
                counter->coin = 0;
            }
            if (coin_pos[1][k] == 0) { /* Reset coin back to top */
                coin_pos[1][k] = (NUM_ROW);
                coin_pos[0][k] = gen_Rand (counter->coin_freq);
            }
        }
        
        variable = bonus_process(variable);
        variable = tick(variable, coin_pos);
        flag = check_game_end (wall_active_array, flag);
    }
    startscreen_gameover(0);
    return 0;
}
