#include <Adafruit_CircuitPlayground.h>
// do NOT include the standard NeoPixel library

#define NEOPIX_PIN      A2
#define NUM_PIXELS      16
#define ONBOARD_PIXELS  10
#define EXPLOSION_LOOPS 500
#define PREFIRE_LOOPS   1024
#define REST_MAX        128
#define TAP_HARDNESS    126
#define BRIGHTNESS      128

Adafruit_CPlay_NeoPixel strip = Adafruit_CPlay_NeoPixel(NUM_PIXELS, NEOPIX_PIN, NEO_GRBW + NEO_KHZ800);

bool fired, up, req_update;
uint8_t stage = REST_MAX - 1;
const uint8_t TOTAL = NUM_PIXELS + ONBOARD_PIXELS;
const uint8_t ARR_SIZE = TOTAL * 3;
uint8_t Colors[ARR_SIZE];
uint8_t cycle = 0;

void setup() {
  CircuitPlayground.begin(BRIGHTNESS);
  // Accelorometer
  CircuitPlayground.setAccelRange(LIS3DH_RANGE_2_G);
  CircuitPlayground.setAccelTap(1, TAP_HARDNESS);
  attachInterrupt(digitalPinToInterrupt(CPLAY_LIS3DH_INTERRUPT), Accel, FALLING);
  //Extenal Strip
  strip.begin();
  req_update = true;
  // debugging
  //Serial.begin(11520);
  //while (!Serial){;}
  //Serial.println("Ready!");
}

void loop() {
  if (fired){
    Explosion();
    fired = false;
  } else {
    Rest();
  }
}

void Display(){
  if (!req_update){
    return;
  }
  CircuitPlayground.clearPixels();
  strip.clear();
  for (int pixel = 0; pixel < ONBOARD_PIXELS; pixel++){
    CircuitPlayground.setPixelColor(pixel, Colors[pixel*3], Colors[pixel*3+1], Colors[pixel*3+2]);
  }
  for (int pixel = 0; pixel < NUM_PIXELS; pixel++){
    strip.setPixelColor(pixel, Colors[(pixel+ONBOARD_PIXELS)*3], Colors[(pixel+ONBOARD_PIXELS)*3+1], Colors[(pixel+ONBOARD_PIXELS)*3+2]);
  }
  strip.show();
  for (int cl = 0; cl < ARR_SIZE; cl++){
    Colors[cl] = 0;
  }
  req_update = false;
}

void Rest(){
  if (stage < REST_MAX){
    stage++;
    req_update = true;
  }
  for(int i = 0; i < TOTAL; i++){
    Colors[i*3] = stage;
  }
  Display();
  delay(250);
}

void Accel(){
  fired = true;
  stage = 0;
}

void Explosion(){
  for (int j = 0; j < PREFIRE_LOOPS; j++){
    PreFire();
  }
  for (int j = 0; j < EXPLOSION_LOOPS; j++){
    Fire();
  }
}

void PreFire(){
  //Pulse
  if (up){
    cycle++;
  } else {
    cycle = cycle - 1;
  }
  //Reverse the pulse
  if (cycle == 255){
    up = false;
  } else if (cycle == 0){
    up = true;
  }
  //Set colors
  for (int i = 0; i < TOTAL; i++){
    Colors[i*3+2] = 255;
    Colors[i*3] = cycle;
  }
  req_update = true;
  Display();
  delay(1);
}

void Fire(){
  for (int i = 0; i < TOTAL; i++){
    Colors[i*3] = 255;
    uint8_t choice = random(256);
    if (choice > 128){
      Colors[i*3+1] = 255;
    }
    if (choice > 230){
      Colors[i*3+2] = 255;
    }
  }
  req_update = true;
  Display();
  delay(1);
}

void PrintArray(){
  Serial.println("===============ARRAY=================");
  Serial.println("\t| RED\t| GRN\t| BLU\t"); 
  for (int z = 0; z < TOTAL; z++){
    Serial.print(z);
    Serial.print("\t| ");
    Serial.print(Colors[z*3]);
    Serial.print("\t| ");
    Serial.print(Colors[(z*3)+1]);
    Serial.print("\t| ");
    Serial.println(Colors[(z*3)+2]);
  }
}
