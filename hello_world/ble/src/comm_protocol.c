/**
 * @file comm_protocol.c
 *
 * @brief functions to pack data according to communication protocol
 *
 */

//* INCLUDES ------------------------------------------------------------------
#include <comm_protocol.h>

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

//* PRIVATE FUNCTION PROTOTYPES ----------------------------------------------

//* MACROS --------------------------------------------------------------------
LOG_MODULE_REGISTER(comm_protocol, LOG_LEVEL_DBG);

//* LOCAL VARIABLES ------------------------------------------------------------

//* PUBLIC FUNCTIONS -----------------------------------------------------------

/**
 * @brief packs data according to the communication protocol
 *
 * @param ret pointer to pointer to the packed data (FREE MEMORY AFTER USE)
 * @param timestamp timestamp of the data
 * @param spacing spacing between data points
 * @param data pointer to the data to be packed
 * @param num_datapoints number of data points
 * @return uint32_t >0 size of data on success, -1 on failure
 */
uint32_t pack_data(void *ret, uint32_t timestamp, uint32_t spacing, float *data, uint16_t num_datapoints)
{
    memcpy(ret, &timestamp, sizeof(uint32_t));
    memcpy(ret + sizeof(uint32_t), &spacing, sizeof(uint32_t));
    memcpy(ret + 2 * sizeof(uint32_t), data, sizeof(float) * num_datapoints);

    return 2 * sizeof(uint32_t) + sizeof(float) * num_datapoints;
}