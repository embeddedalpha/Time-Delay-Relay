#include "Independent_WatchDog.h"

#define IWDG_KEY_ENABLE     0xCCCC
#define IWDG_KEY_ACCESS     0x5555
#define IWDG_KEY_RELOAD     0xAAAA

#define LSI_FREQUENCY_HZ    32000U  // Approximate, calibrate if needed

static uint32_t reload_value = 0;

typedef enum {
	IWDG_PRESCALER_4   = 0,
	IWDG_PRESCALER_8   = 1,
	IWDG_PRESCALER_16  = 2,
	IWDG_PRESCALER_32  = 3,
	IWDG_PRESCALER_64  = 4,
	IWDG_PRESCALER_128 = 5,
	IWDG_PRESCALER_256 = 6
} IWDG_Prescaler;




void IWDG_Init(uint32_t timeout_ms)
{
	// Enable write access to IWDG_PR and IWDG_RLR
	IWDG->KR = IWDG_KEY_ACCESS;

	// Prescaler selection (divide by 32 for good range: 1ms to ~1s)
	IWDG->PR = IWDG_PRESCALER_32;

	// Calculate reload value
	// Timeout = (reload + 1) * prescaler / LSI
	uint32_t prescaler_div = 32U;
	reload_value = ((LSI_FREQUENCY_HZ / prescaler_div) * timeout_ms) / 1000U;

	if (reload_value > 0x0FFF)
		reload_value = 0x0FFF;

	IWDG->RLR = reload_value;

	// Wait for registers to update
	while (IWDG->SR != 0);

	// Reload the counter
	IWDG->KR = IWDG_KEY_RELOAD;

	// Start the watchdog
	IWDG->KR = IWDG_KEY_ENABLE;
}

void IWDG_Kick(void)
{
	// Refresh watchdog counter
	IWDG->KR = IWDG_KEY_RELOAD;
}


bool IWDG_Reset_Check(void)
{
	static bool reset_check = 0;
	if ((RCC->CSR & RCC_CSR_IWDGRSTF) != 0u)
	{
		reset_check = 1;
	}
	else
	{
		reset_check = 0;
	}
	RCC->CSR |= RCC_CSR_RMVF;  /* clear reset flags */

	return reset_check;
}
