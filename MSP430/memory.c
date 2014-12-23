/*
 * memory.c
 *
 *  Created on: Mar 20, 2012
 *      Author: RobG
 */
#include "memory.h"

u_char testSRAM() {
	u_char fail = 'F';
	// random numbers
	u_char byte1 = 0x3A;
	u_char byte2 = 0x5F;
	u_char byte3 = 0xC7;
	// set mode to byte
	sramWriteStatus(MODE_BYTE);
	// write 3 bytes
	sramWriteByte(0x0001, byte1);
	sramWriteByte(0x0002, byte2);
	sramWriteByte(0x0003, byte3);
	// read 3 bytes
	if (sramReadByte(0x0001) != byte1)
		return fail;
	if (sramReadByte(0x0002) != byte2)
		return fail;
	if (sramReadByte(0x0003) != byte3)
		return fail;
	// set mode to sequence
	sramWriteStatus(MODE_SEQUENTIAL);
	// start sequence
	sramWriteSequence(0x0004);
	// write sequence of bytes
	sramWrite(byte1);
	sramWrite(byte2);
	sramWrite(byte3);
	// end sequence
	sramEndSequence();
	// start sequence
	sramReadSequence(0x0004);
	// read sequence of bytes
	if (sramRead() != byte1)
		return fail;
	if (sramRead() != byte2)
		return fail;
	if (sramRead() != byte3)
		return fail;
	// end sequence
	sramEndSequence();
	// we are good :)
	return 'O';
}

u_char testEEPROM() {
	u_char fail = 'F';
	// random numbers
	u_char byte1 = 0x3A;
	u_char byte2 = 0x5F;
	u_char byte3 = 0xC7;
	// write 3 bytes
	eepromWriteByte(0x0001, byte1);
	eepromWriteByte(0x0002, byte2);
	eepromWriteByte(0x0003, byte3);
	// read 3 bytes
	if (eepromReadByte(0x0001) != byte1)
		return fail;
	if (eepromReadByte(0x0002) != byte2)
		return fail;
	if (eepromReadByte(0x0003) != byte3)
		return fail;
	// start sequence
	eepromWriteSequence(0x0004);
	// write sequence of bytes
	eepromWrite(byte1);
	eepromWrite(byte2);
	eepromWrite(byte3);
	// end sequence
	eepromEndSequence();
	// start sequence
	eepromReadSequence(0x0004);
	// read sequence of bytes
	if (eepromRead() != byte1)
		return fail;
	if (eepromRead() != byte2)
		return fail;
	if (eepromRead() != byte3)
		return fail;
	// end sequence
	eepromEndSequence();
	// we are good :)
	return 'O';
}

void eepromReadSequence(u_int addr) {
	while (eepromBusy())
		;
	EEPROM_SELECT;
	memoryReadWrite(READ_MEMORY);
	memoryReadWrite(addr >> 8);
	memoryReadWrite(addr);
}

u_char eepromReadByte(u_int addr) {
	while (eepromBusy())
		;
	EEPROM_SELECT;
	memoryReadWrite(READ_MEMORY);
	memoryReadWrite(addr >> 8);
	memoryReadWrite(addr);
	u_char byte = 0;
	byte = memoryReadWrite(0);
	EEPROM_DESELECT;
	return byte;
}

u_char eepromReadStatus() {
	EEPROM_SELECT;
	memoryReadWrite(READ_STATUS);
	u_char status = 0;
	status = memoryReadWrite(0);
	EEPROM_DESELECT;
	return status;
}

void eepromWriteSequence(u_int addr) {
	while (eepromBusy())
		;
	EEPROM_SELECT;
	memoryReadWrite(WRITE_ENABLE);
	EEPROM_DESELECT;
	EEPROM_SELECT;
	memoryReadWrite(WRITE_MEMORY);
	memoryReadWrite(addr >> 8);
	memoryReadWrite(addr);
}

void eepromWriteByte(u_int addr, u_char byte) {
	while (eepromBusy())
		;
	EEPROM_SELECT;
	memoryReadWrite(WRITE_ENABLE);
	EEPROM_DESELECT;
	EEPROM_SELECT;
	memoryReadWrite(WRITE_MEMORY);
	memoryReadWrite(addr >> 8);
	memoryReadWrite(addr);
	memoryReadWrite(byte);
	EEPROM_DESELECT;
}

void eepromWriteStatus(u_char status) {
	EEPROM_SELECT;
	memoryReadWrite(WRITE_STATUS);
	memoryReadWrite(status);
	EEPROM_DESELECT;
}

u_char eepromBusy() {
	return (eepromReadStatus() & WIP);
}

void sramReadSequence(u_int addr) {
	SRAM_SELECT;
	memoryReadWrite(READ_MEMORY);
	memoryReadWrite(addr >> 8);
	memoryReadWrite(addr);
}

u_char sramReadByte(u_int addr) {
	SRAM_SELECT;
	memoryReadWrite(READ_MEMORY);
	memoryReadWrite(addr >> 8);
	memoryReadWrite(addr);
	u_char byte = 0;
	byte = memoryReadWrite(0);
	SRAM_DESELECT;
	return byte;
}

u_char sramReadStatus() {
	SRAM_SELECT;
	memoryReadWrite(READ_STATUS);
	u_char status = 0;
	status = memoryReadWrite(0);
	SRAM_DESELECT;
	return status;
}

void sramWriteSequence(u_int addr) {
	SRAM_SELECT;
	memoryReadWrite(WRITE_MEMORY);
	memoryReadWrite(addr >> 8);
	memoryReadWrite(addr);
}

void sramWriteByte(u_int addr, u_char byte) {
	SRAM_SELECT;
	memoryReadWrite(WRITE_MEMORY);
	memoryReadWrite(addr >> 8);
	memoryReadWrite(addr);
	memoryReadWrite(byte);
	SRAM_DESELECT;
}

void sramWriteStatus(u_char status) {
	SRAM_SELECT;
	memoryReadWrite(WRITE_STATUS);
	memoryReadWrite(status);
	SRAM_DESELECT;
}

u_char memoryReadWrite(u_char data) {
	u_char c = 0;
	u_char a = 0;

	while (c < 8) {
		a <<= 1;
		(data & 0x80) ? (MEMORY_SPI_OUT |= MEMORY_SI_PIN) : (MEMORY_SPI_OUT &=
								~MEMORY_SI_PIN);
		MEMORY_SPI_OUT |= MEMORY_SCK_PIN;
		if (MEMORY_SPI_IN & MEMORY_SO_PIN)
			a |= 0x01;
		MEMORY_SPI_OUT &= ~MEMORY_SCK_PIN;
		data <<= 1;
		c++;
	}
	return a;
}

