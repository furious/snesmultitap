/*
 * SNES Multitap Device (0 Exit)
 * Developer: FURiOUS <furious.pro>
 * Version: 1.0
 * License: http://creativecommons.org/licenses/by-sa/3.0/legalcode
 * 
 * This firmware makes an arduino board imitate 2 multitaps, both with 2 controllers in the first ports, first controller is a normal gamepad, other 3 have specific button pressed
 */

/* Global Pin Variables */

/* Digital Pin Numbers */
#define CTRL_DPIN 10
#define DATA1_DPIN 9
#define DATA2_DPIN 8
#define DATA3_DPIN 3
#define DATA4_DPIN 4
#define CLOCK_DPIN 14
#define LATCH_DPIN 16
#define IO_DPIN 15
#define LED_DPIN 6
#define BTN_DPIN 2

/* Port Pin Numbers */
#define CTRL_PIN 6
#define DATA1_PIN 5
#define DATA2_PIN 4
#define DATA3_PIN 0
#define DATA4_PIN 4
#define CLOCK_PIN 3
#define LATCH_PIN 2
#define IO_PIN 1
#define LED_PIN 7
#define BTN_PIN 1

#define MULTITAP_BTN bitRead(PIND, BTN_PIN) == 0

/* Multitap Controllers Predefined Inputs */
const word controller[3] = {
  //byetUDLRaxlr0000
  0b1000001010100000, /* Port 1 Controller 2: A0 82 = A+L+B+Left */
  0b1010101010000000, /* Port 2 Controller 2: 80 AA = A+B+Select+Up+Left */
  0b0100100000100000, /* Port 2 Controller 1: 20 48 = L+Y+Up */
};

bool multitap = true, multitap_pressed = false;

#define WAIT_FALLING_EDGE(pin) while(!bitRead(PINB, pin) && multitap); while(bitRead(PINB, pin) && multitap);
#define WAIT_RISING_EDGE(pin) while(bitRead(PINB, pin) && multitap); while(!bitRead(PINB, pin) && multitap);

/* Setup everything */
void setup(){
  
  /* Setting Pin Modes */
  //pinMode(DATA1_DPIN, OUTPUT);
  pinMode(DATA2_DPIN, OUTPUT);
  pinMode(DATA3_DPIN, OUTPUT);
  pinMode(DATA4_DPIN, OUTPUT);

  //pinMode(CTRL_DPIN, INPUT);
  pinMode(CLOCK_DPIN, INPUT);
  pinMode(LATCH_DPIN, INPUT);
  pinMode(IO_DPIN, INPUT);
  pinMode(LED_DPIN, OUTPUT);
  pinMode(BTN_DPIN, INPUT_PULLUP);
  
  //digitalWrite(DATA1_DPIN, HIGH);
  digitalWrite(DATA2_DPIN, HIGH);
  digitalWrite(DATA3_DPIN, HIGH);
  digitalWrite(DATA4_DPIN, HIGH);
  digitalWrite(LED_DPIN, LOW);
}

/* Processes everything */
void loop(){

  noInterrupts();
  /* wait console poll the controllers */
  WAIT_FALLING_EDGE(LATCH_PIN); 
  
  bool pressed1 = false, pressed2 = false, pressed3 = false, pressed4 = false;

  /* process inputs */
  for(int8_t clocks = 0; clocks < 16; clocks++){
    int8_t btn = 0xF - clocks;

    //pressed1 = !bitRead(PINB, CTRL_PIN);
    pressed2 = controller[0] & (1 << btn);
    pressed3 = controller[1] & (1 << btn);
    pressed4 = controller[2] & (1 << btn);

  //bitWrite(PORTB, DATA1_PIN, !pressed1);
    bitWrite(PORTB, DATA2_PIN, !(pressed2 && multitap));
    bitWrite(PORTD, DATA3_PIN, !(pressed3 && multitap));
    bitWrite(PORTD, DATA4_PIN, !(pressed4 && multitap));

    bitWrite(PORTD, LED_PIN, multitap);

    /* wait for console poll next button */
    WAIT_RISING_EDGE(CLOCK_PIN);
  }
  
  interrupts();
}
