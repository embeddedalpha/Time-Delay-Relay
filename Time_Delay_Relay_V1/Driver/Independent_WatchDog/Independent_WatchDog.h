/*
 * Independent_WatchDog.h
 *
 *  Created on: May 21, 2025
 *      Author: kunal
 */

#ifndef INDEPENDENT_WATCHDOG_INDEPENDENT_WATCHDOG_H_
#define INDEPENDENT_WATCHDOG_INDEPENDENT_WATCHDOG_H_

#include "main.h"

#ifdef __cplusplus
extern "C" {
#endif


void IWDG_Init(uint32_t timeout_ms);

/**
 * @brief Kick (refresh) the IWDG to prevent reset
 */
void IWDG_Kick(void);


bool IWDG_Reset_Check(void);


#ifdef __cplusplus
}
#endif

#endif /* INDEPENDENT_WATCHDOG_INDEPENDENT_WATCHDOG_H_ */
