/**
 * @file ble_service.c
 *
 * @brief ble service implementation for a custom notify service
 * resources used:
 * https://github.com/ljengic/diplomski/blob/feature/max30009/app/src/ble/src/ble_service.c
 * https://www.youtube.com/watch?v=o-NECS2TN94
 */

//* INCLUDES ------------------------------------------------------------------
#include <ble_service.h>

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/conn.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/hci.h>

//* PRIVATE FUNCTION PROTOTYPES ----------------------------------------------
static void notify_ccc_changed(const struct bt_gatt_attr *attr, uint16_t value);
static int init_ble(void);

//* MACROS --------------------------------------------------------------------
LOG_MODULE_REGISTER(ble_service, LOG_LEVEL_DBG);

BT_GATT_SERVICE_DEFINE(
	custom_service,
	BT_GATT_PRIMARY_SERVICE(BT_UUID_CUSTOM_SERVICE),
	BT_GATT_CHARACTERISTIC(BT_UUID_MY_CHARACTERISTIC, BT_GATT_CHRC_NOTIFY, BT_GATT_PERM_READ, NULL, NULL, NULL),
	BT_GATT_CCC(notify_ccc_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CHARACTERISTIC(BT_UUID_MY_CHARACTERISTIC_50, BT_GATT_CHRC_NOTIFY, BT_GATT_PERM_READ, NULL, NULL, NULL),
	BT_GATT_CCC(notify_ccc_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CHARACTERISTIC(BT_UUID_MY_CHARACTERISTIC_10, BT_GATT_CHRC_NOTIFY, BT_GATT_PERM_READ, NULL, NULL, NULL),
	BT_GATT_CCC(notify_ccc_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE),
	BT_GATT_CHARACTERISTIC(BT_UUID_MY_CHARACTERISTIC_4, BT_GATT_CHRC_NOTIFY, BT_GATT_PERM_READ, NULL, NULL, NULL),
	BT_GATT_CCC(notify_ccc_changed, BT_GATT_PERM_READ | BT_GATT_PERM_WRITE));

//* LOCAL VARIABLES ------------------------------------------------------------
static volatile bool ble_ready = false;		 // flag to check if ble is ready
static volatile bool notify_enabled = false; // flag to check if notifications are enabled

// advertising data
static const struct bt_data ad[] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
};

// scan response data
static const struct bt_data sd[] = {
	BT_DATA_BYTES(BT_DATA_UUID128_ALL, BT_UUID_CUSTOM_SERVICE_VAL),
};

//* PUBLIC FUNCTIONS -----------------------------------------------------------

/**
 * @brief sends a notification to the client
 *
 * @param stream_id for which characteristic of a service to send the notification, starting from 1
 * @param data pointer to the data to be sent
 * @param size size of the data to be sent in bytes
 * @return int 0 on success, -1 on failure
 */
int ble_notify(uint8_t stream_id, void *data, uint16_t size)
{
	if (!notify_enabled)
	{
		LOG_INF("Notifications are not enabled");
		return -1;
	}

	int real_index = 0;

	switch (stream_id)
	{
	case 1:
		real_index = 1;
		break;
	case 2:
		real_index = 5;
		break;
	case 3:
		real_index = 8;
		break;
	case 4:
		real_index = 11;
		break;
	default:
		LOG_ERR("Invalid stream_id");
		return -1;
	}

	LOG_INF("Sending notification");
	int ret = bt_gatt_notify(NULL, &custom_service.attrs[real_index], data, size);
	if (ret != 0)
	{
		LOG_ERR("Failed to notify at GATT layer (err %d), check MTU?", ret);
		return -1;
	}

	return 0;
}

/**
 * @brief main testing function for ble
 *
 * @return int 0 on success, 1 on failure
 */
int ble_main(void)
{
	init_ble();
	while (!ble_ready)
	{
		LOG_INF("Waiting for BLE to be ready");
		k_sleep(K_MSEC(100));
	}

	LOG_INF("BLE is ready");

	int err;
	err = bt_le_adv_start(BT_LE_ADV_CONN_NAME, ad, ARRAY_SIZE(ad), sd, ARRAY_SIZE(sd));
	if (err)
	{
		LOG_ERR("Advertising failed to start (err %d)", err);
		return 1;
	}

	return 0;
}

//* PRIVATE FUNCTIONS ----------------------------------------------------------
/**
 * @brief callback function for bt_enable
 *
 * @param err bt_enable exit code
 */
static void bt_ready(int err)
{
	if (err)
	{
		LOG_ERR("bt enable return (err %d)", err);
		return;
	}

	LOG_INF("Bluetooth initialized");
	ble_ready = true;
}

/**
 * @brief initializes bluetooth, callback is bt_ready
 *
 * @return int 0 on success, -1 on failure
 */
static int init_ble(void)
{
	LOG_INF("Initializing Bluetooth");

	int err;
	err = bt_enable(bt_ready); // call back is bt_ready

	if (err)
	{
		LOG_ERR("Bluetooth init failed (err %d)", err);
		return err;
	}

	int ret = bt_id_create(BT_ADDR_ANY, NULL); // randomize BT identity
	if (ret < 0)
	{
		LOG_ERR("bt_id_create failed (err %d)", ret);
		return ret;
	}
	else
	{
		LOG_INF("bt_id_create success");
	}
	return 0; // success
}

/**
 * @brief callback for a ccc (client characteristic configuration) being changed
 * updates the notification enabled bool
 *
 * @param attr
 * @param value
 */
static void notify_ccc_changed(const struct bt_gatt_attr *attr, uint16_t value)
{
	notify_enabled = (value == BT_GATT_CCC_NOTIFY);
}

/**
 * @warning deprecated callback function example for reading a characteristic
 * @brief callback for a sample characteristic being read
 * @param conn
 * @param attr
 * @param buf
 * @param len
 * @param offset
 * @return ssize_t
 */
static ssize_t my_read_char_function(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, uint16_t len, uint16_t offset)
{
	const char *value = "Hello World!";
	return bt_gatt_attr_read(conn, attr, buf, len, offset, value, strlen(value));
}
