# esp-idf-t-display-s3

# Overview
This is a template ESP-IDF project for LilyGO [T-Display S3](https://github.com/Xinyuan-LilyGO/T-Display-S3).

This code follows a similar structure to the code provided in [esp-bsp](https://github.com/espressif/esp-bsp) repo.

For the previous version of the code (LVGL 8.3.x & IDF 5.3.x), click [here](https://github.com/hiruna/esp-idf-t-display-s3/tree/idf_5.3.x_lvgl_8.3.x).

## Tested Hardware & Software Framework(s)
* LilyGO T-Display S3 [v1.2] (non-touch)
  * ESP-IDF Version 5.5.x ([master branch](https://github.com/espressif/esp-idf))
  * LVGL 9.2.x

### Note on LVGL 9
LVGL demos were conflicting with the lvgl port task, see comments in `ui_lvgl_demos_task` func for my workaround.

## Capabilities

This project demonstrates the following capabilities:

* Configure the ST7789 display using the I80 Bus
  * Screen orientation is landscape
* Screen brightness control using [hiruna/esp-idf-aw9364](https://github.com/hiruna/esp-idf-aw9364.git)
  * 16-step brightness control
    * NOTE: according to the LilyGO [T-Display S3 Datasheet](https://github.com/Xinyuan-LilyGO/T-Display-S3/blob/main/schematic/T_Display_S3.pdf), the board is equipped with a [AW9364DNR Dimming LED Driver](https://datasheet.lcsc.com/lcsc/1912111437_AWINIC-Shanghai-Awinic-Tech-AW9364DNR_C401007.pdf)
      capable of 16-step brightness control.

## sdkconfig

There are some sdkconfig options that needs to be set, I've included these in a [sdkconfig.defaults](./sdkconfig.defaults) file.
  * I haven't done anything fancy with the partition table, I'm simply using one of the default `PARTITION_TABLE_SINGLE_APP_LARGE`.


## Notes on LVGL and Memory Management

LVGL and display driver parameters have been set to utilize the SPI RAM. Performing a LVGL benchmark, I managed to get over 100 FPS.
According to the [ST7789 datasheet](https://www.rhydolabz.com/documents/33/ST7789.pdf), I think you should aim for 75 FPS.

## Notes on Display Framerate
Display framerate can be affected by many factors. During my testing I found that the `LVGL_BUFFER_SIZE` and `LCD_PIXEL_CLOCK_HZ` plays a key factor.

In my opinion, `LVGL_BUFFER_SIZE` should remain as is (1/10th of the display size in pixels) while the `LCD_PIXEL_CLOCK_HZ` can be tweaked.

The [ST7789 datasheet](https://www.rhydolabz.com/documents/33/ST7789.pdf) mentions a maximum pixel clock of 17 MHz `(17 * 1000 * 1000)`
but you may/may not experience issues with a high clock speed due to PSRAM banwidth (source: [ESP-FAQ Handbook](https://docs.espressif.com/projects/esp-faq/en/latest/esp-faq-en-master.pdf) [end of page 79]).
