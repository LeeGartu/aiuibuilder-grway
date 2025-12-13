#include "machine.h"
// #include <drivers/pin.h>

#define HEAT1_PIN 3,3//"PD.3"
#define HEAT2_PIN 3,2//"PD.2"
#define COOL1_PIN 3,6//"PD.6"
#define COOL2_PIN 3,7//"PD.7"
#define FAN1_PIN  0,2//"PA.2"
#define O_B_PIN  0,5//"PA.5"

struct mach_pin {
    uint8_t heat1 : 1;
    uint8_t heat2 : 1;
    uint8_t cool1 : 1;
    uint8_t cool2 : 1;
    uint8_t fan1 : 1;
    uint8_t o_b : 1;
};

struct mach_pin g_mach_pin = {0};
int g_mach_state = MACH_STATE_IDLE;
int g_mach_mode = MACH_MODE_AUTO;

void mach_output_pin(void)
{
    // hal_gpio_set_value(HEAT1_PIN, g_mach_pin.heat1);
    // hal_gpio_set_value(HEAT2_PIN, g_mach_pin.heat2);
    // hal_gpio_set_value(COOL1_PIN, g_mach_pin.cool1);
    // hal_gpio_set_value(COOL2_PIN, g_mach_pin.cool2);
    // hal_gpio_set_value(FAN1_PIN, g_mach_pin.fan1);
    // hal_gpio_set_value(O_B_PIN, g_mach_pin.o_b);
}

void mach_reset_pin(void)
{
    g_mach_pin.heat1 = 0;
    g_mach_pin.heat2 = 0;
    g_mach_pin.cool1 = 0;
    g_mach_pin.cool2 = 0;
    g_mach_pin.fan1 = 0;
    g_mach_pin.o_b = 0;
}

void mach_idel(void)
{
    g_mach_pin.heat1 = 0;
    g_mach_pin.heat2 = 0;
    g_mach_pin.cool1 = 0;
    g_mach_pin.cool2 = 0;
    g_mach_pin.fan1 = 0;
    g_mach_pin.o_b = 0;
}

void mach_work(void)
{
    switch (g_mach_mode) {
        case MACH_MODE_HEAT:
            g_mach_pin.heat1 = 1;
            break;
        case MACH_MODE_COOL:
            g_mach_pin.cool1 = 1;
            break;
        case MACH_MODE_FAN:
            g_mach_pin.fan1 = 1;
            break;
        default:
            break;
    }
}

void mach_thread_entry(void *arg)
{
    while (1) {
        mach_reset_pin();

        switch (g_mach_state) {
            case MACH_STATE_IDLE:
                mach_idel();
                break;
            case MACH_STATE_WORK:
                mach_work();
                break;
            default:
                break;
        }
        mach_output_pin();
    }
}