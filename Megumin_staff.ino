#include <Adafruit_CircuitPlayground.h>
// do NOT include the standard NeoPixel library

#define NEOPIX_PIN    A2
#define NUM_PIXELS    5

Adafruit_CPlay_NeoPixel strip = Adafruit_CPlay_NeoPixel(NUM_PIXELS, NEOPIX_PIN, NEO_GRB + NEO_KHZ800);

int cycle = 0;
bool fired;

void setup() {
  CircuitPlayground.begin(255);
  CircuitPlayground.setAccelRange(LIS3DH_RANGE_2_G);
  CircuitPlayground.setAccelTap(1, 125);
  attachInterrupt(digitalPinToInterrupt(CPLAY_LIS3DH_INTERRUPT), Accel, FALLING);
  strip.begin();
}

void loop() {
  if (fired){
    Explosion();
    fired = false;
  }
}

void Accel(){
  fired = true;
}

void Rest(){
  CircuitPlayground.clearPixels();
  for (int i = 0; i < 10; i++){
    CircuitPlayground.setPixelColor(i,64,0,0);
  }
}

void Explosion(){
  for(int j = 0; j < 70; j++){
    cycle++;
    cycle = cycle%10;
    PreFire(cycle);
  }
  for (int j = 0; j < 300; j++){
    Fire();
  }
  Reset();
}

void PreFire(int pos){
  CircuitPlayground.clearPixels();
  for (int i = 0; i < 10; i++){
    CircuitPlayground.setPixelColor(cycle%10,0,0,0);
    CircuitPlayground.setPixelColor((cycle+1)%10,0,0,32);
    CircuitPlayground.setPixelColor((cycle+2)%10,0,0,64);
    CircuitPlayground.setPixelColor((cycle+3)%10,0,0,128);
    CircuitPlayground.setPixelColor((cycle+4)%10,0,0,192);
    CircuitPlayground.setPixelColor((cycle+5)%10,0,0,255);
    CircuitPlayground.setPixelColor((cycle+6)%10,64,0,255);
    CircuitPlayground.setPixelColor((cycle+7)%10,128,0,255);
    CircuitPlayground.setPixelColor((cycle+8)%10,192,0,255);
    CircuitPlayground.setPixelColor((cycle+9)%10,255,0,255);
  }
}

void Fire(){
  for (int i = 0; i < 10; i++){
    int choice = random(10);
    switch(choice){
      case 0:
      case 1:
      case 7:
      case 8:
      case 9:
        CircuitPlayground.setPixelColor(i,255,0,0);
        break;
      case 2:
        CircuitPlayground.setPixelColor(i,255,255,255);
        break;
      case 3:
      case 4:
      case 5:
      case 6:
        CircuitPlayground.setPixelColor(i,255,255,0);
        break;
    }
    delay(1);
  }
}

void Reset(){
  for (int color = 0; color < 64; color++){
    CircuitPlayground.clearPixels();
    for (int i = 0; i < 10; i++){
      CircuitPlayground.setPixelColor(i,color,0,0);
    }
    delay(1000);
  }
}
