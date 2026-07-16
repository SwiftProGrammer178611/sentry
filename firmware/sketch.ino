#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <Preferences.h>

#define pirPin 14
#define ldrPin 19
#define dhtPin 13
#define btn1 5
#define ledPinR 35
#define ledPinG 32
#define ledPinB 33
#define buzzer 26
#define OLED_SDA 25
#define OLED_SCL 15
#define darkLvl 1200

Adafruit_SSD1306 display(128,64,&Wire, -1);
DHT dht(dhtPin, DHT22)
Preferences prefs;

//project states:
enum State {IDLE, WAITING, STUDYING, BREAK, SLEEP}
State state = IDLE;

unsigned long stateStart = 0;
unsigned long lastMotion = 0;
unsigned long lastBtn1 = 0;
unsigned long lastDht = 0;

int focus = 0;
int streak = 0;
float temp = 0;
float hum = 0;

const unsigned long studyms = 25UL * 60000;
const unsigned long breakms = 5UL*60000;
const unsigned long sleepms = 10UL*60000;

bool pressed(int pin, unsigned long &last){
  if(digitalRead(pin) == LOW && milis() - last > 300){
    last = millis();
    return true;
  }
  return false;
}

void setColor(int r, int g, int b) {
  analogWrite(ledPinR, r);
  analogWrite(ledPinG, g);
  analogWrite(ledPinB, b);
}

void changeState(State state){
  state = state;
  stateStart = millis();
  if(state = STUDYING){
    focus = 100;
    tone(buzzer, 1000, 80);
  }
  if(state == BREAK) tone(buzzer, 1500, 150);
  if(state == SLEEP) tone(buzzer, 700, 300);
}

// Arduino Blink Example
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  // my stuff
  pinMode(pirPin, INPUT);
  pinMode(ldrPin, INPUT);
  pinMode(dhtPin, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(ledPinR, OUTPUT);
  pinMode(ledPinG, OUTPUT);
  pinMode(ledPinB, OUTPUT);

  Wire.begin(OLED_SDA,OLED_SCL);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  dht.begin();
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);

  bool motion = digitalRead(pirPin) == HIGH;
  if (motion) lastMotion = millis();

  int light = analogRead(ldrPin);
  bool dark = light < darkLvl;

  if(millis() - lastDht > 2500) {
    lastDht = millis();
    float t = dht.readTemperature();
    float h = dht.readHumidity();
    if(!isnan(t)) temp = t;
    if(!isnan(h)) hum = h;
  }

  bool b1 = pressed(btn1, lastBtn1);
  switch(state){
    case IDLE:
      if(motion) changeState(WAITING);
      break;
    case WAITING:
      if(!dark && (b1 || millis() - stateStart > 80000)) changeState(STUDYING);
      if(millis() - lastMotion > sleepms) changeState(SLEEP);
    case STUDYING:
      if(b1 || millis() - stateStart >= studyms){
        if(millis() - stateStart >= studyms) {
          streak++;
          prefs.begin("sb", false);
          prefs.putInt("streak", streak);
          prefs.end();
        }
        changeState(BREAK);
      }
      if(millis() - lastMotion > breakms) changeState(STUDYING);
      break;
    case BREAK:
      if(b1 || millis() - stateStart >= breakms) changeState(STUDYING);
      if(millis() - lastMotion > sleepms) changeState(SLEEP);
      break;
    case SLEEP:
      if(motion) changeState(IDLE);
      break;
  }
  if(state == STUDYING){
    if(focus>=70) setColor(0,150,0);
    else if(focus >= 40)setColor(150,120,0);
    else setColor(150,0,0);
  } else if(state == WAITING){
    bool blink = (millis() / 500) % 2 == 0;
    setColor(blink ? 150: 0, blink ? 150:0,0);
  }
  else if(state == BREAK){
    setColor(0, 0, 150);
  }else{
    setColor(0,0,0);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);  
  display.setCursor(0, 0);  

  switch(state){
    case IDLE:
      display.println("IDLE");
      display.print("Streak: ");
      display.println(streak);
      break;
    case WAITING:
      display.println(dark ? "TOO dark" : "get ready...");
      break;
    case STUDYING:
      unsigned long left = studyms-(millis() - stateStart);
      display.println("STUDYING");
      display.print(left/60000);
      display.print(":");
      display.println((left/60000) %60);
      display.print("Focus: ");
      display.println(focus);
      break;
    case BREAK:
      unsigned long left = breakms - (millis() - stateStart);
      display.println("BREAK");
      display.print(left/60000);
      display.print(":");
      display.println((left/60000) %60);
      break;
    case SLEEP:
      display.println("SLEEP")
      break;
  }
  display.display();
  delay(50);
}