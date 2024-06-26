/*
 * IRTimer.h
 *
 *  Copyright (C) 2020  Armin Joachimsmeyer
 *  armin.joachimsmeyer@gmail.com
 *
 *  This file is part of IRMP https://github.com/IRMP-org/IRMP.
 *
 *  IRMP is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *  See the GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/gpl.html>.
 *
 */

// NO GUARD here, we have the GUARD below with #ifdef _IRSND_H_ and #ifdef _IRMP_H_.
#include "stm32g031xx.h"

#if defined(_IRMP_H_)
void initIRTimerForReceive(void);
#endif

#if defined(_IRSND_H_)
extern void initIRTimerForSend(void);
#endif // defined(_IRSND_H_)

extern void irmp_timer_ISR(void);

extern void disableIRTimerInterrupt(void);
extern void enableIRTimerInterrupt(void);

extern void storeIRTimer(void);
extern void restoreIRTimer(void);
