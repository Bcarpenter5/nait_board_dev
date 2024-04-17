/** @defgroup flash_file FLASH peripheral API
 *
 * @ingroup peripheral_apis
 *
 * @brief <b>libopencm3 STM32G0xx FLASH</b>
 *
 * @version 1.0.0
 *
 * LGPL License Terms @ref lgpl_license
 */

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

/**@{*/

#include "flash.h"
#include "stm32g031xx.h"
#include <stdint.h>
#include <stdio.h>




void flash_prefetch_enable(void)
{
	// FLASH_ACR |= FLASH_ACR_PRFTEN;
    FLASH->ACR |= FLASH_ACR_PRFTEN;
}

void flash_prefetch_disable(void)
{
	// FLASH_ACR &= ~FLASH_ACR_PRFTEN;
    FLASH->ACR &= ~FLASH_ACR_PRFTEN;

}

void flash_set_ws(uint32_t ws)
{
	uint32_t reg32;

	reg32 = FLASH_ACR;
	// reg32 &= ~(FLASH_ACR_LATENCY_MASK << FLASH_ACR_LATENCY_SHIFT);
    // reg32 &= ~(FLASH_ACR_LATENCY_MASK << FLASH_ACR_LATENCY_SHIFT);
	// reg32 |= (ws << FLASH_ACR_LATENCY_SHIFT);
	// FLASH_ACR = reg32;
}

void flash_unlock_option_bytes(void)
{
    FLASH->OPTKEYR = FLASH_OPTKEYR_KEY1;
    FLASH->OPTKEYR = FLASH_OPTKEYR_KEY2;
}







/** @brief Wait until Last Flash Operation has Ended */
void flash_wait_for_last_operation(void)
{
	while (FLASH->SR & FLASH_SR_BSY1);
}

/** @brief Program a 64bits word to FLASH
 *
 * Program a 64bit word to FLASH memory.
 * Flash programing error must be checked and cleared if needed
 * before starting programming.
 *
 * @param[in] address Address in FLASH
 * @param[in] data Double word to write
 */
void flash_program_double_word(uint32_t * address, uint64_t data)
{
	flash_wait_for_last_operation();


if (((uint32_t)address) % 8 == 0) {
        FLASH->CR |= FLASH_CR_PG;
        // Program the first word
        *address = (uint32_t)data;
        // Program the second word
        *(address + 1) = (uint32_t)(data >> 32);

        // Wait for the operation to complete
        flash_wait_for_last_operation();

        // Check if the operation was successful
        if (FLASH->SR & FLASH_SR_EOP)
            FLASH->SR |= FLASH_SR_EOP;  // Clear the EOP bit in the Flash status register

        // Clear the PG bit in the Flash control register
        FLASH->CR &= ~FLASH_CR_PG;
    } else {
        printf("Error: Address is not 64-bit aligned\n");
    }
}

/** @brief Program a Data Block to FLASH
 *
 * This programs an arbitrary length data block to FLASH memory.
 * The program error flag should be checked separately for the event that memory
 * was not properly erased.
 *
 * @param[in] address Starting address in Flash.
 * @param[in] data Pointer to start of data block.
 * @param[in] len Length of data block in bytes (multiple of 8).
 **/
void flash_program(uint32_t * address, uint8_t *data, uint32_t len)
{
    uint32_t i;
    uint64_t *data64 = (uint64_t *)data;
    len = len % 8 == 0 ? len /8 : len / 8 + 1;

    for (i = 0; i < len; i++) {
        flash_program_double_word(address, data64[i]);
        address += 2;
    }

}

/** @brief Erase a page of FLASH
 * @param[in] page
 */
void flash_erase_page(uint32_t page)
{
    
	flash_wait_for_last_operation();
    
    uint32_t reg32 = FLASH->CR;
    //set the per bit
    FLASH->CR |= FLASH_CR_PER;
    // clear the address register
    FLASH->CR &=  ~(FLASH_CR_PNB_MASK << FLASH_CR_PNB_SHIFT);
	// FLASH->CR |= (page & FLASH->CR_PNB_MASK)  << FLASH->CR_PNB_SHIFT;
    FLASH->CR |= ((int)page & FLASH_CR_PNB_MASK)  << FLASH_CR_PNB_SHIFT;
    FLASH->CR |= FLASH_CR_STRT;

	flash_wait_for_last_operation();

	FLASH->CR &= ~FLASH_CR_PER;
}

/** @brief Erase All FLASH
 * This performs all operations necessary to erase all sectors in the FLASH
 * memory.
 */
void flash_erase_all_pages(void)
{
	flash_wait_for_last_operation();

	// FLASH->CR |= FLASH->CR_MER;
    // FLASH->CR |= FLASH_CR_MER;
	// FLASH->CR |= FLASH->CR_STRT;
	//
	// flash_wait_for_last_operation();
	// FLASH->CR &= ~FLASH->CR_MER;
}

/** @brief Clear the Programming Sequence Error Flag */
void flash_clear_pgserr_flag(void)
{
	FLASH->SR |= FLASH_SR_PGSERR;
}

/** @brief Clear the End of Operation Flag */
void flash_clear_eop_flag(void)
{
	FLASH->SR |= FLASH_SR_EOP;
}

/** @brief Clear programming size error flag */
void flash_clear_size_flag(void)
{
	FLASH->SR |= FLASH_SR_SIZERR;
}

/** @brief Clear the Programming Alignment Error Flag
 */
void flash_clear_pgaerr_flag(void)
{
	FLASH->SR |= FLASH_SR_PGAERR;
}

/** @brief Clear the Write Protected Error Flag
 */
void flash_clear_wrperr_flag(void)
{
	FLASH->SR |= FLASH_SR_WRPERR;
}

/** @brief Clear the Programming Error Status Flag
 */
void flash_clear_progerr_flag(void)
{
	FLASH->SR |= FLASH_SR_PROGERR;
}

/** @brief Clear the Operation Error Status Flag
 */
void flash_clear_operr_flag(void)
{
	FLASH->SR |= FLASH_SR_OPERR;
}

/** @brief Clear All Status Flags */
void flash_clear_status_flags(void)
{
	flash_clear_pgserr_flag();
	flash_clear_size_flag();
	flash_clear_pgaerr_flag();
	flash_clear_wrperr_flag();
	flash_clear_progerr_flag();
	flash_clear_eop_flag();
	flash_clear_operr_flag();
}

void flash_icache_enable(void)
{
	// FLASH->ACR |= FLASH->ACR_ICEN;
    FLASH->ACR |= FLASH_ACR_ICEN;
}

void flash_icache_disable(void)
{
	// FLASH->ACR &= ~FLASH->ACR_ICEN;
    FLASH->ACR &= ~FLASH_ACR_ICEN;
}

void flash_icache_reset(void)
{
	// FLASH->ACR |= FLASH->ACR_ICRST;
    FLASH->ACR |= FLASH_ACR_ICRST;
}

void flash_unlock_progmem(void)
{
	FLASH->KEYR = FLASH_KEYR_KEY1;
	FLASH->KEYR = FLASH_KEYR_KEY2;
}

void flash_lock_progmem(void)
{
	// FLASH->CR |= FLASH->CR_LOCK;
    FLASH->CR |= FLASH_CR_LOCK;
}

void flash_lock_option_bytes(void)
{
	// FLASH->CR |= FLASH->CR_OPTLOCK;
    FLASH->CR |= FLASH_CR_OPTLOCK;
}

void flash_unlock(void)
{
	flash_unlock_progmem();
	flash_unlock_option_bytes();
}

void flash_lock(void)
{
	// flash_lock_option_bytes();
	flash_lock_progmem();
}

/**@}*/
