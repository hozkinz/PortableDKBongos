#include "Nintendo.h"
#include "Arduino.h"
#include "SD.h"
#define SD_ChipSelectPin 4  // Using digital pin 4 on Arduino Nano 328
#include "TMRpcm.h"
#include "SPI.h"


//Audacity Mixing Settings for .wavs
//Tracks > Mix -> Mix Stereo down to Mono    
//Audio Setup -> Audio Settings -> Project Rate (HZ) > set to 32000, 22050, 16000 or 11025 (16kHz recommended)
//File > Export > Export as WAV -> Save as type: Other uncompressed files
//Format Options -> Select WAVEX(Microsoft), Unsigned 8 bit PCM  
//Remove .wav from extension


CGamecubeController GamecubeController(7);
TMRpcm audio;  // create an object for use in this sketch
#define ENABLE_MULTI

//speed in which buttons can be last activated
unsigned long flDebouncetime = 0;
unsigned long flDelay = 125;
unsigned long frDebouncetime = 0;
unsigned long frDelay = 125;
unsigned long rlDebouncetime = 0;
unsigned long rlDelay = 125;
unsigned long rrDebouncetime = 0;
unsigned long rrDelay = 125;
unsigned long startDebouncetime = 0;
unsigned long startDelay = 125;
unsigned long claprDebouncetime = 0;
unsigned long claprDelay = 125;


void printDetail(uint8_t type, int value);

// Define a Gamecube Controller
// Pin definitions
#define pinLed LED_BUILTIN

void setup() {
  // Set up debug led
  pinMode(pinLed, OUTPUT);
// audio.setVolume(1); // doesnt work?
  audio.speakerPin = 9;  //5,6,11 or 46 on Mega, 9 on Uno, Nano, etc
  // Start debug serial
  //  while (!Serial)
  //   ;
  Serial.begin(115200);


  // Wait for some Serial input
  // Serial.println(F("Enter any key to start reading the controller."));
  // while (Serial.read() == -1);

  Serial.println(F("Starting controller emulation"));
  Serial.println();
  if (!SD.begin(SD_ChipSelectPin)) {  // see if the card is present and can be initialized:
                                      //  Serial.println("SD fail");
    return;                           // don't do anything more if not
  }
}
void loop() {
  // Try to read the controller data
  if (GamecubeController.read()) {
    auto status = GamecubeController.getStatus();
    auto report = GamecubeController.getReport();
    print_gc_report(report, status);
    delay(10);

    if (report.b) {
      Serial.println();
      if ((millis() - flDebouncetime) > flDelay) {
        audio.play("0001");
        Serial.println(F("left bongo has been pressed"));
        flDebouncetime = millis();
      }
    }
    if (report.a) {
      Serial.println();
      if ((millis() - frDebouncetime) > frDelay) {
        audio.play("0002");
        Serial.println(F("right bongo has been pressed"));
        delay(10);
        frDebouncetime = millis();
      }
    }
    if (report.y) {
      Serial.println();
      if ((millis() - rlDebouncetime) > rlDelay) {
        audio.play("0003");
        Serial.println(F("rear left bongo has been pressed"));
        delay(10);
        rlDebouncetime = millis();
      }
    }
    if (report.x) {
      Serial.println();
      if ((millis() - rrDebouncetime) > rrDelay) {
        audio.play("0004");
        Serial.println(F("rear right bongo has been pressed"));
        delay(10);
        rrDebouncetime = millis();
      }
    }
    if (report.start) {
      Serial.println();
      if ((millis() - startDebouncetime) > startDelay) {
        audio.play("0005");
        Serial.println(F("start has been pressed"));
        delay(10);
        startDebouncetime = millis();
      }
    }
    if ((report.right) > 100) {
      Serial.println();
      if ((millis() - claprDebouncetime) > claprDelay) {
        audio.play("0006");
        Serial.println(F("you have made a loud noise"));
        delay(10);
        claprDebouncetime = millis();
      }
    }
  } else {
    // Add debounce if reading failed
    Serial.println(F("Error reading Gamecube controller."));
    digitalWrite(pinLed, HIGH);
    delay(1000);
  }
  digitalWrite(pinLed, LOW);
}

void print_gc_report(Gamecube_Report_t& gc_report, Gamecube_Status_t& gc_status) {
}
