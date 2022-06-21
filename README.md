# radiorama
radiorama, un dispositif sonore de diffusion d'histoires

## Matériel utilisé

Pour ce projet, nous allons utiliser une carte à base d'ESP32 ( esp32 audio kit v2.2 2957 ) :

![AudioKit utilisé ](https://github.com/julienrat/radiorama/blob/main/Documents%20techniques/AudioKit/audio-toolkit.png)

## Branchements

Leds : GPIO22

Encodeur A : GPIO23

Encodeur B : GPIO18

Bouton : GPIO5

## Code arduino
Pour compiler avec succès le programme, il est nécessaire d'installer quelques bibliothèques dans votre environnement Arduino :

Bibliothèques Audio :

* https://github.com/pschatzmann/arduino-audio-tools.git
* https://github.com/pschatzmann/arduino-audiokit.git
* https://github.com/pschatzmann/arduino-libhelix.git

Bibliothèques Encodeur / Bouton :

* https://github.com/igorantolic/ai-esp32-rotary-encoder

Bibliothèque Leds RGB neoPixel WS2812b

* https://github.com/adafruit/Adafruit_NeoPixel
