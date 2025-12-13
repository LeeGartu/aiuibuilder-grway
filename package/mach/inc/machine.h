#ifndef __MACHINE_H__
#define __MACHINE_H__

#include <stdint.h>

enum {
    MACH_STATE_IDLE,
    MACH_STATE_WORK,
};

enum {
    MACH_MODE_HEAT,
    MACH_MODE_COOL,
    MACH_MODE_FAN,
    MACH_MODE_AUTO,
};

#endif // !__MACHINE_H__
