#define USE_SDFAT //bibliothèque carte SD
#include "AudioTools.h" //Bibliothèque carte son
#include "AudioLibs/AudioKit.h" //Bibliothèque gestion boutons carte
#include "AudioCodecs/CodecMP3Helix.h"

//configuration de la carte audiokit
const char *startFilePath = "/";
const char* ext = "mp3";

SdSpiConfig sdcfg(PIN_AUDIO_KIT_SD_CARD_CS, DEDICATED_SPI, SD_SCK_MHZ(10) , &AUDIOKIT_SD_SPI); //configuration de la carte SD
AudioSourceSdFat source(startFilePath, ext, sdcfg);
AudioKitStream kit; //objet kit (gestion des boutons)
MP3DecoderHelix decoder;  // objet decodeur mp3
AudioPlayer player(source, kit, decoder); //Création de l'objet player
// Tableau de fichiers
const char* voix_off[3] = {"Q1.mp3", "Q2.mp3", "Q3.mp3"};
const char* R_1[3] = { "Q1_R1.mp3", "Q1_R2.mp3", "Q1_R3.mp3"};
const char* R_2[3] = { "Q2_R1.mp3", "Q2_R2.mp3", "Q3_R3.mp3"};
const char* R_3[3] = { "Q3_R1.mp3", "Q3_R2.mp3", "Q3_R3.mp3"};

int position_question = 0;
int num_reponse = 0;
int num_track = 0;
#include <Adafruit_NeoPixel.h> // Bibliothèque Neopixel
#include "AiEsp32RotaryEncoder.h"

#define LED_PIN        22 // Broche pour les Leds
#define LED_NUM        3 // nombre de leds

// Couleur des leds
#define R_COLOR  250 //valeur du rouge
#define G_COLOR  20 //valeur du vert
#define B_COLOR  250 //valeur du bleu

// Définition des connections de l'encodeur
#define ROTARY_ENCODER_A_PIN 23
#define ROTARY_ENCODER_B_PIN 18
#define ROTARY_ENCODER_BUTTON_PIN 5
#define ROTARY_ENCODER_VCC_PIN -1
#define ROTARY_ENCODER_STEPS 30

AiEsp32RotaryEncoder rotaryEncoder = AiEsp32RotaryEncoder(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN, ROTARY_ENCODER_BUTTON_PIN, ROTARY_ENCODER_VCC_PIN, ROTARY_ENCODER_STEPS);

Adafruit_NeoPixel pixels(LED_NUM, LED_PIN, NEO_GRB + NEO_KHZ800); // Création de l'objet pixels

void init_led() {
  for (int i = 0; i < LED_NUM; i++) {
    pixels.setPixelColor(i, pixels.Color(R_COLOR, G_COLOR, B_COLOR));
    pixels.show();

    delay(1000);
  }
  delay(200);
  for (int i = 0; i < LED_NUM; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    pixels.show();
  }
}
void rotary_onButtonClick()
{
  static unsigned long lastTimePressed = 0;
  //ignore multiple press in that time milliseconds
  if (millis() - lastTimePressed < 500)
  {
    return;
  }

  position_question++;

  Serial.print("position question");
  Serial.println(position_question);

  if (position_question > 2) {
    position_question = 0;
    Serial.print("Lecture histoire : ");
    Serial.println(num_track);
    String histoire = String(num_track)+".mp3";
    Serial.println(histoire);
    const char* histoire_char = histoire.c_str();
    source.setFileFilter(histoire_char);
    player.begin();
    while (player.isActive()) {
      player.copy();
    }
    num_track = 0;
  }

  switch (position_question) {
    case 0:
      num_track = num_track + num_reponse;
      break;
    case 1:
      num_track = num_track + num_reponse * 10;
      break;
    case 2:
      num_track = num_track + num_reponse * 100;
      break;
  }
  source.setFileFilter(voix_off[position_question]);
  player.begin();
  while (player.isActive()) {
    player.copy();
  }



}
void select_file(int num_R, int num_Q) {
  switch (num_Q) {

    case 0:
      source.setFileFilter(R_1[num_R]);
      player.begin();
      while (player.isActive()) {
        player.copy();
      }
      break;
    case 1:
      source.setFileFilter(R_2[num_R]);
      player.begin();
      while (player.isActive()) {
        player.copy();
      }
      break;
    case 2:
      source.setFileFilter(R_3[num_R]);
      player.begin();
      while (player.isActive()) {
        player.copy();
      }
      break;
  }

}
void rotary_loop()
{
  //dont print anything unless value changed

  if (rotaryEncoder.encoderChanged())
  {
    Serial.print("Value: ");
    int enc_value = rotaryEncoder.readEncoder();
    Serial.println(enc_value);
    for (int i = 0; i < LED_NUM; i++) {
      if (i == enc_value) {
        pixels.setPixelColor(i, pixels.Color(R_COLOR, G_COLOR, B_COLOR));



      } else {
        pixels.setPixelColor(i, pixels.Color(0, 0, 0));
      }

      pixels.show();

    }
    num_reponse = enc_value;
    select_file(enc_value, position_question);

  }
  if (rotaryEncoder.isEncoderButtonClicked())
  {
    rotary_onButtonClick();
  }
}
void IRAM_ATTR readEncoderISR()
{
  rotaryEncoder.readEncoder_ISR();
}
void setup() {
  Serial.begin(115200); // On démarre la liaison série USB (pour le débug)

  pixels.begin(); // demarrage de l'objet pixels

  rotaryEncoder.begin(); //démarrage de l'objet rotaryEncoder
  rotaryEncoder.setup(readEncoderISR);
  bool circleValues = false;
  rotaryEncoder.setBoundaries(0, 2, circleValues); //minValue, maxValue, circleValues true|false (when max go to min and vice versa)
  rotaryEncoder.setAcceleration(250);

  //configuration de la carte et demarrage
  // AudioLogger::instance().begin(Serial, AudioLogger::Info);

  // setup output
  auto cfg = kit.defaultConfig(TX_MODE);
  kit.begin(cfg);
  player.setVolume(0.7);


  // init_led(); //On allume les leds au démarrage pour vérifier si elles fonctionnent
  scenario();


}
void scenario() {
  source.setFileFilter("Intro.mp3");
  player.begin();
  while (player.isActive()) {
    player.copy();
  }
  source.setFileFilter("Q1.mp3");
  player.begin();
  while (player.isActive()) {
    player.copy();
  }

}
void loop() {

  rotary_loop();



}
