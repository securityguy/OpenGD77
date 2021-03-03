/*
 * Copyright (C)2019 Kai Ludwig, DG4KLU
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _OPENGD77_WDOG_H_
#define _OPENGD77_WDOG_H_

#include "fsl_wdog.h"

#include "interfaces/adc.h"

typedef void (*batteryHistoryCallback_t)(int32_t);

extern volatile bool alive_maintask;
extern volatile bool alive_beeptask;
extern volatile bool alive_hrc6000task;

extern int batteryVoltage;
extern float averageBatteryVoltage;
extern bool headerRowIsDirty;

void init_watchdog(batteryHistoryCallback_t cb);
void fw_watchdog_task(void *data);
void tick_watchdog(void);
void watchdogReboot(void);

#endif /* _OPENGD77_WDOG_H_ */
