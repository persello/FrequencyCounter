#include <avr/io.h>
#include <avr/interrupt.h>
#include <Arduino.h>

#define MODE_FAST 0
#define MODE_SLOW 1

volatile uint32_t overflows = 0;
volatile uint32_t lastcount = 0;
volatile uint32_t cycles = 1;
double frequency = 0;
uint32_t lastCycles = 1;

void resetCounters()
{
  TCNT0 = 0;
  TCNT1 = 0;
}

void setup()
{

  Serial.begin(9600);

  // Setup Timer0: 1ms (@Fclk(I/O) = 16 MHz)

  TCCR0A = (1 << WGM01);            // CTC mode
  OCR0A = 250;                      // Compare A = 250 (= 1kHz)
  TIMSK0 = (1 << OCIE0A);           // Interrupt OC0A enable
  TCCR0B = (1 << CS01 | 1 << CS00); // Start with 64 prescaler

  // Counter input is pin D5 on Arduino Uno, T1 input (PD5)

  TCCR1A = 0;
  TCCR1B = (1 << CS12 | 1 << CS11 | 1 << CS10); // Clock source for Timer/Counter1 is T1, rising edge
  TIMSK1 = (1 << TOIE1);                        // Overflow interrupt enable
}

String unit = "";
void loop()
{

  frequency = (lastcount * 1000) / lastCycles;

  if (frequency < 1)
  {
    unit = "mHz";
    frequency *= 1000;
  }
  else if (frequency < 1000)
  {
    unit = "Hz";
  }
  else if (frequency < 1000000)
  {
    unit = "kHz";
    frequency /= 1000;
  }
  else
  {
    unit = "MHz";
    frequency /= 1000000;
  }

  Serial.print(frequency);
  Serial.print(" ");
  Serial.println(unit);
}

ISR(TIMER0_COMPA_vect) // Called 1000 times per second
{
  cli();
  if (TCNT1 > 1 || overflows > 0)
  {
    lastcount = TCNT1;              // Counter1 value is saved, overflows are calculated later
    TCNT1 = 0;                      // Quickly reset Counter1 value
    lastcount += overflows * 65536; // Now we have enought time to consider the overflows
    lastCycles = cycles;            // Saving the cycle count
    cycles = 1;                     // Resetting the internal cycle counter
    overflows = 0;                  // Overflow count reset
  } else {
    cycles++;
  }
  sei();
}

ISR(TIMER1_OVF_vect) // Called when Timer/Counter1 overflows (65535 -> 0)
{
  overflows++;
}