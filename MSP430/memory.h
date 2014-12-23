/*
 * memory.h
 *
 *  Created on: Mar 20, 2012
 *      Author: RobG
 */

#ifndef MEMORY_H_
#define MEMORY_H_

#include "msp430.h"
#include "typedefs.h"

#define WRITE_STATUS	0x01	// SRAM & EEPROM
#define WRITE_MEMORY	0x02	// SRAM & EEPROM
#define READ_MEMORY		0x03	// SRAM & EEPROM
#define WRITE_DISABLE	0x04
#define READ_STATUS		0x05	// SRAM & EEPROM
#define WRITE_ENABLE	0x06
#define ERASE_PAGE		0x42
#define ERASE_SECTOR	0xD8
#define ERASE_CHIP		0xC7
#define DEEP_POWER_DOWN_OFF	0xAB
#define DEEP_POWER_DOWN_ON	0xB9

#define MODE_BYTE		0x00
#define MODE_PAGE		0x80
#define MODE_SEQUENTIAL	0x40
#define HOLD_BIT		0x01
#define WIP				0x01
#define WEL				0x02
#define BP0				0x04
#define BP1				0x08
#define WPEN			0x80

//
// TODO for now, memory uses bit bang, need to update to HW SPI
//
#define SRAM_CS_PIN 	BIT1
#define SRAM_CS_PORT	P1OUT
#define SRAM_HOLD_PIN 	BIT0
#define SRAM_HOLD_PORT	P1OUT
#define EEPROM_CS_PIN	BIT1 //BIT3
#define EEPROM_CS_PORT	P1OUT //P2OUT
#define EEPROM_HOLD_PIN BIT0
#define EEPROM_HOLD_PORT	P1OUT
#define MEMORY_SCK_PIN	BIT5
#define MEMORY_SI_PIN	BIT7
#define MEMORY_SO_PIN	BIT6
#define MEMORY_SPI_OUT	P1OUT
#define MEMORY_SPI_IN	P1IN

#define SRAM_SELECT		SRAM_CS_PORT &= ~SRAM_CS_PIN
#define SRAM_DESELECT	SRAM_CS_PORT |= SRAM_CS_PIN
#define SRAM_HOLD_ON	SRAM_HOLD_PORT &= ~SRAM_HOLD_PIN
#define SRAM_HOLD_OFF	SRAM_HOLD_PORT |= SRAM_HOLD_PIN
#define EEPROM_SELECT	EEPROM_CS_PORT &= ~EEPROM_CS_PIN
#define EEPROM_DESELECT	EEPROM_CS_PORT |= EEPROM_CS_PIN
#define EEPROM_HOLD_ON	EEPROM_HOLD_PORT &= ~EEPROM_HOLD_PIN
#define EEPROM_HOLD_OFF	EEPROM_HOLD_PORT |= EEPROM_HOLD_PIN

void sramWriteSequence(u_int addr);
void sramWriteByte(u_int addr, u_char byte);
void sramWriteStatus(u_char status);
void sramReadSequence(u_int addr);
u_char sramReadByte(u_int addr);
u_char sramReadStatus();

void eepromWriteSequence(u_int addr);
void eepromWriteByte(u_int addr, u_char byte);
void eepromWriteStatus(u_char status);
void eepromReadSequence(u_int addr);
u_char eepromReadByte(u_int addr);
u_char eepromReadStatus();
u_char eepromBusy();

u_char memoryReadWrite(u_char data);

#define sramRead() 		memoryReadWrite(0)
#define sramWrite(byte)	memoryReadWrite(byte)
#define sramEndSequence() SRAM_DESELECT
#define eepromRead() 		memoryReadWrite(0)
#define eepromWrite(byte)	memoryReadWrite(byte)
#define eepromEndSequence() EEPROM_DESELECT

u_char testSRAM();
u_char testEEPROM();

#endif /* MEMORY_H_ */
