// can_parse.h
#ifndef CAN_PARSE_H
#define CAN_PARSE_H

#include "CAN_IDS.h"
#include "CANSIM.h"   
#include <stdint.h>
#include <our_candata.h>
#include "main.h" 

void CAN_Dispatch(uint32_t id, const uint8_t *data, our_candata *out);

#endif