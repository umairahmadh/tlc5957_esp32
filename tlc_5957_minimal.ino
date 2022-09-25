/******************************************************************************

    TLC5957_minimal.ino
        minimal usage for slight_TLC5957 library.
        this sketch just lights up all pixels after each other.
        debugout on usbserial interface: 115200baud

    hardware:
        Board:
            ItsyBitsy M4 (or compatible Port-Pin-Mapping)
            LED on pin 13
            TLC5957


    libraries used:
        ~ slight_TLC5957
            written by stefan krueger (s-light),
                git@s-light.eu, http://s-light.eu, https://github.com/s-light/
            license: MIT

    written by stefan krueger (s-light),
        git@s-light.eu, http://s-light.eu, https://github.com/s-light/

******************************************************************************/
/******************************************************************************
    The MIT License (MIT)

    Copyright (c) 2019 Stefan Krüger

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
******************************************************************************/

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Includes
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// use "file.h" for files in same directory as .ino
// #include "file.h"
// use <file.h> for files in library directory
// #include <file.h>

#include <SPI.h>
#include <slight_TLC5957.h>

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Info
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void sketchinfo_print(Print &out) {
  out.println();
  //             "|~~~~~~~~~|~~~~~~~~~|~~~..~~~|~~~~~~~~~|~~~~~~~~~|"
  out.println(F("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"));
  out.println(F("|                       ^ ^                      |"));
  out.println(F("|                      (0,0)                     |"));
  out.println(F("|                      ( _ )                     |"));
  out.println(F("|                       \" \"                      |"));
  out.println(F("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"));
  out.println(F("| TLC5957_minimal.ino"));
  out.println(F("|   minimal usage for slight_TLC5957 library."));
  out.println(F("|   this sketch just lights up all pixels after each other"));
  out.println(F("|"));
  out.println(F("| dream on & have fun :-)"));
  out.println(F("|"));
  out.println(F("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"));
  out.println(F("|"));
  //out.println(F("| compiled: Nov 11 2013  20:35:04"));
  out.print(F("| compiled: "));
  out.print(F(__DATE__));
  out.print(F("  "));
  out.print(F(__TIME__));
  out.println();
  out.print(F("| last changed: "));
  out.print(F(__TIMESTAMP__));
  out.println();
  out.println(F("|"));
  out.println(F("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"));
  out.println();

  // __DATE__ Nov 11 2013
  // __TIME__ 20:35:04
  // __TIMESTAMP__ Tue Dec 27 14:14:04 2016
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// definitions (global)
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Debug Output

boolean infoled_state = 0;
const byte infoled_pin = 13;

unsigned long debugOut_LiveSign_TimeStamp_LastAction = 0;
const uint16_t debugOut_LiveSign_UpdateInterval = 1000; //ms

boolean debugOut_LiveSign_Serial_Enabled = 0;
boolean debugOut_LiveSign_LED_Enabled = 1;


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TLC5957

// possible options and there defaults:
// slight_TLC5957(
//     uint8_t chip_count = 16,
//     uint8_t latch = A5,
//     uint8_t gsclk = 2,
//     uint8_t spi_clock = SCK,
//     uint8_t spi_mosi = MOSI,
//     uint8_t spi_miso = MISO
// );
uint8_t pixel_count = 2 * 16;
// use default pins
slight_TLC5957 tlc = slight_TLC5957(pixel_count);
#include "driver/ledc.h"
#include "driver/periph_ctrl.h"


void setup_D2_10MHz(void)
{
  periph_module_enable(PERIPH_LEDC_MODULE);

  // Set up timer
  ledc_timer_config_t ledc_timer = {


    // Clock frequency, 10 MHz, high speed
    .speed_mode = LEDC_HIGH_SPEED_MODE,
      // We need clock, not PWM so 1 bit is enough.
    .duty_resolution = LEDC_TIMER_1_BIT,
    .timer_num = LEDC_TIMER_0,
 
    .freq_hz = 10000000,
    // I think not needed for new esp-idf software, try uncommenting
    .clk_cfg = LEDC_USE_APB_CLK
  };

}

//void setup_D2_10MHz() {
//    // Activate timer TC1
//    // check for correct mask at MCLK – Main Clock - 15.7 Register Summary
//    // page178f
//    // http://ww1.microchip.com/downloads/en/DeviceDoc/60001507C.pdf#page=178&zoom=page-width,-8,539
//    // CLK_TC1_APB
//    MCLK->APBAMASK.reg |= MCLK_APBAMASK_TC1;
//
//    // for PCHCTRL numbers have a look at
//    // Table 14-9. PCHCTRLm Mapping page168ff
//    // http://ww1.microchip.com/downloads/en/DeviceDoc/60001507C.pdf#page=169&zoom=page-width,-8,696
//    // GCLK->PCHCTRL[GCLK_TC1].reg =
//    GCLK->PCHCTRL[9].reg =
//        // Enable the peripheral channel
//        GCLK_PCHCTRL_CHEN |
//        // Connect generic clock 7
//        GCLK_PCHCTRL_GEN_GCLK7;
//
//    // Enable the peripheral multiplexer on pin D2
//    PORT->Group[g_APinDescription[2].ulPort].
//        PINCFG[g_APinDescription[2].ulPin].bit.PMUXEN = 1;
//
//    // Set the D2 (PORT_PA07) peripheral multiplexer to
//    // peripheral (odd port number) E(6): TC1, Channel 1
//    // check if you need even or odd PMUX!!!
//    // http://forum.arduino.cc/index.php?topic=589655.msg4064311#msg4064311
//    PORT->Group[g_APinDescription[2].ulPort].
//        PMUX[g_APinDescription[2].ulPin >> 1].reg |= PORT_PMUX_PMUXO(4);
//
//    TC1->COUNT8.CTRLA.reg =
//        // Set prescaler to 2
//        // 120MHz/2 = 60MHz
//        TC_CTRLA_PRESCALER_DIV2 |
//        // Set the reset/reload to trigger on prescaler clock
//        TC_CTRLA_PRESCSYNC_PRESC;
//
//    // Set-up TC1 timer for
//    // Match Frequency Generation (MFRQ)
//    // the period time (T) is controlled by the CC0 register.
//    // (instead of PER or MAX)
//    // WO[0] toggles on each Update condition.
//    TC1->COUNT8.WAVE.reg = TC_WAVE_WAVEGEN_MFRQ;
//    // Wait for synchronization
//    // while (TC1->COUNT8.SYNCBUSY.bit.WAVE)
//
//    // Set-up the CC (counter compare), channel 0 register
//    // this sets the period
//    //
//    // (clockfreq / 2) / (CC0 + 1)  = outfreq  | * (CC0 + 1)
//    // (clockfreq / 2) = outfreq * (CC0 + 1)   | / outfreq
//    // (clockfreq / 2) / outfreq  = CC0 + 1    | -1
//    // ((clockfreq / 2) / outfreq) -1  = CC0
//    //
//    // ((60 / 2) / 2) -1  = CC0
//    //
//    // MAX: (60MHz / 2) / (0 + 1)  = 30MHz
//    // MIN: (60MHz / 2) / (255 + 1)  = 0,117MHz = 117kHz
//    //
//    //       60.0MHz
//    //   0 = 30.0MHz
//    //   1 = 15.0MHz
//    //   2 = 10.0MHz
//    //   3 =  7.5MHz
//    //   4 =  6.0MHz
//    //   5 =  5.0MHz
//    //   9 =  3.0MHz
//    //  14 =  2.0MHz
//    //  29 =  1.0MHz
//    //  59 =  0.5MHz
//    //  74 =  0.4MHz
//    //  99 =  0.3MHz
//    // 149 =  0.2MHz
//    // 255 =  0.11MHz
//    // start with 10MHz
//    TC1->COUNT8.CC[0].reg = 2;
//    // Wait for synchronization
//    while (TC1->COUNT8.SYNCBUSY.bit.CC1) {}
//
//    // Enable timer TC1
//    TC1->COUNT8.CTRLA.bit.ENABLE = 1;
//    // Wait for synchronization
//    while (TC1->COUNT8.SYNCBUSY.bit.ENABLE) {}
//}


unsigned long animation_timestamp = 0;
const uint16_t animation_interval = 1000; //ms

uint8_t step = 0;

uint16_t value_low = 0;
uint16_t value_high = 500;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// functions
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// TLC5957

void tlc_init(Print &out) {
  out.println(F("setup tlc:")); {
    setup_D2_10MHz();
    tlc.begin();
  }
  out.println(F("\t finished."));
}


void pixel_check() {
  if (step >= tlc.pixel_count) {
    step = 0;
    Serial.println("step wrap around.");
    tlc.set_pixel_all_16bit_value(value_low, value_low, value_low);
  }
  tlc.set_pixel_16bit_value(step, value_high, value_high, value_high);
  step += 1;
  Serial.print("step:");
  Serial.println(step);
  tlc.show();
}

void channel_check() {
  if (step >= tlc.channel_count) {
    step = 0;
    Serial.println("step wrap around.");
    tlc.set_pixel_all_16bit_value(value_low, value_low, value_low);
  }
  tlc.set_pixel_all_16bit_value(value_low, value_low, value_low);
  tlc.set_channel(step, value_high);
  step += 1;
  Serial.print("step:");
  Serial.println(step);
  tlc.show();
}

void update_animation() {
  if ((millis() - animation_timestamp) > animation_interval) {
    animation_timestamp = millis();
    // pixel_check();
    channel_check();
  }
}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// main
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void setup() {

  // initialise PINs
  pinMode(infoled_pin, OUTPUT);
  digitalWrite(infoled_pin, HIGH);

  // wait for arduino IDE to release all serial ports after upload.
  delay(1000);
  // initialise serial
  Serial.begin(115200);
  delay(1000);
  Serial.println();
  Serial.println();

  // print welcome
  sketchinfo_print(Serial);

  // setup TLC5957
  tlc_init(Serial);

  // go
  Serial.println(F("wait 0.5s."));
  delay(500);
  Serial.println(F("loop:"));
}

void loop() {
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // TLC5957

  update_animation();


  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // debug output
  if (
    (millis() - debugOut_LiveSign_TimeStamp_LastAction) >
    debugOut_LiveSign_UpdateInterval
  ) {
    debugOut_LiveSign_TimeStamp_LastAction = millis();

    if ( debugOut_LiveSign_Serial_Enabled ) {
      Serial.print(millis());
      Serial.print(F("ms;"));
    }

    if ( debugOut_LiveSign_LED_Enabled ) {
      infoled_state = ! infoled_state;
      if (infoled_state) {
        //set LED to HIGH
        digitalWrite(infoled_pin, HIGH);
      } else {
        //set LED to LOW
        digitalWrite(infoled_pin, LOW);
      }
    }

  }

}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// THE END
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
