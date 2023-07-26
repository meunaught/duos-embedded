#include <seven_segment.h>
#include <gpio.h>

/**
 * BREAD BOARD TO GPIO PIN MAPPING
 * TOP
 * 1 -> DOT
 * 2 -> L1 -> PC1
 * 4 -> H1 -> PC2
 * 5 -> R1 -> PC3
 * BOTTOM
 * 1 -> L2 -> PC4
 * 2 -> H3 -> PC5
 * 4 -> R2 -> PC6
 * 5 -> H2 -> PC7
*/

static const int seven[][7] = {
	{3,6,0,0,0,0,0},
	{2,3,7,4,5,0,0},
	{2,3,7,6,5,0,0},
	{1,7,6,3,0,0,0},
	{2,1,7,6,5,0,0},
	{2,5,7,1,4,6,0},
	{3,6,2,0,0,0,0},
	{1,2,3,4,5,6,7},
	{2,7,3,6,1,5,0}
};

const uint16_t pin_mask_all = (1U << 10) - 1; /* Select all pins from 0 to 10*/

void module_init(void) {
    RCC->AHB1ENR |= (1<<RCC_AHB1ENR_GPIOCEN_Pos); //Enable clock GPIOC

	GPIO_InitTypeDef config;
	config.Mode = GPIO_MODE_OUTPUT_PP;
	config.Pull = GPIO_NOPULL;
	config.Speed = GPIO_SPEED_FREQ_HIGH;
	config.Pin= pin_mask_all;

	GPIO_Init(GPIOC, &config);
}

void module_exit(void) {
    //pore dekhtesi
}

uint32_t lit_digit(const char d) {
    int i = d - '0' - 1;
    uint16_t pin_mask = 0U;
    for(int j = 0; j < 7; ++j) {
        pin_mask |= (1U << seven[i][j]);
    }
    GPIO_WritePin(GPIOC, pin_mask_all, GPIO_PIN_RESET);
    if(i < 0 || i >= 9) {
        return 1;
    }
    GPIO_WritePin(GPIOC, pin_mask, GPIO_PIN_SET);
    return 0;
}
