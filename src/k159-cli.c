/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <libusb-1.0/libusb.h>

#include "k159.h"
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
static libusb_device_handle * handle = NULL;
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
void
usb_control_transfer
(
    uint8_t request_type,
    uint8_t request,
    uint16_t wValue,
    uint16_t wIndex,
    uint8_t *data,
    uint16_t wLen
)
{
    libusb_control_transfer(handle, request_type, request, wValue, wIndex, data, wLen, 1000); 
}
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
int
main
(
    int argc,
    char **argv
)
{
    int ret;
    struct k159_context k159_ctx;
    size_t list;
    uint8_t relay_values;

    libusb_device **devs;
    libusb_device *dev;
    libusb_context * context = NULL;
    struct libusb_device_descriptor desc;
    struct libusb_config_descriptor *config;
    const struct libusb_interface *inter;
    const struct libusb_interface_descriptor *interdesc;
    const struct libusb_endpoint_descriptor *endpointdesc;

    ret = libusb_init(&context);

    k159_ctx.usb_control_transfer = usb_control_transfer;

    if (argc == 2)
    {
        relay_values = strtoul(argv[1], NULL, 16);
    }

    if (ret < 0)
    {
        printf("Could not initialize libusb context\n");
        return -1;
    }

    list = libusb_get_device_list(context, &devs);

    if (list < 0)
    {
        libusb_free_device_list(devs, 1);
        libusb_exit(context);
        printf("Could not get list of usb devices\n");
        return -1;
    }

    for (size_t i = 0; i < list; i++)
    {
        uint8_t k159_manufacturer_string[64] = { 0 };
        uint8_t k159_product_string[64]      = { 0 };

        uint8_t manufacturer[64] = { 0 };
        uint8_t product[64]      = { 0 };

        uint8_t a;
        uint8_t b;

        int ret = libusb_get_device_descriptor(devs[i], &desc);

        if (ret < 0)
        {
            printf("Could not get device descriptor\n");
            return -1;
        }

        if ((desc.idVendor == k159_get_usb_vendor_id()) && (desc.idProduct == k159_get_usb_product_id()))
        {
            ret = libusb_open(devs[i], &handle);

            if (LIBUSB_SUCCESS != ret)
            {
                printf("Could not open usb device\n");
                return -1;
            }

            libusb_get_string_descriptor_ascii(handle, desc.iManufacturer, manufacturer, sizeof(manufacturer));
            libusb_get_string_descriptor_ascii(handle, desc.iProduct,      product,      sizeof(product));

            k159_get_manufacturer_string(k159_manufacturer_string, &a);
            k159_get_product_string(k159_product_string, &b);

            if ((strncmp(k159_manufacturer_string, manufacturer, sizeof(manufacturer)) == 0) &&
                (strncmp(k159_product_string, product, sizeof(product)) == 0))
            {
                dev = devs[i];
                printf("Device found!\n");
                break;
            }
        }
    }


    k159_set_relays(&k159_ctx, relay_values);

    libusb_free_device_list(devs, 1);
    libusb_exit(context);

    return 0;
}
/* -------------------------------------------------------------------------- */
