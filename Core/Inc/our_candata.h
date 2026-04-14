#ifndef OUR_CANDATA_H
#define OUR_CANDATA_H

#include <stdint.h>

typedef struct {
    /* ── BMS ──────────────────────────────────────────── */
    uint8_t  pack_state_of_charge;   /* 0x6B0 — raw, ×0.5 % to decode  */
    uint16_t pack_voltage;           /* 0x6B0 — raw, ×0.1 V to decode  */
    int16_t  pack_current;           /* 0x6B0 — raw, ×0.1 A (+ = discharge) */
    uint16_t high_cell_temp;         /* 0x6B4 — raw, ×1 °C             */
    uint16_t low_cell_temp;          /* 0x6B4 — raw, ×1 °C             */
    uint16_t highest_cell_voltage;   /* 0x6B3 — raw, ×1 V (coarse)     */
    uint16_t lowest_cell_voltage;    /* 0x6B3 — raw, ×0.0001 V         */
    uint16_t bms_temperature;        /* 0x6B1 — raw, ×1 °C             */
    uint8_t  bms_fault;              /* 0x6B0 — malfunction indicator   */
    uint8_t  bms_error;              /* derived from bms_error_codes    */
    uint8_t  bms_warning;            /* software-level classification   */
    uint32_t bms_error_codes;        /* 0xAB  — Post_Fault_Lo | Run_Fault_Lo */

    /* ── RMS motor controller ─────────────────────────── */
    int16_t  motor_temperature;      /* 0xA2 — raw, ×0.1 °C            */
    uint16_t mc_temperature;         /* 0xA0 — raw, ×0.1 °C            */
    int16_t  motor_speed;            /* 0xA5 — raw, ×1 rpm             */
    uint8_t  mc_fault;               /* 0xAB — nonzero = fault present  */
    uint8_t  motor_on;               /* 0xAA — inverter run mode bit    */

    /* ── PDU ──────────────────────────────────────────── */
    uint8_t  bike_status;            /* 0x706 — bits: 0=ACC 1=IGN 2=Latch */

    /* ── GPCM ─────────────────────────────────────────── */
    uint8_t  aux_voltage;            /* 0x700 — raw, ×0.01 V (lossy in uint8) */

    /* ── Derived (not from CAN directly) ──────────────── */
    uint8_t  aux_percent;            /* computed from aux_voltage + curve */
    int16_t  bike_speed;             /* computed from motor_speed + gear ratio */

} our_candata;

#endif /* OUR_CANDATA_H */