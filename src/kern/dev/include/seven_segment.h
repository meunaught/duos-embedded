#ifndef __SEVEN_SEGMENT_H
#define __SEVEN_SEGMENT_H
#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>

void module_init(void); //return 0 for successful otherwise unsuccessfull
void module_exit(void); //disable or offload the device
uint32_t lit_digit(const int);

#ifdef __cplusplus
}
#endif
#endif
