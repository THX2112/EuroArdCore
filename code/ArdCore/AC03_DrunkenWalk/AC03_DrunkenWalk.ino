//  ============================================================
//
//  Program: ArdCore DrunkenWalk
//
//  Description: Perform a drunken walk at full 8 bits
//
//  I/O Usage:
//    Knob 1: Speed, ranges from 0 to 1280 milliseconds
//    Knob 2: Jump amount, ranges from 1 to 64 steps
//    Analog In 1: Tranpose amount
//    Analog In 2: HIGH to allow duplicates
//    Digital Out 1: Trigger on output
//    Digital Out 2: unused
//    Clock In: External trigger of output
//    Analog Out: Drunken walk output, full 8-bit
//
//  Input Expander: unused
//  Output Expander: 8 bits of output exposed
//
//  Created:  28 Nov 2010
//  Modified: 05 Dec 2010  ddg Fixed looping problem
//            17 Jan 2011  ddg Change milli store to uint
//                             Reduce default trigtime to 10 ms
//            21 Feb 2011  ddg Addition of transposition on
//                             analog input 1, move duplicate
//                             detection to analog in 2.
//                             Inverted knob reads.
//                             Low interval set to trigger-only.
//            17 Apr 2012  ddg Updated for Arduino 1.0
//						18 Apr 2012	 ddg  Changed dacOutput routine to Alba version
//
//  ============================================================
//
//  License:
//
//  This software is licensed under the Creative Commons
//  "Attribution-NonCommercial license. This license allows you
//  to tweak and build upon the code for non-commercial purposes,
//  without the requirement to license derivative works on the
//  same terms. If you wish to use this (or derived) work for
//  commercial work, please contact 20 Objects LLC at our website
//  (www.20objects.com).
//
//  For more information on the Creative Commons CC BY-NC license,
//  visit http://creativecommons.org/licenses/
//
//  ================= start of global section ==================

#include <SPI.h>
#include <DAC_MCP49xx.h>
DAC_MCP49xx dac(DAC_MCP49xx::MCP4922, 10); // DAC model, SS pin, LDAC pin

//  constants related to the Arduino Nano pin use
const int clkIn = 2;           // the digital (clock) input
const int digPin[2] = {3, 4};  // the digital output pins
const int pinOffset = 5;       // the first DAC pin (from 5-12)
const int trigTime = 10;       // 10 millisecond trigger time

//  variables for interrupt handling of the clock input
volatile int clkState = LOW;

//  variables used to control the current DIO output states
int digState[2] = {LOW, LOW};  // start with both set low
long digMilli[2] = {0, 0};      // used for trigger timing

//  program variables of interest
int doStep = 0;                // Turn on to do a step
int randVal = 3;               // Gives us -1 to 1 to start
int outValue = 127;            // the DAC output value

//  variable for timing loops
long prevMillis = 0;           // the last time of a timed loop
int interval = 10;             // the last interval value

//  ==================== start of setup() ======================
//  The stock setup() routine - see ArdCore_Template for info.

void setup() {

	dac.setBuffer(true);
	dac.setPortWrite(true); //Faster analog outs, but loses pin 7.


  // set up the digital (clock) input
  pinMode(clkIn, INPUT);

  // set up the digital outputs
  for (int i=0; i<2; i++) {
	pinMode(digPin[i], OUTPUT);
	digitalWrite(digPin[i], LOW);
  }

  // set up the 8-bit DAC output pins
  //for (int i=0; i<8; i++) {
	//pinMode(pinOffset+i, OUTPUT);
	//digitalWrite(pinOffset+i, LOW);
  //}

  // set up clock interrupt
  attachInterrupt(0, isr, RISING);
}

//  ==================== start of loop() =======================

void loop()
{
  // service a clock trigger
  if (clkState == HIGH) {
	clkState = LOW;
	doStep = 1;
  }

  // test for a timer hit (no timer if too low)
  interval = ((1023 - analogRead(0)) >> 4) * 20;
  if ((interval < 1200) && ((millis() - prevMillis) > interval)) {
	prevMillis = millis();
	doStep = 1;
  }

  // do the random walk step
  if (doStep) {
	doStep = 0;
	int tmpOut = outValue + (analogRead(2) >> 2);
	if (tmpOut > 255)
	  tmpOut = 256 - (tmpOut - 255);
	dacOutput(tmpOut);

	digitalWrite(digPin[0], HIGH);
	digState[0] = HIGH;
	digMilli[0] = millis();

	// get the value jump
	randVal = ((analogRead(1) >> 6) * 2) + 3;

	// calculate the next value
	int newVal = outValue;
	if (analogRead(3) < 511) {
	  while (newVal == outValue)
		newVal = outValue + random(randVal) - (randVal / 2);
	} else {
	  newVal = outValue + random(randVal) - (randVal / 2);
	}
	outValue = newVal;

	// keep in in 8-bit range
	if (outValue > 255)
	  outValue = 256 - (outValue - 255);
	if (outValue < 0)
	  outValue = 0 - outValue;
  }

  // test for trigger turnoff
  if ((digState[0] == HIGH) && (millis() - digMilli[0] > trigTime)) {
	digitalWrite(digPin[0], LOW);
	digState[0] = LOW;
  }
}

//  ==================== additional routines ===================

//  isr() - quickly handle interrupts from the clock input
//  ------------------------------------------------------
void isr()
{
  clkState = HIGH;
}

//  dacOutput(byte) - deal with the DAC output
//  -----------------------------------------
void dacOutput(int v)
{
  //PORTB = (PORTB & B11100000) | (v >> 3);
	//PORTD = (PORTD & B00011111) | ((v & B00000111) << 5);

	dac.outputA(v*16);
	dac.outputB(v * 16);
}

//  ===================== end of program =======================
