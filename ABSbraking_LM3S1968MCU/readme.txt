// Author: Nathanael Park and Hayden Becker
// f_main for ENCE361 Project 2
// Contains System Clock initialisation and main function

// ABS to Car simulator connections:
// P2.0: FL => PF4
// P2.1: FR => PF5
// P2.2: RL => PF6/CCP1
// P2.3: RR => PF7
// P2.4: Steering => PF2/PWM4
// P2.5: Brakes => PB1/CCP2
// P1.4: Brake Signal Output => PD1/PWM1

The following modules provided by Texas Instruments StellarisWare need to be linked:
“C:\Courses\ENCE361\StellarisWare\boards\ek-lm3s1968\drivers\rit128x96x4.c”
“C:\Courses\ENCE361\StellarisWare\utils\ustdlib.c”

The stack and heap size allocated for the program with the ARM linker need to be: 0x800 bits and 0x800 bits.