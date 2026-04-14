// can_parse.c
#include "CANParser.h"
#include <string.h>
#include "main.h" 

// pull a little-endian uint16 out of two bytes
static inline uint16_t u16_le(const uint8_t *d, uint8_t start_byte)
{
    return (uint16_t)(d[start_byte] | (d[start_byte + 1] << 8));
}

// pull a little-endian int16
static inline int16_t i16_le(const uint8_t *d, uint8_t start_byte)
{
    return (int16_t)u16_le(d, start_byte);
}

// ── per-message parsers ────────────────────────────────────────────
static void parse_bms_info(const uint8_t *d, our_candata *out)
{
    // Pack_Current  — bytes 0-1, signed, 0.1 A/LSB
    // stored raw; scale when you display
    out->pack_current = i16_le(d, 0);

    // Pack_Voltage  — bytes 2-3, unsigned, 0.1 V/LSB
    out->pack_voltage = u16_le(d, 2);

    // Pack_SOC      — byte 4, unsigned, 0.5 %/LSB
    out->pack_state_of_charge = d[4];

    // Relay/status flags live in byte 5 — add if you need them
}

static void parse_bms_info_1(const uint8_t *d, our_candata *out)
{
    // Internal_Temperature — bytes 4-5, 1 °C/LSB
    out->bms_temperature = u16_le(d, 4);
}

static void parse_bms_info_3(const uint8_t *d, our_candata *out)
{
    // High_Cell_Voltage — bytes 2-3, 1 V/LSB (coarse — see cheat sheet note)
    out->highest_cell_voltage = u16_le(d, 2);

    // Low_Cell_Voltage  — bytes 4-5, 0.0001 V/LSB
    out->lowest_cell_voltage  = u16_le(d, 4);
}

static void parse_bms_info_4(const uint8_t *d, our_candata *out)
{
    // High_Cell_Temp — bytes 0-1, signed, 1 °C/LSB
    // cast through int16 first to preserve sign before storing
    out->high_cell_temp = (uint16_t)i16_le(d, 0);

    // Low_Cell_Temp  — bytes 2-3, signed, 1 °C/LSB
    out->low_cell_temp  = (uint16_t)i16_le(d, 2);
}

static void parse_rms_temp_3(const uint8_t *d, our_candata *out)
{
    // D3_Motor_Temperature — bytes 4-5, signed, 0.1 °C/LSB
    out->motor_temperature = i16_le(d, 4);
}

static void parse_rms_temp_1(const uint8_t *d, our_candata *out)
{
    // D1_Module_A used as mc_temperature — bytes 0-1, signed, 0.1 °C/LSB
    out->mc_temperature = (uint16_t)i16_le(d, 0);
}

static void parse_rms_motor_pos(const uint8_t *d, our_candata *out)
{
    // D2_Motor_Speed — bytes 2-3, signed, 1 rpm/LSB
    out->motor_speed = i16_le(d, 2);
}

static void parse_rms_fault_codes(const uint8_t *d, our_candata *out)
{
    // Post_Fault_Lo (bytes 0-1) | Run_Fault_Lo (bytes 2-3) packed into 32 bits
    // see mapping notes — you only have 32 bits for 4x16-bit words, pick two
    uint16_t post_lo = u16_le(d, 0);
    uint16_t run_lo  = u16_le(d, 2);
    out->bms_error_codes = ((uint32_t)run_lo << 16) | post_lo;

    out->mc_fault = (run_lo != 0 || post_lo != 0) ? 1 : 0;
}

static void parse_rms_int_states(const uint8_t *d, our_candata *out)
{
    // D4_Inverter_Run_Mode — byte 5, bit 0
    out->motor_on = (d[5] >> 0) & 0x01;
}

static void parse_pdu_bike_status(const uint8_t *d, our_candata *out)
{
    // ACC=bit0, IGN=bit1, Latch=bit2 — pack all three into bike_status
    out->bike_status = d[0] & 0x07;
}

static void parse_gpcm_voltages(const uint8_t *d, our_candata *out)
{
    // Aux_Pack_Voltage — bytes 0-1, 0.01 V/LSB
    // stored raw in uint8_t (lossy — consider changing type per mapping notes)
    uint16_t raw = u16_le(d, 0);
    out->aux_voltage = (raw > 0xFF) ? 0xFF : (uint8_t)raw;
}

void CAN_Dispatch(uint32_t id, const uint8_t *data, our_candata *out)
{
    switch (id)
    {
        case CAN_ID_BMS_INFO:        parse_bms_info(data, out);        break;
        case CAN_ID_BMS_INFO_1:      parse_bms_info_1(data, out);      break;
        case CAN_ID_BMS_INFO_3:      parse_bms_info_3(data, out);      break;
        case CAN_ID_BMS_INFO_4:      parse_bms_info_4(data, out);      break;
        case CAN_ID_RMS_TEMP_1:      parse_rms_temp_1(data, out);      break;
        case CAN_ID_RMS_TEMP_3:      parse_rms_temp_3(data, out);      break;
        case CAN_ID_RMS_MOTOR_POS:   parse_rms_motor_pos(data, out);   break;
        case CAN_ID_RMS_FAULT_CODES: parse_rms_fault_codes(data, out); break;
        case CAN_ID_RMS_INT_STATES:  parse_rms_int_states(data, out);  break;
        case CAN_ID_PDU_BIKE_STATUS: parse_pdu_bike_status(data, out); break;
        case CAN_ID_GPCM_VOLTAGES:   parse_gpcm_voltages(data, out);   break;
        default: break;  // unknown ID — ignore
    }
}

