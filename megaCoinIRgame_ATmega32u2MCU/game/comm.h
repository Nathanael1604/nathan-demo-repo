#ifndef COMM_H
#define COMM_H

#include "system.h"
#include "gameplay.h"

void comm_transmit (Object* player, uint8_t comm);

uint8_t* comm_receive (const uint8_t* comms, uint8_t* wall_active_array);

#endif
