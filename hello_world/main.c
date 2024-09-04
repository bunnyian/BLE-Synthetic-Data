#include <synth_data.h>
#include <ble_service.h>

#include <stdio.h>

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include <zephyr/types.h>

// bluetooth imports
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/hci.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);

int main()
{
    LOG_INF("starting main");

    ble_main();

    synth_data_main();

    return 0;
}