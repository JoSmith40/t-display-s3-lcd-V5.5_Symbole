# esp-idf-t-display-s3

# Überblick
Dies ist ein ESP-IDF-Vorlagenprojekt für LilyGO [T-Display S3](https://github.com/Xinyuan-LilyGO/T-Display-S3).

Dieser Code folgt einer ähnlichen Struktur wie der Code, der im [esp-bsp](https://github.com/espressif/esp-bsp) Repo bereitgestellt wird.

Für die vorherige Version des Codes (LVGL 8.3.x & IDF 5.3.x), klicken Sie [hier](https://github.com/hiruna/esp-idf-t-display-s3/tree/idf_5.3.x_lvgl_8.3.x).

## Getestete Hardware & Software Framework(s)
* LilyGO T-Display S3 [v1.2] (nicht berührungsempfindlich)
  * ESP-IDF Version 5.5.x ([master branch](https://github.com/espressif/esp-idf))
  * LVGL 9.2.x

### Anmerkung zu LVGL 9
LVGL Demos waren im Konflikt mit der lvgl port task, siehe Kommentare in `ui_lvgl_demos_task` func für meinen Workaround.


## Fähigkeiten

Dieses Projekt demonstriert die folgenden Fähigkeiten:

* Konfigurieren des ST7789-Displays mit dem I80-Bus
  * Bildschirmausrichtung im Querformat
* Steuerung der Bildschirmhelligkeit mit [hiruna/esp-idf-aw9364](https://github.com/hiruna/esp-idf-aw9364.git)
  * 16-stufige Helligkeitsregelung
    * HINWEIS: Gemäß dem LilyGO [T-Display S3 Datenblatt](https://github.com/Xinyuan-LilyGO/T-Display-S3/blob/main/schematic/T_Display_S3.pdf) ist die Karte mit einem [AW9364DNR Dimming LED Driver](https://datasheet.lcsc.com/lcsc/1912111437_AWINIC-Shanghai-Awinic-Tech-AW9364DNR_C401007.pdf)
 ausgestattet, der eine 16-stufige Helligkeitsregelung ermöglicht.


## sdkconfig

Es gibt einige sdkconfig-Optionen, die gesetzt werden müssen, ich habe diese in eine [sdkconfig.defaults](./sdkconfig.defaults) Datei aufgenommen.
  * Ich habe nichts Ausgefallenes mit der Partitionstabelle gemacht, ich verwende einfach eine der Standardeinstellungen `PARTITION_TABLE_SINGLE_APP_LARGE`.
  * Sie können die Anzeige einfach durch Setzen der Optionen `CONFIG_LV_USE_DEMO_BENCHMARK` (erfordert auch `CONFIG_LV_USE_DEMO_WIDGETS`) oder `CONFIG_LV_USE_DEMO_STRESS` einem Benchmark/Stresstest unterziehen.
  * LVGL FPS/CPU Usage Overlay kann mit `CONFIG_LV_USE_PERF_MONITOR=n` deaktiviert werden.


## Hinweise zu LVGL und Speicherverwaltung

Die LVGL- und Anzeigetreiber-Parameter wurden so eingestellt, dass sie das SPI-RAM nutzen. Bei einem LVGL-Benchmark konnte ich über 100 FPS erzielen.
Laut dem [ST7789-Datenblatt] (https://www.rhydolabz.com/documents/33/ST7789.pdf) sollte man 75 FPS anstreben.


## Hinweise zur Bildwiederholrate
Die Bildwiederholrate kann von vielen Faktoren beeinflusst werden. Während meiner Tests habe ich herausgefunden, dass die `LVGL_BUFFER_SIZE` und `LCD_PIXEL_CLOCK_HZ` einen wichtigen Faktor darstellen.

Meiner Meinung nach sollte `LVGL_BUFFER_SIZE` so bleiben, wie es ist (1/10 der Displaygröße in Pixeln), während die `LCD_PIXEL_CLOCK_HZ` angepasst werden kann.

Im [ST7789-Datenblatt](https://www.rhydolabz.com/documents/33/ST7789.pdf) wird ein maximaler Pixeltakt von 17 MHz `(17 * 1000 * 1000)`
erwähnt, aber es kann sein, dass Sie aufgrund der PSRAM-Bandbreite Probleme mit einer hohen Taktrate haben (Quelle: [ESP-FAQ Handbook](https://docs.espressif.com/projects/esp-faq/en/latest/esp-faq-en-master.pdf) [Ende Seite 79]).
