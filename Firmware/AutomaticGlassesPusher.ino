#include <ESP32Servo.h>
#include <Adafruit_NeoPixel.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>


//wifi configuration
const char* ssid = "Pixel_8200";
const char* password = "canapeche";

const char* slack_webhook_url = "https://hooks.slack.com/services/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";

#define PIN_LED      D2  
#define PIN_BUTTON   D6
#define NUM_LEDS     15

Adafruit_NeoPixel strip(NUM_LEDS, PIN_LED, NEO_GRB + NEO_KHZ800);

long premiereTeinte = 0;

const int PIN_SERVO = D10;
const int PIN_SERVO2 = D9;
const int PIN_TRIG = D5;
const int PIN_ECHO = D4;
#define PIN_BUTTON   D6

const int NB_LECTURES = 5;         
unsigned long precedentMillis = 0; 
const long INTERVALLE = 250;  

Servo monServo;
Servo monServo2;

float distanceStable;
int compteurDist = 0;


void sendSlackMessage(String message) {
    if (WiFi.status() == WL_CONNECTED) {
        WiFiClientSecure client;
        
        client.setInsecure(); 

        HTTPClient http;
        if (http.begin(client, slack_webhook_url)) {
            
            http.addHeader("Content-Type", "application/json");

            StaticJsonDocument<200> doc;
            doc["text"] = message;
            
            String requestBody;
            serializeJson(doc, requestBody);

            int httpResponseCode = http.POST(requestBody);

            if (httpResponseCode > 0) {
                String response = http.getString();
                Serial.print("[Slack] Response HTTP code: ");
                Serial.println(httpResponseCode);
                Serial.print("[Slack] Response from server : ");
                Serial.println(response); 
            } else {
                Serial.print("[Slack] Error when sending POST : ");
                Serial.println(http.errorToString(httpResponseCode).c_str());
            }

            http.end();
        } else {
            Serial.println("[Slack] Can't connect to the webhook.");
        }
    } else {
        Serial.println("[Wi-Fi] Not connected.");
    }
}

void RedBlink(){
  // Red rising
  for (int luminosite = 10; luminosite <= 200; luminosite += 4) {
    applyRed(luminosite);
    delay(10); 
  }

  // Red dropping
  for (int luminosite = 200; luminosite >= 10; luminosite -= 4) {
    applyRed(luminosite);
    delay(10); 
  }
  
  delay(100); 
}

void applyRed(int intensite) {
  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, strip.Color(intensite, 0, 0)); 
  }
  strip.show();
}

void rainbowsiuw(int pas){
  strip.rainbow(premiereTeinte, 1);
  strip.show();
  premiereTeinte += pas;
  if (premiereTeinte >= 65536) {
    premiereTeinte = 0;
  }

}

void setup() {
  pinMode(PIN_BUTTON, INPUT_PULLUP);
  Serial.begin(115200);
  strip.begin();           
  strip.clear();
  strip.show();            
  strip.setBrightness(128); 

  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  digitalWrite(PIN_TRIG, LOW);

  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);

  monServo.setPeriodHertz(50);
  monServo2.setPeriodHertz(50);

  monServo.attach(PIN_SERVO, 500, 2400);
  monServo2.attach(PIN_SERVO2, 500, 2400);

  monServo.write(180);
  monServo2.write(0); 

  delay(500); 
  WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        for (int i = 0; i < NUM_LEDS; i++) {
            // strip.Color(Rouge, Vert, Bleu)
            // On n'allume que le canal Rouge avec l'intensité variable
            if(i==0){
                strip.setPixelColor(NUM_LEDS-1, strip.Color(0, 0, 0)); 
            }else{
                strip.setPixelColor(i-1, strip.Color(0, 0, 0)); 
            }
            strip.setPixelColor(i, strip.Color(0, 0, 255)); 
            strip.show();
            delay(100);
        }
    }
    for(int i = 0; i<5; i++){
        for (int i = 0; i < NUM_LEDS; i++) {
        strip.setPixelColor(i, strip.Color(0, 255, 0)); 
    
        }
        strip.show();
        delay(400);
        strip.clear();
        strip.show();
        delay(400);

    } 
    
}

void loop() {
  if(!digitalRead(PIN_BUTTON)){ // Easter egg!
    for(int i=0; i<10;i++){
      monServo.write(180);
      monServo2.write(50); 
      delay(250);
      monServo.write(130);
      monServo2.write(0); 
      delay(250);
    }
    monServo.write(180);
    monServo2.write(0); 
  }
  float echantillons[NB_LECTURES];
  int nbValides = 0;

  for (int i = 0; i < NB_LECTURES; i++) {
    digitalWrite(PIN_TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(PIN_TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(PIN_TRIG, LOW);

    long duree = pulseIn(PIN_ECHO, HIGH, 20000); 

    if (duree > 0) {
      echantillons[nbValides] = (duree * 0.343) / 2.0;
      nbValides++;
    }
    
    delay(10); 
  }

  if (nbValides > 0) {
    for (int i = 0; i < nbValides - 1; i++) {
      for (int j = 0; j < nbValides - i - 1; j++) {
        if (echantillons[j] > echantillons[j + 1]) {
          float temp = echantillons[j];
          echantillons[j] = echantillons[j + 1];
          echantillons[j + 1] = temp;
        }
      }
    }

    distanceStable = echantillons[nbValides / 2];
  }else{
    distanceStable = 999;
  }
  if(distanceStable<40){
    compteurDist++;
  }else{
    compteurDist=0;
  }

  if (compteurDist>4) {
    for(int i =0; i<2;i++){
      RedBlink();
    }
    // --- 1. GO UP ---
    for (int step = 0; step <= 50; step++) {
      int angle1 = 180 - (step * 2); 
      int angle2 = (step * 2);  
      
      monServo.write(angle1);
      monServo2.write(angle2);
      delay(15); 
    }
    
    delay(1000); 
    
    // --- 2. GO BACK ---
    for (int step = 0; step <= 50; step++) {
      int angle1 = 100 + (step * 2); 
      int angle2 = 100 - (step * 2); 
      
      monServo.write(angle1);
      monServo2.write(angle2);
      delay(15);
    }
    
    delay(1000); 

    sendSlackMessage("The Automatic Glasses Pusher as succesfly pushed the glasses ! :ultrafastparrot:");

  }else{
    rainbowsiuw(300);
    delay(20);
  }
}