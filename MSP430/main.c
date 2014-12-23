/*
 * main.c
 *
 *  Created on: May 6, 2013
 *      Author: RobG
 */
#include "msp430.h"
#include "msp.h"
#include "lcd.h"
#include "graphics.h"
#include "color.h"
#include "typedefs.h"

u_char cc = 0;
u_char sx = 0;
u_char sy = 0;
u_char d = 0;
u_char o = 0;

u_char orientation = 0;

const u_char starty[13] = { 30, 48, 150, 50, 40, 85, 38, 105, 66, 130, 35, 71,
		117 };
const u_char startx[17] = { 180, 30, 100, 96, 48, 50, 150, 70, 40, 38, 123, 132,
		80, 35, 110, 55, 99 };
const u_char dim[9] = { 20, 14, 5, 17, 10, 5, 3, 18, 12 };
const u_char offset[10] = { 6, 4, 1, 9, 7, 2, 8, 0, 3, 5 };
void drawLogicLines(u_char h);
void shesGotColors(u_int repeatTimes);

void main(void) {

	WDTCTL = WDTPW + WDTHOLD; // disable WDT

	initMSP430();

	_delay_cycles(160000); // wait

	initLCD();

	while (1) {
		_delay_cycles(1600000);

		clearScreen(1);

		setColor(COLOR_16_RED);
		drawString(5, 5, FONT_SM, "Texas Instruments");
		setColor(COLOR_16_WHITE);
		drawString(5, 20, FONT_MD, "LaunchPad BoosterPack");
		setColor(COLOR_16_BLUE);
		drawString(5, 40, FONT_MD, "Library works with:");
		setColor(COLOR_16_YELLOW);
		drawString(5, 60, FONT_LG, "F5172 F5510 F5529");
		setColor(COLOR_16_GREEN_YELLOW);
		drawString(5, 80, FONT_LG, "G2553 G2955 & more");
		setColor(COLOR_16_ORANGE);
		drawString(5, 100, FONT_MD, "2.2\" 320 x 240 pixels display");
		setColor(COLOR_16_PURPLE);
		drawString(5, 120, FONT_MD, "and RobG's graphics library");

		_delay_cycles(64000000);

		shesGotColors(100);
		//
		_delay_cycles(25000000);
		//
		clearScreen(1);
		//
		drawLogicLines(8);

		_delay_cycles(16000000);

		setOrientation(++orientation & 0x03);

	}
}

///////////////////////////
// needed for test only
///////////////////////////
void drawLogicLines(u_char h) {
	u_char * addr = 0;
	addr += 0x1C00;
	u_int y = 0;
	u_char c = 1;
	while (y < getScreenHeight()) {
		setColor(colors[c]);
		drawLogicLine(1, y, getScreenWidth(), h, (u_char*) addr);
		y += (h + 2);
		c++;
		addr += getScreenWidth();
	}
}

u_int getColor() {
	cc++;
	if (cc == 43)
		cc = 0;
	return colors[cc];
}

u_char getX() {
	sx++;
	if (sx == 17)
		sx = 0;
	return startx[sx];
}

u_char getY() {
	sy++;
	if (sy == 13)
		sy = 0;
	return starty[sy];
}

u_char getD() {
	d++;
	if (d == 9)
		d = 0;
	return dim[d];
}

u_char getO() {
	o++;
	if (o == 10)
		o = 0;
	return offset[o];
}

void shesGotColors(u_int times) {

	u_char x;
	u_char y;
	u_char d;
	u_int repeatTimes = times;

	while (repeatTimes > 0) {
		setColor(getColor());
		x = getX();
		y = getY();
		d = getD();
		drawLine(x - d - getO(), y - d - getO(), x + d + getO(),
				y + d + getO());

		setColor(getColor());
		x = getX();
		y = getY();
		d = getD();
		fillRect(x - d - getO(), y - d - getO(), x + d + getO(),
				y + d + getO());

		setColor(getColor());
		x = getX();
		y = getY();
		d = getD();
		drawRect(x - d - getO(), y - d - getO(), x + d + getO(),
				y + d + getO());

		setColor(getColor());
		x = getX();
		y = getY();
		d = getD();
		fillCircle(x, y, d + getO());

		setColor(getColor());
		x = getX();
		y = getY();
		d = getD();
		drawLine(x - d - getO(), y + d + getO(), x + d + getO(),
				y - d - getO());

		setColor(getColor());
		x = getX();
		y = getY();
		d = getD();
		drawCircle(x, y, d + getO());

		repeatTimes--;
	}
}
/////////////////////////////////////////
// end of test
/////////////////////////////////////////

