//! @file
//!
//! Copyright (c) Memfault, Inc.
//! See License.txt for details
//!
//! Example app main

// clang-format off
#include "memfault/ports/zephyr/include_compatibility.h"

#include MEMFAULT_ZEPHYR_INCLUDE(device.h)
#include MEMFAULT_ZEPHYR_INCLUDE(devicetree.h)
#include MEMFAULT_ZEPHYR_INCLUDE(drivers/gpio.h)
#include MEMFAULT_ZEPHYR_INCLUDE(drivers/hwinfo.h)
#include MEMFAULT_ZEPHYR_INCLUDE(kernel.h)
#include MEMFAULT_ZEPHYR_INCLUDE(logging/log.h)
#include MEMFAULT_ZEPHYR_INCLUDE(shell/shell.h)

#include "app_version.h"

#include "cdr.h"
#include "memfault/components.h"
#include "memfault/ports/zephyr/core.h"
// clang-format on

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

// Blink code taken from the zephyr/samples/basic/blinky example.
static void blink_forever(void) {
#if CONFIG_QEMU_TARGET
  k_sleep(K_FOREVER);
#else
  /* 1000 msec = 1 sec */
  #define SLEEP_TIME_MS 1000

  /* The devicetree node identifier for the "led0" alias. */
  #define LED0_NODE DT_ALIAS(led0)

  static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

  int ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
  if (ret < 0) {
    return;
  }

  while (1) {
    gpio_pin_toggle_dt(&led);
    k_msleep(SLEEP_TIME_MS);
  }
#endif  // CONFIG_QEMU_TARGET
}

static const char *prv_get_device_id(void) {
  uint8_t dev_id[16] = { 0 };
  static char dev_id_str[sizeof(dev_id) * 2 + 1];
  static const char *dev_str = "UNKNOWN";

  // Obtain the device id
  ssize_t length = hwinfo_get_device_id(dev_id, sizeof(dev_id));

  // If this fails for some reason, use a fixed ID instead
  if (length <= 0) {
    dev_str = CONFIG_SOC_SERIES "-test";
    length = strlen(dev_str);
  } else {
    // Render the obtained serial number in hexadecimal representation
    for (size_t i = 0; i < length; i++) {
      (void)snprintf(&dev_id_str[i * 2], sizeof(dev_id_str), "%02x", dev_id[i]);
    }
    dev_str = dev_id_str;
  }

  return dev_str;
}

void memfault_platform_get_device_info(sMemfaultDeviceInfo *info) {
  *info = (sMemfaultDeviceInfo){
    .device_serial = prv_get_device_id(),
    .software_type = "zephyr-app",
    .software_version = APP_VERSION_STRING,
    .hardware_version = CONFIG_BOARD,
  };
}

#if defined(CONFIG_MEMFAULT_FAULT_HANDLER_RETURN)
  #include MEMFAULT_ZEPHYR_INCLUDE(fatal.h)
void k_sys_fatal_error_handler(unsigned int reason, const z_arch_esf_t *esf) {
  printk("User fatal handler invoked with reason: %d. Rebooting!\n", reason);
  memfault_platform_reboot();
}
#endif

int main(void) {
  printk(MEMFAULT_BANNER_COLORIZED);

  LOG_INF("👋 Memfault Demo App! Board %s\n", CONFIG_BOARD);
  memfault_device_info_dump();

  memfault_cdr_register_source(&g_custom_data_recording_source);

#if !defined(CONFIG_MEMFAULT_RECORD_REBOOT_ON_SYSTEM_INIT)
  memfault_zephyr_collect_reset_info();
#endif

  blink_forever();

  return 0;
}
