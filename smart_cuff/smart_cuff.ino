//AUTHOR: GUILLERMO PEREZ GUILLEN

#include <MCUFRIEND_kbv.h> // TFT library
MCUFRIEND_kbv tft;      // TFT instruction
#include <TouchScreen.h> // TFT library

#include <OneWire.h> // DS18B29 library                
#include <DallasTemperature.h> // DS18B29 library

OneWire ourWire(30);                // Pin 30 is set as OneWire bus
DallasTemperature sensors(&ourWire); // A variable or object is declared for our DS18B20 sensor

int16_t BOXSIZE; // TFT variable
uint16_t ID, currentcolor; // TFT variables
uint8_t Orientation = 0;    //PORTRAIT

// Assign human-readable names to some common 16-bit color values:
#define BLACK   0x0000 
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF 

#define PROCESSING_VISUALIZER 1
#define SERIAL_PLOTTER  2

//  Variables
int pulsePin = 8;                 // Pulse Sensor purple wire connected to analog pin 0
int blinkPin = 22;                // pin (13) to blink led at each beat
int fadePin = 24;                  // pin (5) to do fancy classy fading blink at each beat
int fadeRate = 0;                 // used to fade LED on with PWM on fadePin

// Volatile Variables, used in the interrupt service routine!
volatile int BPM;                   // int that holds raw Analog in 0. updated every 2mS
volatile int Signal;                // holds the incoming raw data
volatile int IBI = 600;             // int that holds the time interval between beats! Must be seeded!
volatile boolean Pulse = false;     // "True" when User's live heartbeat is detected. "False" when not a "live beat".
volatile boolean QS = false;        // becomes true when Arduoino finds a beat.

static int outputType = SERIAL_PLOTTER;

void setup(){
  pinMode(blinkPin,OUTPUT);         // pin that will blink to your heartbeat!
  pinMode(fadePin,OUTPUT);          // pin that will fade to your heartbeat!
  interruptSetup();                 // sets up to read Pulse Sensor signal every 2mS

    while (!Serial); 
  Serial.begin(115200);

    sensors.begin();   // DS18B20 sensor starts   
    uint16_t tmp;
    tft.reset(); 
    ID = tft.readID();
    tft.begin(ID); 
    tft.setRotation(Orientation); 
    tft.fillScreen(BLACK); 
}

void loop(){
    serialOutput() ;

  if (QS == true){     // A Heartbeat Was Found
                       // BPM and IBI have been Determined
                       // Quantified Self "QS" true when arduino finds a heartbeat
        fadeRate = 255;         // Makes the LED Fade Effect Happen
                                // Set 'fadeRate' Variable to 255 to fade LED with pulse
        serialOutputWhenBeatHappens();   // A Beat Happened, Output that to serial.
        QS = false;                      // reset the Quantified Self flag for next time
  }

  ledFadeToBeat();                      // Makes the LED Fade Effect Happen
  delay(20);                             //  take a break
}

void ledFadeToBeat(){
    fadeRate -= 15;                         //  set LED fade value
    fadeRate = constrain(fadeRate,0,255);   //  keep LED fade value from going into negative numbers!
    analogWrite(fadePin,fadeRate);          //  fade LED
  }
