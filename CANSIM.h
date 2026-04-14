#ifndef CANSIM
#define CANSIM



#include "main.h"
#include "stm32c0xx_hal_fdcan.h"
#include <stdint.h>
#include <stdbool.h>

#define BIT_MASK1 0b11111111
#define BIT_MASK2 0b00000000
typedef enum {
    CAN_MODE_COUNTER, //simply increments every cycle
    CAN_MODE_RAMP, //8-byte ramp: ecah byte = counter + offset
    CAN_MODE_TOGGLE, //alternates between 0 and max on all bits (0x00 to 0xFF)
    CAN_MODE_WALKING_BIT, //single bit walking (this is when it makes every bit 1 or 0 one at a time)
    // LATER WHEN MASON CAN GET DATA : CAN_MODE_ASREALISTICAP, //I'm gonna try to actually simulate what the bike does, idk how useful this is or whatever
    CAN_MODE_COUNT
} CAN_SimMode;

typedef struct {
    uint32_t base_id; //starting CAN id
    CAN_SimMode mode;        
    uint8_t bytes; // bytes per frame
    bool use_ext_id; // false = 11-bit , true = 29-bit
} CAN_SimConfig;

typedef struct {
    CAN_SimConfig cfg; 
    uint32_t tick; //increments per send
    uint8_t walk_place; //current pos for walking bit
    bool toggle_state; //toggle current state
} CAN_SimHandle;

void CAN_Sim_Init (CAN_SimHandle *sim, CAN_SimConfig cfg);
void CAN_Sim_Mode (CAN_SimHandle *sim, CAN_SimMode mode);
bool CAN_Sim_Send (CAN_SimHandle *sim, FDCAN_HandleTypeDef *hfdcan);





#endif