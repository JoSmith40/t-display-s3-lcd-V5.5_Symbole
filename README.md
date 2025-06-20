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


## Output
```
I (673) main_task: Calling app_main()
I (676) esp_idf_t_display_s3: Configuring LCD PWR GPIO...
I (676) LVGL: Starting LVGL task
I (681) esp_idf_t_display_s3: Configuring LCD RD GPIO...
I (689) esp_idf_t_display_s3: Configuring LCD Brightness...
I (695) esp_idf_t_display_s3: aw9364_init
I (698) esp_idf_t_display_s3: Configuring battery monitor...
I (704) esp_idf_t_display_s3: Initializing Intel 8080 bus...
I (709) esp_idf_t_display_s3: Initializing ST7789 LCD Driver...
I (835) esp_idf_t_display_s3: Adding display driver to lvgl port...
I (837) main_task: Returned from app_main()
````


## License
  - Copyright (c) 2023 Hiruna Wijesinghe (MIT License)
  - Copyright (c) 2025 JoSmith40 (Apache License Version 2.0)
