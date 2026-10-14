#include <zephyr/kernel.h>
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/hci.h>
#include <zephyr/sys/printk.h>

#define DEVICE_NAME "TEST_DEVICE"

static const uint8_t test_manufacturer_data[] = {
    0xFF, 0xFF,             /* 测试用 Company ID */
    0x01, 0x02, 0x03, 0x04  /* 自定义测试数据 */
};

static const struct bt_data ad[] = {
    BT_DATA_BYTES(BT_DATA_FLAGS,
                  (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),

    BT_DATA(BT_DATA_NAME_COMPLETE,
            DEVICE_NAME,
            sizeof(DEVICE_NAME) - 1),

    BT_DATA_BYTES(BT_DATA_UUID16_ALL, 0x3C, 0xFE),

    BT_DATA(BT_DATA_MANUFACTURER_DATA,
            test_manufacturer_data,
            sizeof(test_manufacturer_data)),
};

int main(void)
{
    int err;

    printk("Starting BLE test broadcaster\n");

    err = bt_enable(NULL);
    if (err) {
        printk("Bluetooth initialization failed: %d\n", err);
        return 0;
    }

    err = bt_le_adv_start(
        BT_LE_ADV_PARAM(
            BT_LE_ADV_OPT_USE_IDENTITY,
            BT_GAP_ADV_FAST_INT_MIN_2,
            BT_GAP_ADV_FAST_INT_MAX_2,
            NULL
        ),
        ad,
        ARRAY_SIZE(ad),
        NULL,
        0
    );

    if (err) {
        printk("Advertising start failed: %d\n", err);
        return 0;
    }

    printk("BLE advertising started\n");

    while (1) {
        k_sleep(K_SECONDS(10));
    }

    return 0;
}
