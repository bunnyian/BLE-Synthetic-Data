/**
 * @file synth_data.c
 *
 * @brief source file with function run synth data
 *
 */

//* INCLUDES ------------------------------------------------------------------
#include <synth_data.h>
#include <stdlib.h>

#include <data_100Hz.h>
#include <data_50Hz.h>
#include <data_10Hz.h>
#include <data_4Hz.h>

#include <comm_protocol.h>
#include <ble_service.h>

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

//* DATA TYPES ------------------------------------------------------------------

//* MACROS ----------------------------------------------------------------------
LOG_MODULE_REGISTER(synth_data, LOG_LEVEL_DBG);

int END_TIME_SECONDS = 229;
bool BIOZ = false;

//* PRIVATE FUNCTION PROTOTYPES -------------------------------------------------
int notify_100hz(int second_count, void *packed_data)
{
    LOG_INF("100HZ: Notifying data");
    uint32_t timestamp;
    uint32_t spacing = (uint32_t)SPACING_100;
    float data_points[SAMPLES_PER_SECOND_100];

    timestamp = timestamps_100[second_count * SAMPLES_PER_SECOND_100];

    for (int j = 0; j < SAMPLES_PER_SECOND_100; j++)
    {
        data_points[j] = values_100[second_count * SAMPLES_PER_SECOND_100 + j];
    }

    uint32_t packed_size = pack_data(packed_data, timestamp, spacing, data_points, SAMPLES_PER_SECOND_100);

    int ret = ble_notify(1, packed_data, packed_size);

    if (ret != 0)
    {
        LOG_ERR("Failed to notify data");
        return -1;
    }

    LOG_INF("100HZ: Sent data for timestamp %d with size %d", timestamp, packed_size);
    return 0;
}

int notify_50hz(int second_count, void *packed_data)
{
    LOG_INF("50HZ: Notifying data");
    uint32_t timestamp;
    uint32_t spacing = (uint32_t)SPACING_50;
    float data_points[SAMPLES_PER_SECOND_50];

    timestamp = timestamps_50[second_count * SAMPLES_PER_SECOND_50];

    for (int j = 0; j < SAMPLES_PER_SECOND_50; j++)
    {
        data_points[j] = values_50[second_count * SAMPLES_PER_SECOND_50 + j];
    }

    uint32_t packed_size = pack_data(packed_data, timestamp, spacing, data_points, SAMPLES_PER_SECOND_50);

    int ret = ble_notify(2, packed_data, packed_size);

    if (ret != 0)
    {
        LOG_ERR("Failed to notify data");
        return -1;
    }

    LOG_INF("50HZ: Sent data for timestamp %d with size %d", timestamp, packed_size);
    return 0;
}

int notify_10hz(int second_count, void *packed_data)
{
    LOG_INF("10HZ: Notifying data");
    uint32_t timestamp;
    uint32_t spacing = (uint32_t)SPACING_10;
    float data_points[SAMPLES_PER_SECOND_10];

    timestamp = timestamps_10[second_count * SAMPLES_PER_SECOND_10];

    for (int j = 0; j < SAMPLES_PER_SECOND_10; j++)
    {
        data_points[j] = values_10[second_count * SAMPLES_PER_SECOND_10 + j];
    }

    uint32_t packed_size = pack_data(packed_data, timestamp, spacing, data_points, SAMPLES_PER_SECOND_10);

    int ret = ble_notify(3, packed_data, packed_size);

    if (ret != 0)
    {
        LOG_ERR("Failed to notify data");
        return -1;
    }

    LOG_INF("10HZ: Sent data for timestamp %d with size %d", timestamp, packed_size);
    return 0;
}

int notify_4hz(int second_count, void *packed_data)
{
    LOG_INF("4HZ: Notifying data");
    uint32_t timestamp;
    uint32_t spacing = (uint32_t)SPACING_4;
    float data_points[SAMPLES_PER_SECOND_4];

    timestamp = timestamps_4[second_count * SAMPLES_PER_SECOND_4];

    for (int j = 0; j < SAMPLES_PER_SECOND_4; j++)
    {
        data_points[j] = values_4[second_count * SAMPLES_PER_SECOND_4 + j];
    }

    uint32_t packed_size = pack_data(packed_data, timestamp, spacing, data_points, SAMPLES_PER_SECOND_4);

    int ret = ble_notify(4, packed_data, packed_size);

    if (ret != 0)
    {
        LOG_ERR("Failed to notify data");
        return -1;
    }

    LOG_INF("4HZ: Sent data for timestamp %d with size %d", timestamp, packed_size);
    return 0;
}

//* LOCAL VARIABLES ------------------------------------------------------------

volatile int second_count = 0;

void my_work_handler(struct k_work *work)
{
    if (!BIOZ) // if not bioz, then we are using synth data
    {
        void *packed_data_100 = k_malloc((sizeof(uint32_t) + sizeof(float) * SAMPLES_PER_SECOND_100));
        void *packed_data_50 = k_malloc((sizeof(uint32_t) + sizeof(float) * SAMPLES_PER_SECOND_50));
        void *packed_data_10 = k_malloc((sizeof(uint32_t) + sizeof(float) * SAMPLES_PER_SECOND_10));
        void *packed_data_4 = k_malloc((sizeof(uint32_t) + sizeof(float) * SAMPLES_PER_SECOND_4));

        if (packed_data_100 == NULL || packed_data_50 == NULL || packed_data_10 == NULL || packed_data_4 == NULL)
        {
            LOG_ERR("Failed to allocate memory for packed data");
        }
        else
        {
            LOG_INF("Allocated memory for packed data at %p", packed_data_100);
            LOG_INF("Allocated memory for packed data at %p", packed_data_50);
            LOG_INF("Allocated memory for packed data at %p", packed_data_10);
            LOG_INF("Allocated memory for packed data at %p", packed_data_4);
        }

        // LOG_INF("100Hz work handler");

        notify_100hz(second_count, packed_data_100);
        notify_50hz(second_count, packed_data_50);
        notify_10hz(second_count, packed_data_10);
        notify_4hz(second_count, packed_data_4);

        LOG_WRN("trying to free");

        k_free(packed_data_100);
        k_free(packed_data_50);
        k_free(packed_data_10);
        k_free(packed_data_4);

        second_count++;

        if (second_count >= (int)END_TIME_SECONDS)
        {
            second_count = 0;
            LOG_INF("Resetting second count");
        }
    }

    else
    {
        LOG_INF("BIOZ is enabled");

        int NUM_DATAPOINTS = 2;

        uint32_t timestamp = k_uptime_get_32();

        uint16_t freq = 100;
        float real = 55.123;
        float imag = 23.456;

        uint16_t freq2 = 50;
        float real2 = -777.777;
        float imag2 = 0.0;

        void *packed = k_malloc(sizeof(uint32_t) + (sizeof(uint16_t) + sizeof(float) + sizeof(float)) * NUM_DATAPOINTS);
        memmove(packed, &timestamp, sizeof(uint32_t));
        memmove(packed + sizeof(uint32_t), &freq, sizeof(uint16_t));
        memmove(packed + sizeof(uint32_t) + sizeof(uint16_t), &real, sizeof(float));
        memmove(packed + sizeof(uint32_t) + sizeof(uint16_t) + sizeof(float), &imag, sizeof(float));

        memmove(packed + sizeof(uint32_t) + sizeof(uint16_t) + sizeof(float) + sizeof(float), &freq2, sizeof(uint16_t));
        memmove(packed + sizeof(uint32_t) + sizeof(uint16_t) + sizeof(float) + sizeof(float) + sizeof(uint16_t), &real2, sizeof(float));
        memmove(packed + sizeof(uint32_t) + sizeof(uint16_t) + sizeof(float) + sizeof(float) + sizeof(uint16_t) + sizeof(float), &imag2, sizeof(float));

        int ret = ble_notify(1, packed, sizeof(uint32_t) + (sizeof(uint16_t) + sizeof(float) + sizeof(float)) * NUM_DATAPOINTS);
    }
}

K_WORK_DEFINE(my_work, my_work_handler);

//* PUBLIC FUNCTIONS -----------------------------------------------------------
void my_timer_handler(struct k_timer *dummy)
{
    LOG_INF("Timer expired");

    k_work_submit(&my_work);
    LOG_INF("Submitted work");
}
K_TIMER_DEFINE(my_timer, my_timer_handler, NULL);

int synth_data_main()
{
    k_timer_start(&my_timer, K_MSEC(1000), K_MSEC(1000));

    while (1)
    {
        int truth = 1;
        LOG_INF("");
        k_sleep(K_MSEC(1000));
    }

    return 0;
}
