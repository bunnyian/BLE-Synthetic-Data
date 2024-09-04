#if !defined(__COMM_PROTOCOL_H__)
#define __COMM_PROTOCOL_H__

#if defined(__cplusplus)
extern "C"
{
#endif // __cplusplus

//* INCLUDES --------------------------------------------------------------------
#include <zephyr/types.h>

    //* DATA TYPES ------------------------------------------------------------------

    //* MACROS ----------------------------------------------------------------------

    //* PUBLIC FUNCTION PROTOTYPES -------------------------------------------------

    uint32_t pack_data(void *ret, uint32_t timestamp, uint32_t spacing, float *data, uint16_t num_datapoints);

#if defined(__cplusplus)
}
#endif // __cplusplus

#endif // __COMM_PROTOCOL_H__