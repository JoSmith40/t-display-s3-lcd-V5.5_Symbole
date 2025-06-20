# t-display-s3-lcd-V5.5_Symbole

# Überblick
Dieses Projekt erzeugt eine Ansicht der integrierten Symbole der LVGL V9.2. LVGL ist Tool für sehr einfache grafische Darstellung auf verschiedenen Displays.

Beispiel Ansicht:
<div align="center">
<img src="doc/LVGL_Symbole.jpg" alt="Darstellung Symbole" width="400"/>
</div>


Hinweiss: In diesem Projekt wurde Elemente aus dem Projekt von Hiruna https://github.com/hiruna/esp-idf-t-display-s3.git verwendet.
Vielen Dank für die sehr gute Arbeit, Hiruna!


## Getestete Hardware & Software Framework(s)
* LilyGO T-Display S3 [v1.2] (ohne Touch) (https://lilygo.cc/products/t-display-s3)
  * ESP-IDF Version 5.5.x ([master branch](https://github.com/espressif/esp-idf))
  * LVGL 9.2.x


## Fähigkeiten

Dieses Projekt demonstriert die Zusammenhänge:

* Konfigurieren des ST7789-Displays mit dem I80-Bus (8Bit-Parallel)
* Konfiguration von LVGL/LVGL-Port und Darstellung von Symbolen
* Steuerung der Bildschirmhelligkeit mit [hiruna/esp-idf-aw9364](https://github.com/hiruna/esp-idf-aw9364.git)
  16-stufige Helligkeitsregelung
