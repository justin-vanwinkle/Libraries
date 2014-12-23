/*
 * msp.c
 *
 *  Created on: Mar 20, 2012
 *      Author: RobG
 */

#include "msp.h"
#include "lcd.h"
#include "config.h"
#include "msp430.h"

//////////////////////
// MSP430 Init
//////////////////////
void initMSP430() {

	setUpClock();

	LCD_SCLK_OUT |= LCD_SCLK_PIN;
	LCD_SCLK_DIR |= LCD_SCLK_PIN;
	LCD_CS_OUT |= LCD_CS_PIN;
	LCD_CS_DIR |= LCD_CS_PIN;
	LCD_MOSI_OUT |= LCD_MOSI_PIN;
	LCD_MOSI_DIR |= LCD_MOSI_PIN;
	LCD_DC_OUT |= LCD_DC_PIN;
	LCD_DC_DIR |= LCD_DC_PIN;

//	configure SRAM/EEPROM pins
//	SRAM_CS_OUT |= SRAM_CS_PIN
//	SRAM_CS_DIR |= SRAM_CS_PIN
//	SRAM_HOLD_OUT |= SRAM_HOLD_PIN;
//	SRAM_HOLD_DIR |= SRAM_HOLD_PIN;
//	EEPROM_CS_OUT |= EEPROM_CS_PIN;
//	EEPROM_CS_DIR |= EEPROM_CS_PIN;
//	EEPROM_HOLD_OUT |= EEPROM_HOLD_PIN;
//	EEPROM_HOLD_DIR |= EEPROM_HOLD_PIN;
///////////////////////////

#ifdef HARDWARE_SPI
/////////////////////
// hardware SPI setup
/////////////////////

	portMapping();

// setup UCB0 or UCB1
	LCD_SPI_SEL |= LCD_SCLK_PIN + LCD_MOSI_PIN + LCD_MISO_PIN;
#if defined G2553_TI
	LCD_SPI_SEL2 |= LCD_SCLK_PIN + LCD_MOSI_PIN + LCD_MISO_PIN;
#endif

#if defined F5510_ROBG
	UCB1CTL0 = UCCKPH + UCMSB + UCMST + UCSYNC; // 3-pin, 8-bit SPI master
	UCB1CTL1 |= UCSSEL_2;// SMCLK
	UCB1BR0 |= 0x01;// 1:1
	UCB1BR1 = 0;
	UCB1CTL1 &= ~UCSWRST;// clear SW
#else
	UCB0CTL0 = UCCKPH + UCMSB + UCMST + UCSYNC; // 3-pin, 8-bit SPI master
	UCB0CTL1 |= UCSSEL_2; // SMCLK
	UCB0BR0 |= 0x01; // 1:1
	UCB0BR1 = 0;
	UCB0CTL1 &= ~UCSWRST; // clear SW
#endif

/////////////////////
#endif

}

/////////////////////////
// setup XT
/////////////////////////

void setUpClock() {

	setVcoreLevel(0x01);
	setVcoreLevel(0x02);
	setVcoreLevel(0x03);

#if defined F5172_ROBG

	PJSEL |= BIT4 | BIT5;

	UCSCTL6 &= ~XT1OFF;									// Enable XT1
	UCSCTL6 |= XTS;									// XT1 = HF mode
	UCSCTL3 = SELREF_0;									// FLL = XT1
	UCSCTL4 |= SELA_2;									// ACLK = REFO

	do {									// Loop until XT1 & DCO stabilizes
		UCSCTL7 &= ~(XT1HFOFFG + XT1LFOFFG + DCOFFG);// Clear XT1, DCO fault flags
		SFRIFG1 &= ~OFIFG;								// Clear osc fault flag
	} while (SFRIFG1 & OFIFG);

	UCSCTL6 &= ~XT1DRIVE_2;								// Lower XT1 drive
	UCSCTL4 = 0;								// xCLK = XT1

#elif defined F5529_ROBG || defined F5510_ROBG

///////// 4MHz XT2
//
//	P5SEL |= BIT2 + BIT3;								// Port select XT2
//
//	UCSCTL6 &= ~XT2OFF;// Enable XT2
//	UCSCTL3 |= SELREF_2;
//	UCSCTL4 |= SELA_2;// ACLK = REFO, SMCLK = DCO, MCLK = DCO
//
//	do {
//		UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + DCOFFG);
//		SFRIFG1 &= ~OFIFG;
//	} while (SFRIFG1 & OFIFG);
//
//	UCSCTL6 &= ~XT2DRIVE0;
//	UCSCTL4 |= SELS_5 + SELM_5;							// SMCLK = MCLK = XT2

//////// 25MHz DCO REFO
	UCSCTL3 = SELREF_2;// Set DCO FLL reference = REFO
	UCSCTL4 |= SELA_2;// Set ACLK = REFO

	__bis_SR_register(SCG0);
	UCSCTL0 = 0x0000;
	UCSCTL1 = DCORSEL_7;// 50MHz operation
	UCSCTL2 = FLLD_0 + 762;// (N + 1) * FLLRef = Fdco, (762 + 1) * 32768 = 25MHz, FLL Div = fDCOCLK/2
	__bic_SR_register(SCG0);
	__delay_cycles(782000);// 32 x 32 x 25 MHz / 32,768 Hz ~ 780k MCLK cycles for DCO to settle

	do {
		UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + DCOFFG);
		SFRIFG1 &= ~OFIFG;
	}while (SFRIFG1 & OFIFG);

#elif defined G2553_TI || defined G2955_ROBG

	BCSCTL1 = CALBC1_16MHZ; // 16MHz clock
	DCOCTL = CALDCO_16MHZ;

#else
// default DCO
#endif
}

void setVcoreLevel(unsigned int level) {
#if defined G2955_ROBG || defined G2553_TI
#else
	PMMCTL0_H = PMMPW_H;						// Open PMM registers for write
	SVSMHCTL = SVSHE + SVSHRVL0 * level + SVMHE + SVSMHRRL0 * level; // Set SVS/SVM high side new level
	SVSMLCTL = SVSLE + SVMLE + SVSMLRRL0 * level; // Set SVM low side to new level
	while ((PMMIFG & SVSMLDLYIFG)== 0);				// Wait till SVM is settled
	PMMIFG &= ~(SVMLVLRIFG + SVMLIFG);				// Clear already set flags
	PMMCTL0_L = PMMCOREV0 * level;					// Set VCore to new level
	if ((PMMIFG & SVMLIFG))
		while ((PMMIFG & SVMLVLRIFG)== 0);		// Wait till new level reached
	SVSMLCTL = SVSLE + SVSLRVL0 * level + SVMLE + SVSMLRRL0 * level; // Set SVS/SVM low side to new level
	PMMCTL0_H = 0x00;					// Lock PMM registers for write access
#endif
}

////////////////////////////////////
// port mapping
////////////////////////////////////

void portMapping() {
#if defined G2955_ROBG || defined G2553_TI
#else
	__disable_interrupt(); // Disable Interrupts before altering Port Mapping registers
	PMAPPWD = 0x02D52;   // Enable Write-access to modify port mapping registers

#ifdef PORT_MAP_RECFG
	PMAPCTL = PMAPRECFG;                 // Allow reconfiguration during runtime
#endif

#if defined F5172_ROBG

	P1MAP5 = PM_UCB0CLK;
	P1MAP6 = PM_UCB0SOMO;
	P1MAP7 = PM_UCB0SIMO;

#endif

	PMAPPWD = 0;        // Disable Write-Access to modify port mapping registers
#ifdef PORT_MAP_EINT
			__enable_interrupt();                    // Re-enable all interrupts
#endif
#endif
}
////////////////////////////////////
// write & delay
////////////////////////////////////

// code duplication, but we are saving clock cycles by not passing dataCommand
void writeData(u_char data) {
	LCD_SELECT;
#ifdef SPI9BIT
#ifdef HARDWARE_SPI
	LCD_DISABLE_HARDWARE_SPI;
#endif
	LCD_MOSI_HI;
	LCD_CLOCK;
#ifdef HARDWARE_SPI
	LCD_ENABLE_HARDWARE_SPI;
#endif
#else
	LCD_DC_HI;
#endif

#ifdef HARDWARE_SPI

	UCBxTXBUF = data;
	while (UCBxSTAT & UCBUSY)
		;

#else
	u_char c = 0;
	while (c < 8) {
		(data & 0x80) ? (LCD_MOSI_HI) : (LCD_MOSI_LO);
		LCD_CLOCK
		;
		data <<= 1;
		c++;
	}
#endif
	LCD_DESELECT;
}

// code duplication, but we are saving clock cycles by not passing dataCommand
void writeCommand(u_char command) {
	LCD_SELECT;
#ifdef SPI9BIT
#ifdef HARDWARE_SPI
	LCD_DISABLE_HARDWARE_SPI;
#endif
	LCD_MOSI_LO;
	LCD_CLOCK;
#ifdef HARDWARE_SPI
	LCD_ENABLE_HARDWARE_SPI;
#endif
#else
	LCD_DC_LO;
#endif

#ifdef HARDWARE_SPI

	UCBxTXBUF = command;
	while (UCBxSTAT & UCBUSY)
		;

#else
	u_char c = 0;
	while (c < 8) {
		(command & 0x80) ? (LCD_MOSI_HI) : (LCD_MOSI_LO);
		LCD_CLOCK
		;
		command <<= 1;
		c++;
	}
#endif
	LCD_DESELECT;
}

/*
 * long delay
 */
void delay(u_char x10ms) {
	while (x10ms > 0) {
		_delay_cycles(160000);
		x10ms--;
	}
}

