# t-display-s3-lcd-V5.5_Symbole

# Overview
This project creates a view of the integrated symbols of LVGL V9.2. LVGL is a tool for very simple graphical representation on various displays.

Example view:
<div align="center">
<img src="doc/LVGL_Symbole.jpg" alt="Darstellung Symbole" width="400"/>
</div>

Note: In this project elements from the project of Hiruna https://github.com/hiruna/esp-idf-t-display-s3.git were used.
Many thanks for the very good work, Hiruna!


## Tested Hardware & Software Framework(s)
* LilyGO T-Display S3 [v1.2] (without touch) (https://lilygo.cc/products/t-display-s3)
  * ESP-IDF version 5.5.x ([master branch](https://github.com/espressif/esp-idf))
  * LVGL 9.2.x


## Capabilities

This project demonstrates the relationships:

* Configuring the ST7789 display with the I80 bus (8bit parallel)
* Configuration of LVGL/LVGL port and display of symbols
* Controlling the screen brightness with [hiruna/esp-idf-aw9364](https://github.com/hiruna/esp-idf-aw9364.git)
 16-step brightness control
