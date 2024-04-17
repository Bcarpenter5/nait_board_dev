/** @defgroup flash_defines FLASH Defines
 *
 * @ingroup STM32G0xx_defines
 *
 * @brief <b>Defined Constants and Types for the STM32G0xx Flash Control</b>
 *
 * @version 1.0.0
 *
 * LGPL License Terms @ref lgpl_license
 *  */
/*
 * This file is part of the libopencm3 project.
 *
 * This library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdint.h>
#include "stm32g031xx.h"

/**@{*/
#ifndef LIBOPENCM3_FLASH_H
#define LIBOPENCM3_FLASH_H

#define MMIO32(addr)	(addr)



#define FLASH_MEM_INTERFACE_BASE	(FLASH->ACR)

/** @defgroup flash_registers FLASH Registers
@{*/
#define FLASH_ACR			MMIO32(FLASH_MEM_INTERFACE_BASE + 0x00)
#define FLASH_KEYR			MMIO32(FLASH_MEM_INTERFACE_BASE + 0x08)
#define FLASH_OPTKEYR		MMIO32(FLASH_MEM_INTERFACE_BASE + 0x0c)
#define FLASH_SR			MMIO32(FLASH_MEM_INTERFACE_BASE + 0x10)
#define FLASH_CR			MMIO32(FLASH_MEM_INTERFACE_BASE + 0x14)
#define FLASH_ECCR			MMIO32(FLASH_MEM_INTERFACE_BASE + 0x18)
#define FLASH_OPTR			MMIO32(FLASH_MEM_INTERFACE_BASE + 0x20)
#define FLASH_PCROP1ASR		MMIO32(FLASH_MEM_INTERFACE_BASE + 0x24)
#define FLASH_PCROP1AER		MMIO32(FLASH_MEM_INTERFACE_BASE + 0x28)
#define FLASH_WRP1AR		MMIO32(FLASH_MEM_INTERFACE_BASE + 0x2c)
#define FLASH_WRP1BR		MMIO32(FLASH_MEM_INTERFACE_BASE + 0x30)
#define FLASH_PCROP1BSR		MMIO32(FLASH_MEM_INTERFACE_BASE + 0x34)
#define FLASH_PCROP1BER		MMIO32(FLASH_MEM_INTERFACE_BASE + 0x38)
#define FLASH_SECR			MMIO32(FLASH_MEM_INTERFACE_BASE + 0x80)
/**@}*/



#define FLASH_ACR_LATENCY_0WS		0x00
#define FLASH_ACR_LATENCY_1WS		0x01
#define FLASH_ACR_LATENCY_2WS		0x02
/**@}*/

/**@}*/

/** @defgroup flash_keyr KEYR Flash key register
@{*/

/** FLASH_KEYR_KEY1 Flash key 1 **/
#define FLASH_KEYR_KEY1			((uint32_t)0x45670123)
/** FLASH_KEYR_KEY2 Flash key 2 **/
#define FLASH_KEYR_KEY2			((uint32_t)0xcdef89ab)

/**@}*/

/** @defgroup flash_optkeyr OPTKEYR Option byte key register
@{*/

/** FLASH_OPTKEYR_KEY1 Option key 1 **/
#define FLASH_OPTKEYR_KEY1		((uint32_t)0x08192a3b)
/** FLASH_OPTKEYR_KEY2 Option key 2 **/
#define FLASH_OPTKEYR_KEY2		((uint32_t)0x4c5d6e7f)

/**@}*/

/** @defgroup flash_sr SR Status register
@{*/


#define FLASH_CR_PNB_SHIFT		3
#define FLASH_CR_PNB_MASK		0x3f


#define FLASH_ECCR_ADDR_ECC_SHIFT	0
#define FLASH_ECCR_ADDR_ECC_MASK	0x3fff

/**@}*/

/** @defgroup flash_optr OPTR Flash option register
@{*/


#define FLASH_OPTR_NRST_MODE_SHIFT	27
#define FLASH_OPTR_NRST_MODE_MASK	0x03
/** @defgroup flash_optr_nrst_mode NRST MODE
* @brief NRST_MODE
@{*/
#define FLASH_OPTR_NRST_MODE_RESET	1
#define FLASH_OPTR_NRST_MODE_GPIO	2
#define FLASH_OPTR_NRST_MODE_BIDIR	3
/**@}*/


#define FLASH_OPTR_BORR_LEV_SHIFT	11
#define FLASH_OPTR_BORR_LEV_MASK	0x03
/** @defgroup flash_optr_borr_lev BORR LEV
* @brief These bits contain the VDD supply level threshold that releases the reset.
@{*/
#define FLASH_OPTR_BORR_LEV_2V1		0
#define FLASH_OPTR_BORR_LEV_2V3		1
#define FLASH_OPTR_BORR_LEV_2V6		2
#define FLASH_OPTR_BORR_LEV_2V9		3
/**@}*/

#define FLASH_OPTR_BORF_LEV_SHIFT	9
#define FLASH_OPTR_BORF_LEV_MASK	0x03
/** @defgroup flash_optr_borf_lev BOR FLEV
* @brief These bits contain the VDD supply level threshold that activates the reset
@{*/
#define FLASH_OPTR_BORF_LEV_2V0		0
#define FLASH_OPTR_BORF_LEV_2V2		1
#define FLASH_OPTR_BORF_LEV_2V5		2
#define FLASH_OPTR_BORF_LEV_2V8		3
/**@}*/

/** FLASH_OPTR_BOREN BOR reset Level **/
#define FLASH_OPTR_BOREN			(1 << 8)

#define FLASH_OPTR_RDP_SHIFT		0
#define FLASH_OPTR_RDP_MASK			0xff
/** @defgroup flash_optr_rdp RDP
* @brief Read protection level
@{*/
#define FLASH_OPTR_RDP_LEVEL_0		0xAA
#define FLASH_OPTR_RDP_LEVEL_1		0xBB /* or any other value. */
#define FLASH_OPTR_RDP_LEVEL_2		0xCC
/**@}*/

/**@}*/

// BEGIN_DECLS
//
/**
 * This buffer is used for instruction fetches and may or may not be
 * enabled by default, depending on platform. (F1: yes, most others: no)
 * 
 * Note carefully the clock restrictions under which the prefetch buffer may be
 * enabled or disabled. Changes are normally made while the clock is running in
 * the power-on low frequency mode before being set to a higher speed mode.
 *
 * Note carefully that prefetch may also results in increased consumption
 * and can only improve performance on "mostly linear" workloads where there
 * is at least one flash wait state.
 *
 * See the reference manual for your particular target for more details.
 */
void flash_prefetch_enable(void);

/**
 * Note carefully the clock restrictions under which the prefetch buffer may be
 * set to disabled. See the reference manual for details.
 */
void flash_prefetch_disable(void);


/** Set the Number of Wait States.

Used to match the system clock to the FLASH memory access time. See the
programming manual for more information on clock speed ranges. The latency must
be changed to the appropriate value <b>before</b> any increase in clock
speed, or <b>after</b> any decrease in clock speed.

@param[in] ws values from @ref flash_latency.
*/
void flash_set_ws(uint32_t ws);

/** Lock the Flash Program and Erase Controller
 * Used to prevent spurious writes to FLASH.
 */
void flash_lock(void);

/** Unlock the Flash Program and Erase Controller
 * This enables write access to the Flash memory. It is locked by default on
 * reset.
 */
void flash_unlock(void);

/** Unlock the Option Byte Access.
 * This enables write access to the option bytes. It is locked by default on
 * reset.
 */
void flash_unlock_option_bytes(void);

void flash_clear_progerr_flag(void);
void flash_clear_pgserr_flag(void);
void flash_clear_size_flag(void);
void flash_clear_pgaerr_flag(void);
void flash_clear_wrperr_flag(void);
void flash_clear_operr_flag(void);
void flash_clear_eop_flag(void);

void flash_clear_status_flags(void);

void flash_wait_for_last_operation(void);

void flash_program_double_word(uint32_t * address, uint64_t data);
void flash_program(uint32_t * address, uint8_t *data, uint32_t len);

void flash_erase_page(uint32_t  page);
void flash_erase_all_pages(void);

/** Enable instruction cache */
void flash_icache_enable(void);
/** Disable instruction cache */
void flash_icache_disable(void);
/** Reset instruction cache */
void flash_icache_reset(void);

/** Unlock program memory */
void flash_unlock_progmem(void);
/** lock program memory */
void flash_lock_progmem(void);

/** Lock Option Byte Access */
void flash_lock_option_bytes(void);

// END_DECLS

#endif
/**@}*/
