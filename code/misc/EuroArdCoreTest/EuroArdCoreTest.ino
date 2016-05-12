/*
	   ___ __ __ ____   ___   ____ ____  ___      __  ___  ____    ___
	  /  _|  |  |    \ /   \ /    |    \|   \    /  ]/   \|    \  /  _]
	 /  [_|  |  |  D  |     |  o  |  D  |    \  /  /|     |  D  )/  [_
	|    _|  |  |    /|  O  |     |    /|  D  |/  / |  O  |    /|    _]
	|   [_|  :  |    \|     |  _  |    \|     /   \_|     |    \|   [_
	|     |     |  .  |     |  |  |  .  |     \     |     |  .  |     |
	|_____|\__,_|__|\_|\___/|__|__|__|\_|_____|\____|\___/|__|\_|_____|







**	References	**

D0/RX	-	Expansion pin 10 (MIDI IN)
D1/TX	-	Expansion pin 12 (MIDI OUT/THRU)
D2		-	Clock (digital input or output)
D3		-	Trigger/Gate 1 (digital input or output)
D4		-	Trigger/Gate 2 (digital input or output)
D5		-	Expansion
D6		-	Expansion
D7		-	Expansion
D8		-	Expansion
D9		-	Expansion
D10		-	USED (DAC SS)
D11		-	USED (DAC MISO)
D12		-	RESERVED (MOSI)
D13		-	USED (DAC SCK)
A0		-	Control 1
A1		-	Control 2
A2		-	Control 3 (CV)
A3		-	Control 4 (CV)
A4		-	Expansion (I2C SDA)
A5		-	Expansion (I2C SCL)
A6		-	Expansion
A7		-	Expansion



20 Objects ArdCore:

D2		-	Clock in (clkIn)
D3		-	Digital Out 1 (digPin3)
D4		-	Digital Out 2 (digPin4)
D5		-	DAC
D5		-	DAC
D5		-	DAC
D5		-	DAC
D5		-	DAC
D5		-	DAC
D5		-	DAC
D5		-	DAC
A0		-
A1		-
A2		-
A3		-

*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>


#include <SPI.h>
#include <DAC_MCP49xx.h>
DAC_MCP49xx dac(DAC_MCP49xx::MCP4922, 10); // DAC model, SS pin, LDAC pin


//#include <MCPDAC.h>


#define SCL_PIN 4
#define SDA_PIN 5
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address. Connected to I2C pins shared with DAC.




void setup() {

	// CS on pin 10, no LDAC pin (tie it to ground).
	//MCPDAC.begin(10);

	// Set the gain to "HIGH" mode - 0 to 4096mV.
	//MCPDAC.setGain(CHANNEL_A, GAIN_LOW);

	// Do not shut down channel A, but shut down channel B.
	//MCPDAC.shutdown(CHANNEL_A, false);
	//MCPDAC.shutdown(CHANNEL_B, false);

	dac.setBuffer(false);				//	Set FALSE for 5V vref.
	dac.setGain(2);						//	"1" for 5V vref. "2" for 2.5V vref.
	dac.setPortWrite(true);				//	Faster analog outs, but loses pin 7.

	lcd.begin(16, 2);					//	initialize the lcd for 16 chars 2 lines, turn on backlight
	lcd.backlight();					//	backlight on
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("INPUTS");

	pinMode(2, INPUT);
	pinMode(3, INPUT);
	pinMode(4, INPUT);

	pinMode(0, OUTPUT);
	pinMode(1, OUTPUT);
	pinMode(5, OUTPUT);
	pinMode(6, OUTPUT);
	pinMode(7, OUTPUT);
	pinMode(8, OUTPUT);
	pinMode(9, OUTPUT);



	int volts = 0;
	byte v;
}

//  ==================== start of loop() =======================
void loop()
{
	/*digitalWrite(0, HIGH);
	digitalWrite(1, HIGH);
	digitalWrite(5, HIGH);
	digitalWrite(6, HIGH);
	digitalWrite(7, HIGH);
	digitalWrite(8, HIGH);
	digitalWrite(9, HIGH);

	int a = analogRead(4);
	int b = analogRead(5);
	int c = analogRead(6);
	int d = analogRead(7);
	dac.outputA((a+b+c+d)*4);
	delay(100);

	digitalWrite(0, LOW);
	digitalWrite(1, LOW);
	digitalWrite(5, LOW);
	digitalWrite(6, LOW);
	digitalWrite(7, LOW);
	digitalWrite(8, LOW);
	digitalWrite(9, LOW);

	delay(100);*/

	/*lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("INPUTS");
	lcd.setCursor(10, 0);
	lcd.print(digitalRead(3));
	lcd.setCursor(12, 0);
	lcd.print(digitalRead(2));
	lcd.setCursor(14, 0);
	lcd.print(digitalRead(4));

	lcd.setCursor(0, 1);
	lcd.print(analogRead(A0));

	lcd.setCursor(4, 1);
	lcd.print(analogRead(A1));

	lcd.setCursor(8, 1);
	lcd.print(analogRead(A2));

	lcd.setCursor(12, 1);
	lcd.print(analogRead(A3));*/

	//static unsigned int volts = 0;

	// Set the voltage of channel A.
	//MCPDAC.setVoltage(CHANNEL_A, volts & 0x0fff);
	//MCPDAC.setVoltage(CHANNEL_B, volts & 0x0fff);

	/*for (short volts = 0; volts < 4096; volts++){
		byte v = volts / 16;
		dac.outputB(volts);
		dacOutput(v);
	}*/

	//dac.outputA(0);
	//delay(1000);
	//dac.outputA(4095);
	//delay(10000);


	//dac.outputA(volts);
	//dac.outputB(volts);

	// Increase the voltage in steps of 100mV.
	//volts += 1;

	//delay(0);
	//short r = random(4096);
	//short r8 = r / 16;
	///dac.outputA(r);
	//dac.outputB(r8);
	dac.outputA(analogRead(2)*4);
	dac.outputB(analogRead(3)*4);
	//dacOutput(r8);
	//delay(100);
}

//  =================== convenience routines ===================

//  isr() - quickly handle interrupts from the clock input
//  ------------------------------------------------------
void isr()
{
	// Note: you don't want to spend a lot of time here, because
	// it interrupts the activity of the rest of your program.
	// In most cases, you just want to set a variable and get
	// out.
	//clkState = HIGH;
}

//  dacOutput(byte) - deal with the DAC output
//  -----------------------------------------
void dacOutput(byte v)
{
	//PORTB = (PORTB & B11100000) | (v >> 3);
	//PORTD = (PORTD & B00011111) | ((v & B00000111) << 5);
	//short out = (v * 16);	//	Scale 8 bits to 12 bits.
	//short out = v << 4;	//	Bitwise scale 8 bit to 12 bits.
	dac.outputB(v*16);
}

//  ===================== end of program =======================