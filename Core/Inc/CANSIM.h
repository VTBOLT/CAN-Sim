#ifndef CANSIM_H
#define CANSIM_H

#include "main.h"
#include <stdint.h>
#include <stdbool.h>

#define BIT_MASK1 0xFF
#define BIT_MASK2 0x00

typedef enum {
    CAN_MODE_COUNTER,     /* byte 0 = tick low, byte 1 = tick high */
    CAN_MODE_RAMP,        /* each byte = tick + offset */
    CAN_MODE_TOGGLE,      /* alternates 0xFF and 0x00 every frame */
    CAN_MODE_WALKING_BIT, /* single bit walks across all bytes */
    CAN_MODE_REALIST,     /* Based on interpreted CAN data*/
    CAN_MODE_COUNT      /* keep last -- used for mode cycling */
} CAN_SimMode;

typedef struct {
    uint32_t    base_id;    /* starting CAN ID */
    CAN_SimMode mode;
    uint8_t     bytes;      /* bytes per frame (1-8) */
    bool        use_ext_id; /* false = 11-bit, true = 29-bit */
} CAN_SimConfig;

typedef struct {
    CAN_SimConfig cfg;
    uint32_t      tick;         /* increments per successful send */
    uint8_t       walk_place;   /* current bit position for walking bit */
    bool          toggle_state; /* current toggle state */
} CAN_SimHandle;



static const uint32_t CAN_SIM_ID_TABLE[] = {
    0x01,    /*Ignition */
    0x02,    /*Throttle Pin*/
    0x6B0,  /* BMS Info */
    0x6B1,  /* BMS Info 1 */
    0x6B2,  /* BMS Info 2 */
    0x6B3,  /* BMS Info 3 */
    0x6B4,  /* BMS Info 4 */
    0x700,  /* GPCM Voltages */
    0xA0,   /* RMS Temp 1 */
    0xA1,   /* RMS Temp 2 */
    0xA2,   /* RMS Temp 3 */
    0xA3,   /* RMS Analog Inputs */
    0xA4,   /* RMS Digital Inputs */
    0xA5,   /* RMS Motor Position */
    0xA6,   /* RMS Current */
    0xA7,   /* RMS Voltage */
    0xA8,   /* RMS Flux */
    0xA9,   /* RMS Internal Voltages */
    0xAA,   /* RMS Internal States */
    0xAB,   /* RMS Fault Codes */
    0xAC,   /* RMS Torque */
    0xAD,   /*RMS Modulation/Flux*/
    0xAE,   /*Firmware Info*/
    0xAF,   /*Diag Data*/
    0x1D7,   /*M187_U2C_Command_Txd (whatever this is)*/
    0xC2,    /*M194_Read_Write_Param_Response (whatever this is)*/
    0x701,  /* PDU VCC Voltage */
    0x702,  /* PDU VCC Current */
    0x703,  /* PDU 12V Current */
    0x704,  /* PDU 5V Current */
    0x705,  /* PDU 3V3 Current */
    0x706,  /* PDU Bike Status */
};

#define CAN_SIM_ID_COUNT (sizeof(CAN_SIM_ID_TABLE) / sizeof(CAN_SIM_ID_TABLE[0]))

void CAN_Sim_Init   (CAN_SimHandle *sim, CAN_SimConfig cfg);
void CAN_Sim_SetMode(CAN_SimHandle *sim, CAN_SimMode mode);
bool CAN_Sim_Send   (CAN_SimHandle *sim, FDCAN_HandleTypeDef *hfdcan);

#endif 
   
