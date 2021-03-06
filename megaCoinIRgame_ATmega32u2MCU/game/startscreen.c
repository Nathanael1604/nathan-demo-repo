/*startscreen.c 
 * Created by Nathanael Park and Tony Booth
 * All in game text loops.
 * */

#include "system.h"
#include "tinygl.h"
#include "button.h"
#include "pacer.h"
#include "navswitch.h"
#include "startscreen.h"
#include "../fonts/font3x5_1.h"
#define PACER_RATE 500
#define MESSAGE_RATE 25




/** Initialise startscreen.  */
void startscreen_init (void)
{
    tinygl_init (PACER_RATE);
    tinygl_text_speed_set (MESSAGE_RATE);
    tinygl_font_set (&font3x5_1);
    tinygl_text_mode_set  (TINYGL_TEXT_MODE_SCROLL);
    tinygl_text_dir_set (TINYGL_TEXT_DIR_ROTATE);
    tinygl_text (" MEGA COIN");
    
}

/** Players to press Start (Navswitch Push) to start game */
void startscreen_press_start (void)
{
    bool startGame = 0;
    int i = 0;
    while(!startGame)
    {
        pacer_wait();
        tinygl_update ();
        button_update ();
        navswitch_update ();
    
        if (button_push_event_p (BUTTON1) && (i == 0))
        {
            tinygl_text (" PLAYERS PRESS START");
            i = 1;
        }
        else if ((navswitch_push_event_p (NAVSWITCH_PUSH)) && (i == 1))
        {
            startGame = 1;
        }
    }
}

/** Game ends and players get message depending on outcome **/
void startscreen_gameover (bool win)
{
    int i = 1;
    while(1){
        pacer_wait();
        tinygl_update ();
        
        if (win == 0 && i == 1){
            tinygl_text (" YOU LOSE!!!!");
        }
        
        else if ((win == 1) && (i == 1)){
            tinygl_text (" YOU WIN!!!!");
        }
        i = 0;
    }
}
