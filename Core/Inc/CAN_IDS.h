#ifndef CAN_IDS_H
#define CAN_IDS_H

/* ── BMS (0x6B0–0x6B4) ─────────────────────────────── */
#define CAN_ID_BMS_INFO          0x6B0
#define CAN_ID_BMS_INFO_1        0x6B1
#define CAN_ID_BMS_INFO_2        0x6B2
#define CAN_ID_BMS_INFO_3        0x6B3
#define CAN_ID_BMS_INFO_4        0x6B4

/* ── RMS Motor Controller (0xA0–0xAF, 0xC2) ────────── */
#define CAN_ID_RMS_TEMP_1        0xA0
#define CAN_ID_RMS_TEMP_2        0xA1
#define CAN_ID_RMS_TEMP_3        0xA2
#define CAN_ID_RMS_ANALOG_IN     0xA3
#define CAN_ID_RMS_DIGITAL_IN    0xA4
#define CAN_ID_RMS_MOTOR_POS     0xA5
#define CAN_ID_RMS_CURRENT       0xA6
#define CAN_ID_RMS_VOLTAGE       0xA7
#define CAN_ID_RMS_FLUX          0xA8
#define CAN_ID_RMS_INT_VOLTAGES  0xA9
#define CAN_ID_RMS_INT_STATES    0xAA
#define CAN_ID_RMS_FAULT_CODES   0xAB
#define CAN_ID_RMS_TORQUE        0xAC
#define CAN_ID_RMS_MODULATION    0xAD
#define CAN_ID_RMS_FIRMWARE      0xAE
#define CAN_ID_RMS_DIAG          0xAF
#define CAN_ID_RMS_RW_PARAM      0xC2

/* ── GPCM ───────────────────────────────────────────── */
#define CAN_ID_GPCM_VOLTAGES     0x700

/* ── PDU (0x701–0x706) ──────────────────────────────── */
#define CAN_ID_PDU_VCC_VOLTAGE   0x701
#define CAN_ID_PDU_VCC_CURRENT   0x702
#define CAN_ID_PDU_12V_CURRENT   0x703
#define CAN_ID_PDU_5V_CURRENT    0x704
#define CAN_ID_PDU_3V3_CURRENT   0x705
#define CAN_ID_PDU_BIKE_STATUS   0x706

#endif /* CAN_IDS_H */
