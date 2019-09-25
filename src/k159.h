#if !defined(K159_H)
#define K159_H

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
#define RELAY_1 UINT8_C(0x01)
#define RELAY_2 UINT8_C(0x02)
#define RELAY_3 UINT8_C(0x04)
#define RELAY_4 UINT8_C(0x08)
#define RELAY_5 UINT8_C(0x10)
#define RELAY_6 UINT8_C(0x20)
#define RELAY_7 UINT8_C(0x40)
#define RELAY_8 UINT8_C(0x80)
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
typedef void (*usb_control_transfer_function_prototype) (uint8_t request_type, uint8_t request, uint16_t wValue, uint16_t wIndex, uint8_t *data, uint16_t wLen);
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
struct k159_context
{
    usb_control_transfer_function_prototype usb_control_transfer;
};
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/* -------------------------------------------------------------------------- */
uint16_t k159_get_usb_vendor_id();
uint16_t k159_get_usb_product_id();
void k159_get_manufacturer_string(uint8_t * const buffer, uint8_t * const size);
void k159_get_product_string(uint8_t * const buffer, uint8_t * const size);
void k159_get_relays(struct k159_context const * const k159_ctx, uint8_t * const relays);
void k159_set_relays(struct k159_context const * const k159_ctx, uint8_t relays);
/* -------------------------------------------------------------------------- */

#endif
