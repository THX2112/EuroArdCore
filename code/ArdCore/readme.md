**Converting ArdCore Code**
-----------------------

Install the [DAC_MCP49xx](http://github.com/exscape/electronics/tree/master/Arduino/Libraries/DAC_MCP49xx) Arduino [library](http://www.arduino.cc/en/Guide/Libraries).

Add this before Setup():

    #include <SPI.h> 
    #include <DAC_MCP49xx.h> 
    DAC_MCP49xx dac(DAC_MCP49xx::MCP4922, 10);

Add this to Setup():

    dac.setBuffer(true); 
    dac.setPortWrite(true); //Faster analog outs, but loses pin 7.

Change the "dacOutput" function:

    void dacOutput(byte v)
    {
    	short out = (v * 16);	//	Scale 8 bits to 12 bits.
    	dac.outputA(out);
    }


----------


**Reference:**
--------------
**EuroArdCore pins:**

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
 
**20 Objects ArdCore Pins:**
 
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


