/* Arduino USB Joystick HID demo */

/* Author: Darran Hunt
 * Modified for PPM inputs by Willian Galvani
 * Released into the public domain.
 */

#undef DEBUG

#define NUM_BUTTONS	40
#define NUM_AXES	8	       // 8 axes, X, Y, Z, etc

typedef struct joyReport_t {
    int16_t axis[NUM_AXES];
    uint8_t button[(NUM_BUTTONS+7)/8]; // 8 buttons per byte
} joyReport_t;

joyReport_t joyReport;


void setup(void);
void loop(void);
void setButton(joyReport_t *joy, uint8_t button);
void clearButton(joyReport_t *joy, uint8_t button);
void sendJoyReport(joyReport_t *report);



#define PPM_Pin 3   // Pin of PPM Input. Must be 2 or 3
int ppm[16];        // array for storing up to 16 servo signals

void setup() 
{
    Serial.begin(115200);
    delay(200);

    pinMode(PPM_Pin, INPUT);
    attachInterrupt(PPM_Pin - 2, read_ppm, CHANGE);

    TCCR1A = 0;  //reset timer1
    TCCR1B = 0;
    TCCR1B |= (1 << CS11);  //set timer1 to increment every 0,5 us

    for (uint8_t ind=0; ind<8; ind++) {
	    joyReport.axis[ind] = 0;
    }
    for (uint8_t ind=0; ind<sizeof(joyReport.button); ind++) {
        joyReport.button[ind] = 0;
    }
}



void read_ppm(){  //leave this alone
  static unsigned int pulse;
  static unsigned long counter;
  static byte channel;

  counter = TCNT1;
  TCNT1 = 0;

  if(counter < 1020){  //must be a pulse if less than 510us
    pulse = counter;
  }
  else if(counter > 3820){  //sync pulses over 1910us
    channel = 0;
  }
  else{  //servo values between 510us and 2420us will end up here
    ppm[channel] = (counter + pulse)/2;
    channel++;
  }
}


// Send an HID report to the USB interface
void sendJoyReport(struct joyReport_t *report)
{
#ifndef DEBUG
    Serial.write((uint8_t *)report, sizeof(joyReport_t));
#else
    // dump human readable output for debugging
    for (uint8_t ind=0; ind<NUM_AXES; ind++) {
	Serial.print("axis[");
	Serial.print(ind);
	Serial.print("]= ");
	Serial.print(report->axis[ind]);
	Serial.print(" ");
    }
    Serial.println();
    for (uint8_t ind=0; ind<NUM_BUTTONS/8; ind++) {
	Serial.print("button[");
	Serial.print(ind);
	Serial.print("]= ");
	Serial.print(report->button[ind], HEX);
	Serial.print(" ");
    }
    Serial.println();
#endif
}

// turn a button on
void setButton(joyReport_t *joy, uint8_t button)
{
    uint8_t index = button/8;
    uint8_t bit = button - 8*index;

    joy->button[index] |= 1 << bit;
}

// turn a button off
void clearButton(joyReport_t *joy, uint8_t button)
{
    uint8_t index = button/8;
    uint8_t bit = button - 8*index;

    joy->button[index] &= ~(1 << bit);
}

uint8_t button=0;	// current button
bool press = true;	// turn buttons on?

/* Turn each button on in sequence 1 - 40, then off 1 - 40
 * add values to each axis each loop
 */
void loop() 
{
    // Turn on a different button each time
    /*if (press) {
	setButton(&joyReport, button);
    } else {
	clearButton(&joyReport, button);
    }
*/
    /* Move all of the axes */
    for (uint8_t ind=0; ind<8; ind++) {
	joyReport.axis[ind] = map(ppm[ind], 990, 2010, -32000, 32000);
    }

    sendJoyReport(&joyReport);
    /*
    button++;
    if (button >= 40) {
       button = 0;
       press = !press;
    }*/
    delay(100);
}
