/**
 * plat-sc9850.c
 *
**/

#include <linux/delay.h>
#include <linux/gpio.h>
#include <linux/of_gpio.h>

#include "ff_log.h"
#include "ff_ctl.h"

# undef LOG_TAG
#define LOG_TAG "SC9850"

#define FF_COMPATIBLE_NODE "focaltech,fingerprint"

/*
 * Driver configuration. See ff_ctl.c
 */
extern ff_driver_config_t *g_config_t;

int ff_ctl_init_pins(int *irq_num)
{
    int err = 0, gpio;
    struct device_node *dev_node = NULL;
    enum of_gpio_flags flags;
    FF_LOGV("'%s' enter.", __func__);

    if (unlikely(!g_config_t)) {
        return (-ENOSYS);
    }

    /* Find device tree node. */
    dev_node = of_find_compatible_node(NULL, NULL, FF_COMPATIBLE_NODE);
    if (!dev_node) {
        FF_LOGE("of_find_compatible_node(.., '%s') failed.", FF_COMPATIBLE_NODE);
        return (-ENODEV);
    }    
	
	/* Initialize RST pin. */
    gpio = of_get_named_gpio_flags(dev_node, "reset-gpio", 0, &flags);
    //gpio = 131;
	FF_LOGI("reset_gpio = %d", gpio);
    if (gpio > 0) {
        g_config_t->gpio_rst_pin = gpio;
    }
    if (!gpio_is_valid(g_config_t->gpio_rst_pin)) {
        FF_LOGE("g_config_t->gpio_rst_pin(%d) is invalid.", g_config_t->gpio_rst_pin);
        return (-ENODEV);
    }
    err = gpio_request(g_config_t->gpio_rst_pin, "ff_gpio_rst_pin");
    if (err) {
        FF_LOGE("gpio_request(%d) = %d.", g_config_t->gpio_rst_pin, err);
        return err;
    }
    err = gpio_direction_output(g_config_t->gpio_rst_pin, 1);
    if (err) {
        FF_LOGE("gpio_direction_output(%d, 1) = %d.", g_config_t->gpio_rst_pin, err);
        return err;
    }
	
    /* Initialize INT pin. */
    gpio = of_get_named_gpio_flags(dev_node, "irq-gpio", 0, &flags);
    //gpio = 130;
	FF_LOGI("irq-gpio = %d", gpio);
    if (gpio > 0) {
        g_config_t->gpio_int_pin = gpio;
    }
#if 0
    if (!gpio_is_valid(g_config_t->gpio_int_pin)) {
        FF_LOGE("g_config_t->gpio_int_pin(%d) is invalid.", g_config_t->gpio_int_pin);
        return (-ENODEV);
    }
    err = gpio_request(g_config_t->gpio_int_pin, "ff_gpio_int_pin");
    if (err) {
        FF_LOGE("gpio_request(%d) = %d.", g_config_t->gpio_int_pin, err);
        return err;
    }
    err = gpio_direction_input(g_config_t->gpio_int_pin);
    if (err) {
        FF_LOGE("gpio_direction_input(%d) = %d.", g_config_t->gpio_int_pin, err);
        return err;
    }
#endif
    /* Retrieve the IRQ number. */
    *irq_num = gpio_to_irq(g_config_t->gpio_int_pin);
    if (*irq_num < 0) {
        FF_LOGE("gpio_to_irq(%d) failed.", g_config_t->gpio_int_pin);
        return (-EIO);
    } else {
        FF_LOGD("gpio_to_irq(%d) = %d.", g_config_t->gpio_int_pin, *irq_num);
    }
    FF_LOGV("'%s' leave.", __func__);
    return err;
}

int ff_ctl_free_pins(void)
{
    int err = 0;
    FF_LOGV("'%s' enter.", __func__);

    /* Release GPIO resources. */
    gpio_free(g_config_t->gpio_rst_pin);
//    gpio_free(g_config_t->gpio_int_pin);

    FF_LOGV("'%s' leave.", __func__);
    return err;
}

int ff_ctl_enable_spiclk(bool on)
{
    int err = 0;
    FF_LOGV("'%s' enter.", __func__);
    FF_LOGD("clock: '%s'.", on ? "enable" : "disabled");

    if (on) {
        // TODO:
    } else {
        // TODO:
    }

    FF_LOGV("'%s' leave.", __func__);
    return err;
}

int ff_ctl_enable_power(bool on)
{
    int err = 0;
    FF_LOGV("'%s' enter.", __func__);
    FF_LOGD("power: '%s'.", on ? "on" : "off");

    if (unlikely(!g_config_t)) {
        return (-ENOSYS);
    }

    FF_LOGV("'%s' leave.", __func__);
    return err;
}

int ff_ctl_reset_device(void)
{
    int err = 0;
    FF_LOGV("'%s' enter.", __func__);

    if (unlikely(!g_config_t)) {
        return (-ENOSYS);
    }

    /* 3-1: Pull down RST pin. */
    err = gpio_direction_output(g_config_t->gpio_rst_pin, 0);

    /* 3-2: Delay for 10ms. */
    mdelay(10);

    /* Pull up RST pin. */
    err = gpio_direction_output(g_config_t->gpio_rst_pin, 1);

    FF_LOGV("'%s' leave.", __func__);
    return err;
}

const char *ff_ctl_arch_str(void)
{
    return "SC9832E";
}

