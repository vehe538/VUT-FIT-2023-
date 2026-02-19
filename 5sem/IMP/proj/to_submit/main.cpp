#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED dimensions
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// components addresses
#define OLED_ADDR 0x3C
#define LED_PIN 2
#define BTN_PIN 4

// initialize OLED (I2C)
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// pulse sensor pin
#define PULSE_PIN 34  // ESP32 ADC pin (GPIO 34)

// graphing variables
int x = 0;
int lastx = 0;
int lasty = 0;
unsigned long LastTime = 0;
bool BPMTiming = false;
bool BeatComplete = false;
int BPM = 0;
int amplitude = 0;

// thresholds - for detecting heartbeat
int UPPER_THRESHOLD  = 0;
int LOWER_THRESHOLD  = 0;

// minimum and maximum signal values measured
int SIGNAL_MIN = 4095;
int SIGNAL_MAX = 0;

// button state trackking
bool ButtonState = HIGH;


// function for calibration of the sensor for correct values
void calibration() {

    Serial.println("1. Place the finger on the ensor for 10 seconds...");
    delay(3000);

    // display 'waits' for user to place his finger on the sensor
    oled.setCursor(0, 0);
    oled.setTextSize(2);
    oled.setTextColor(SSD1306_WHITE);
    oled.print("Place\nfinger on\nthe sensor");
    oled.display();
    delay(5000);

    oled.clearDisplay();
    oled.setCursor(SCREEN_WIDTH / 3, SCREEN_HEIGHT / 4);
    oled.setTextSize(1);
    oled.setTextColor(SSD1306_WHITE);
    oled.print("Calibrating...");

    int sum = 0;
    int tmp = 0;

    oled.drawRect(5, 30, 118, 15, SSD1306_WHITE);  // Outer frame
    
    oled.display();

    for (int i = 0; i < 1000; i++) {

        tmp = analogRead(PULSE_PIN);
        if (tmp > 0) sum += tmp;
        if (tmp > SIGNAL_MAX) SIGNAL_MAX = tmp;
        if (tmp < SIGNAL_MIN) SIGNAL_MIN = tmp;

        if (i % 20 == 0) {
            int barWidth = map(i, 0, 1000, 0, 118);
            
            // clear progress area
            oled.fillRect(5, 30, 118, 15, SSD1306_BLACK);
            
            // redraw frame
            oled.drawRect(5, 30, 118, 15, SSD1306_WHITE);
            
            // fill progress
            oled.fillRect(5, 30, barWidth, 15, SSD1306_WHITE);
            
            // clear time area
            oled.fillRect(50, 50, 48, 10, SSD1306_BLACK);

            // set correct cursor coords and print remaining time
            oled.setCursor(50, 50);
            oled.print("Time:");
            oled.print((10000 - (i * 10)) / 1000);
            oled.print("s");
            
            oled.display();
        }
        delay(10);
    }

    int avg = sum / 1000;

    Serial.println("--------------------");
    Serial.print("\n Average signal value: ");
    Serial.println(avg);

    Serial.print("\n Max signal value: ");
    Serial.println(SIGNAL_MAX);

    Serial.print("\n Min signal value: ");
    Serial.println(SIGNAL_MIN);

    Serial.print("\nAmplitude: ");
    Serial.println(amplitude);
    Serial.println("--------------------");


    // upper & lower thresholds and amplitude calculations
    amplitude = SIGNAL_MAX - SIGNAL_MIN;
    UPPER_THRESHOLD = SIGNAL_MIN + (amplitude * 0.65);
    LOWER_THRESHOLD = avg;

    Serial.print("Upper threshold will be: ");
    Serial.println(UPPER_THRESHOLD);
    Serial.print("Lower threshold will be: ");
    Serial.println(LOWER_THRESHOLD);

}

void setup() {
    
    Serial.begin(115200);

    // OLED pins config
    Wire.begin(19, 18);  // SDA=GPIO19, SCL=GPIO18    
    //  OLED init
    if(!oled.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
        Serial.println(F("SSD1306 allocation failed"));
        for(;;); 
    }
    oled.clearDisplay();

    // LED and button pin setup
    pinMode(LED_PIN, OUTPUT);           
    pinMode(BTN_PIN, INPUT_PULLUP); 

    // configure ADC for ESP32
    analogReadResolution(12);       // 12-bit resolution (0-4095)
    analogSetAttenuation(ADC_11db); // 0-3.3V range

    calibration();  
    oled.clearDisplay();

    Serial.println("ESP32 Pulse Monitor Ready");
}

void loop() {

    // reset graph when we reach right edge
    if(x > SCREEN_WIDTH - 1) {
        oled.clearDisplay();
        x = 0;
        lastx = x;
        lasty = 0;
    }
    
    
    // track button state for recalibration
    int btn = digitalRead(BTN_PIN);
    if (btn != ButtonState) { 
        oled.clearDisplay();
        calibration();
        oled.clearDisplay();
        ButtonState = HIGH;
    }

    // read pulse sensor
    int signal = analogRead(PULSE_PIN);
    
    // map signal to screen height (invert because y=0 is top)
    int y = SCREEN_HEIGHT - 1 - map(signal, SIGNAL_MIN, SIGNAL_MAX, SCREEN_HEIGHT/2, SCREEN_HEIGHT - 10);

    y = constrain(y, 0, SCREEN_HEIGHT - 1);
    
    // draw line from last point to current point
    if(x > 0) {
        oled.drawLine(lastx, lasty, x, y, SSD1306_WHITE);
    }
    
    // store for next line
    lasty = y;
    lastx = x;
    
    // HEARTBEAT DETECTION
    if(signal > UPPER_THRESHOLD) {
        if(BeatComplete) {
            digitalWrite(LED_PIN, HIGH);
            unsigned long interval = millis() - LastTime;
            if(interval > 0) {
                BPM = 60000 / interval;  // convert ms to BPM
                BPMTiming = false;
                BeatComplete = false;
                
                // debug output
                Serial.print("Beat detected! Interval: ");
                Serial.print(interval);
                Serial.print("ms, BPM: ");
                BPM < 40 ? Serial.println("--") : Serial.println(BPM);
            }
        }
        
        if(BPMTiming == false) {
            LastTime = millis();
            BPMTiming = true;
        }
    }
    
    // if lower theshold is passed, beat is complete and LED is turned off
    if((signal < LOWER_THRESHOLD) && BPMTiming) {
        BeatComplete = true;
        digitalWrite(LED_PIN, LOW);
    }
    
    // display BPM
    oled.fillRect(0, SCREEN_HEIGHT - 10, SCREEN_WIDTH, 10, SSD1306_BLACK);
    oled.setCursor(0, SCREEN_HEIGHT - 9);
    oled.setTextSize(1);
    oled.print("BPM: ");
    (BPM > 45 && BPM < 120) ? oled.print(BPM) : oled.print("--");
    
    // show current signal
    oled.setCursor(70, SCREEN_HEIGHT - 9);
    oled.print("Val: ");
    oled.print(signal);
    
    // update display
    oled.display();
    
    // increment x for next point
    x++;
    
    // ~50Hz sampling - 50 samples per second
    delay(20);  
}