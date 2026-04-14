#include "main.h"
#include "CANSIM.h"
#include <string.h>
#include <stdbool.h>

static void build_payload(CAN_SimHandle *sim, uint8_t *data, uint8_t len)
{
    switch (sim->cfg.mode)
    {
    case CAN_MODE_COUNTER:
        data[0] = (uint8_t)(sim->tick & BIT_MASK1);
        data[1] = (uint8_t)((sim->tick >> 8) & BIT_MASK1);  
        break;

    case CAN_MODE_RAMP:
        for (uint8_t i = 0; i < len; ++i)
        {
            data[i] = (uint8_t)((sim->tick + i) & BIT_MASK1);
        }
        break;

    case CAN_MODE_TOGGLE:
        memset(data, sim->toggle_state ? BIT_MASK1 : BIT_MASK2, len);
        sim->toggle_state = !sim->toggle_state;
        break;

    case CAN_MODE_WALKING_BIT:
        {
            uint8_t total_bits  = len * 8u;
            uint8_t bit_idx     = sim->walk_place % total_bits;
            uint8_t byte_idx    = bit_idx / 8u;
            uint8_t bit_in_byte = 7u - (bit_idx % 8u);
            data[byte_idx] = (uint8_t)(1u << bit_in_byte);
            sim->walk_place = (uint8_t)((bit_idx + 1u) % total_bits);
        }
        break;
    // case CAN_MODE_REALIST:
    //     {
            

    //     }

    default:
        break;
    }
}

void CAN_Sim_Init(CAN_SimHandle *sim, CAN_SimConfig cfg)
{
    sim->cfg          = cfg;
    sim->tick         = 0;
    sim->walk_place   = 0;
    sim->toggle_state = false;
}

void CAN_Sim_SetMode(CAN_SimHandle *sim, CAN_SimMode mode)
{
    sim->cfg.mode     = mode;
    sim->tick         = 0;
    sim->walk_place   = 0;
    sim->toggle_state = false;
}

bool CAN_Sim_Send(CAN_SimHandle *sim, FDCAN_HandleTypeDef *hfdcan)
{
    FDCAN_TxHeaderTypeDef hdr;
    uint8_t data[8] = {0};

    const uint32_t dlc_map[] = {
        FDCAN_DLC_BYTES_0, FDCAN_DLC_BYTES_1, FDCAN_DLC_BYTES_2,
        FDCAN_DLC_BYTES_3, FDCAN_DLC_BYTES_4, FDCAN_DLC_BYTES_5,
        FDCAN_DLC_BYTES_6, FDCAN_DLC_BYTES_7, FDCAN_DLC_BYTES_8
    };

    uint8_t dlc = (sim->cfg.bytes > 8) ? 8 : sim->cfg.bytes;

    hdr.Identifier = CAN_SIM_ID_TABLE[sim->tick % CAN_SIM_ID_COUNT];
    hdr.IdType              = sim->cfg.use_ext_id ? FDCAN_EXTENDED_ID : FDCAN_STANDARD_ID;
    hdr.TxFrameType         = FDCAN_DATA_FRAME;
    hdr.DataLength          = dlc_map[dlc];
    hdr.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    hdr.BitRateSwitch       = FDCAN_BRS_OFF;
    hdr.FDFormat            = FDCAN_CLASSIC_CAN;
    hdr.TxEventFifoControl  = FDCAN_NO_TX_EVENTS;
    hdr.MessageMarker       = 0;

    build_payload(sim, data, dlc);

    bool check = (HAL_FDCAN_AddMessageToTxFifoQ(hfdcan, &hdr, data) == HAL_OK);
    if (check)
    {
        sim->tick++;
    }
    return check;  
}
  
