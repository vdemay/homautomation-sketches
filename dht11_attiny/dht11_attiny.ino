#include <dht.h>
#include <avr/io.h>  
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

// Routines to set and claer bits (used in the sleep code)
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

// Variables for the Sleep/power down modes:
volatile boolean f_wdt = 1;

// PIN CONFIGURATIO
#define DHT11_PIN 1
#define LED_PIN 0

//DHT
dht DHT;

void setup()
{
  pinMode(LED_PIN, OUTPUT); 
  setup_watchdog(9); // approximately 8 seconds sleep
  delay(2000);
}

void loop()
{
  if (f_wdt==1) {  // wait for timed out watchdog / flag is set when a watchdog timeout occurs
    f_wdt=0;       // reset flag
    digitalWrite(LED_PIN, HIGH);
    delay (100);
    digitalWrite(LED_PIN, LOW);
    delay (100);
    
    // READ DATA
    int chk = DHT.read11(DHT11_PIN);
    switch (chk)
    {
      case DHTLIB_OK:  
                if (DHT.humidity > 40) {
                  digitalWrite(LED_PIN,HIGH);
                  delay(1000);
                } else {
                  digitalWrite(LED_PIN,LOW);
                }
  		break;
      case DHTLIB_ERROR_CHECKSUM: 
                digitalWrite(LED_PIN, HIGH);
                delay (100);
                digitalWrite(LED_PIN,LOW );
                delay (100);
                digitalWrite(LED_PIN, HIGH);
                delay (100);
                digitalWrite(LED_PIN,LOW );
                delay (100);
                digitalWrite(LED_PIN, HIGH);
                delay (100);
                digitalWrite(LED_PIN,LOW );
  		break;
      case DHTLIB_ERROR_TIMEOUT: 
                digitalWrite(LED_PIN, HIGH);
                delay (100);
                digitalWrite(LED_PIN,LOW );
                delay (100);
                digitalWrite(LED_PIN, HIGH);
                delay (100);
                digitalWrite(LED_PIN,LOW );
                delay (100);
                digitalWrite(LED_PIN, HIGH);
                delay (100);
                digitalWrite(LED_PIN,LOW );
  		break;
      default: 
                digitalWrite(LED_PIN, HIGH);
                delay (100);
                digitalWrite(LED_PIN,LOW );
                delay (100);
                digitalWrite(LED_PIN, HIGH);
                delay (100);
                digitalWrite(LED_PIN,LOW );
                delay (100);
                digitalWrite(LED_PIN, HIGH);
                delay (100);
                digitalWrite(LED_PIN,LOW );
  		break;
    }
    
    pinMode(LED_PIN, INPUT);
    system_sleep();  // Send the unit to sleep
    pinMode(LED_PIN, OUTPUT);
    
  }
}

/**********************************************************************/
/**                       ROUTINES FOR WATCHDOG                      **/
/**********************************************************************/

// set system into the sleep state 
// system wakes up when wtchdog is timed out
void system_sleep() {
  
  cbi(ADCSRA,ADEN);                    // switch Analog to Digitalconverter OFF

  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // sleep mode is set here
  sleep_enable();

  sleep_mode();                        // System actually sleeps here

  sleep_disable();                     // System continues execution here when watchdog timed out 
  
  sbi(ADCSRA,ADEN);                    // switch Analog to Digitalconverter ON
  
}

// 0=16ms, 1=32ms,2=64ms,3=128ms,4=250ms,5=500ms
// 6=1 sec,7=2 sec, 8=4 sec, 9= 8sec
void setup_watchdog(int ii) {

  byte bb;
  int ww;
  if (ii > 9 ) ii=9;
  bb=ii & 7;
  if (ii > 7) bb|= (1<<5);
  bb|= (1<<WDCE);
  ww=bb;

  MCUSR &= ~(1<<WDRF);
  // start timed sequence
  WDTCSR |= (1<<WDCE) | (1<<WDE);
  // set new watchdog timeout value
  WDTCSR = bb;
  WDTCSR |= _BV(WDIE);
}
  
// Watchdog Interrupt Service / is executed when watchdog timed out
ISR(WDT_vect) {
  f_wdt=1;  // set global flag
}
