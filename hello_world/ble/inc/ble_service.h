#if !defined(__BLE_SERVICE_H__)
#define __BLE_SERVICE_H__

#if defined(__cplusplus)
extern "C"
{
#endif // __cplusplus

//* INCLUDES --------------------------------------------------------------------
#include <zephyr/types.h>
#include <zephyr/bluetooth/uuid.h>

//* DATA TYPES ------------------------------------------------------------------

//* MACROS ----------------------------------------------------------------------
// UUID of my custom services
#define BT_UUID_CUSTOM_SERVICE_VAL BT_UUID_128_ENCODE(0x1bded8de, 0x47ac, 0x4712, 0x94a8, 0xc8b99c843ddf)
#define BT_UUID_CUSTOM_SERVICE BT_UUID_DECLARE_128(BT_UUID_CUSTOM_SERVICE_VAL)

// UUID of my custom characteristic
#define BT_UUID_MY_CHARACTERISTIC_VAL BT_UUID_128_ENCODE(0x1bded8de, 0x47ac, 0x4712, 0x94a8, 0xFFFFFFFFF100)
#define BT_UUID_MY_CHARACTERISTIC BT_UUID_DECLARE_128(BT_UUID_MY_CHARACTERISTIC_VAL)

#define BT_UUID_MY_CHARACTERISTIC_VAL_50 BT_UUID_128_ENCODE(0x1bded8de, 0x47ac, 0x4712, 0x94a8, 0xFFFFFFFFFF50)
#define BT_UUID_MY_CHARACTERISTIC_50 BT_UUID_DECLARE_128(BT_UUID_MY_CHARACTERISTIC_VAL_50)

#define BT_UUID_MY_CHARACTERISTIC_VAL_10 BT_UUID_128_ENCODE(0x1bded8de, 0x47ac, 0x4712, 0x94a8, 0xFFFFFFFFFFF10)
#define BT_UUID_MY_CHARACTERISTIC_10 BT_UUID_DECLARE_128(BT_UUID_MY_CHARACTERISTIC_VAL_10)

#define BT_UUID_MY_CHARACTERISTIC_VAL_4 BT_UUID_128_ENCODE(0x1bded8de, 0x47ac, 0x4712, 0x94a8, 0xFFFFFFFFFFFF4)
#define BT_UUID_MY_CHARACTERISTIC_4 BT_UUID_DECLARE_128(BT_UUID_MY_CHARACTERISTIC_VAL_4)

    //* PUBLIC FUNCTION PROTOTYPES -------------------------------------------------
    int ble_main();

    int ble_notify(uint8_t stream_id, void *data, uint16_t size);

#if defined(__cplusplus)
}
#endif // __cplusplus

#endif // __BLE_SERVICE_H__
