// SPDX-FileCopyrightText: © 2025 Hiruna Wijesinghe <hiruna.kawinda@gmail.com>
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <esp_log.h>
#include <esp_timer.h>
#include <math.h>
#include <esp_task_wdt.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "t_display_s3.h"
#include "iot_button.h"
#include "button_gpio.h"

/*
#if defined CONFIG_LV_USE_DEMO_BENCHMARK
#include "lvgl__lvgl/demos/benchmark/lv_demo_benchmark.h"
#elif defined CONFIG_LV_USE_DEMO_STRESS
#include "lvgl__lvgl/demos/stress/lv_demo_stress.h"
#endif
*/


#define TAG "main"

// used for setup_test_ui() function
char *power_icon = LV_SYMBOL_POWER;
int battery_voltage;
int battery_percentage;
bool on_usb_power = false;
int current_battery_symbol_idx = 0;
char *battery_symbols[5] = {
        LV_SYMBOL_BATTERY_EMPTY,
        LV_SYMBOL_BATTERY_1,
        LV_SYMBOL_BATTERY_2,
        LV_SYMBOL_BATTERY_3,
        LV_SYMBOL_BATTERY_FULL
};

char *wlan_icon = LV_SYMBOL_WIFI;
char *bluetooth_icon = LV_SYMBOL_BLUETOOTH;
char *gps_icon = LV_SYMBOL_GPS;
char *audio_icon = LV_SYMBOL_AUDIO;
char *video_icon = LV_SYMBOL_VIDEO;
char *list_icon = LV_SYMBOL_LIST;


TaskHandle_t lcd_brightness_task_hdl;
esp_timer_handle_t lcd_brightness_timer_hdl;

// lvgl ui elements
lv_obj_t *side_bar;
lv_obj_t *top_bar;
lv_obj_t *bottom_bar;
lv_obj_t *lbl_power_mode;
lv_obj_t *lbl_battery_pct;
lv_obj_t *lbl_voltage;
lv_obj_t *lbl_power_icon;
lv_obj_t *screen_brightness;

lv_obj_t *lbl_wlan_icon;
lv_obj_t *lbl_bluetooth_icon;
lv_obj_t *lbl_gps_icon;
lv_obj_t *lbl_audio_icon;
lv_obj_t *lbl_video_icon;
lv_obj_t *lbl_list_icon;


void ui_init() {
    side_bar = lv_obj_create(lv_screen_active());
    lv_obj_set_width(side_bar, 50);
    lv_obj_set_height(side_bar, LCD_V_RES);
    lv_obj_remove_flag(side_bar, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_radius(side_bar, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(side_bar, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(side_bar, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(side_bar, lv_color_hex(0x88EE77), LV_PART_MAIN | LV_STATE_DEFAULT);

    top_bar = lv_obj_create(lv_screen_active());
    lv_obj_align(top_bar, LV_ALIGN_TOP_RIGHT, 0, 0);
    lv_obj_set_width(top_bar, LCD_H_RES - 50);
    lv_obj_set_height(top_bar, 40);
    lv_obj_remove_flag(top_bar, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_radius(top_bar, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(top_bar, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(top_bar, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(top_bar, lv_color_hex(0xFFDDB3), LV_PART_MAIN | LV_STATE_DEFAULT);

    lbl_power_mode = lv_label_create(top_bar);
    lv_obj_align(lbl_power_mode, LV_ALIGN_TOP_LEFT, 0, -10); // Horizontal, Vertikal alignment


    lbl_voltage = lv_label_create(top_bar);
    lv_obj_align(lbl_voltage, LV_ALIGN_TOP_RIGHT, 0, 5);

    lbl_power_icon = lv_label_create(top_bar);
    lv_obj_align(lbl_power_icon, LV_ALIGN_BOTTOM_RIGHT, 0, -10);


    lbl_battery_pct = lv_label_create(top_bar);
    lv_obj_align(lbl_battery_pct, LV_ALIGN_BOTTOM_LEFT, 0, 5);

    bottom_bar = lv_obj_create(lv_screen_active());
    lv_obj_align(bottom_bar, LV_ALIGN_BOTTOM_RIGHT, 0, 0);
    lv_obj_set_width(bottom_bar, LCD_H_RES - 50);                                                   // Set width
    lv_obj_set_height(bottom_bar, 130);
    lv_obj_remove_flag(bottom_bar, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_radius(bottom_bar, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(bottom_bar, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(bottom_bar, 255, LV_PART_MAIN | LV_STATE_DEFAULT);                      // Full opacity
    lv_obj_set_style_bg_color(bottom_bar, lv_color_hex(0xADD8E6), LV_PART_MAIN | LV_STATE_DEFAULT);



    lbl_wlan_icon = lv_label_create(bottom_bar);
    lv_obj_align(lbl_wlan_icon, LV_ALIGN_TOP_RIGHT, 0, 0);

    lbl_bluetooth_icon = lv_label_create(bottom_bar);
    lv_obj_align(lbl_bluetooth_icon, LV_ALIGN_TOP_RIGHT, -25, 0);

    lbl_gps_icon = lv_label_create(bottom_bar);
    lv_obj_align(lbl_gps_icon, LV_ALIGN_TOP_RIGHT, -50, 0);

    lbl_audio_icon = lv_label_create(bottom_bar);
    lv_obj_align(lbl_audio_icon, LV_ALIGN_TOP_RIGHT, -75, 0);

    lbl_video_icon = lv_label_create(bottom_bar);
    lv_obj_align(lbl_video_icon, LV_ALIGN_TOP_RIGHT, -100, 0);

    lbl_list_icon = lv_label_create(bottom_bar);
    lv_obj_align(lbl_list_icon, LV_ALIGN_BOTTOM_RIGHT, -125, 0);



}

static void update_hw_info_timer_cb(void *arg) {
    battery_voltage = get_battery_voltage();
    on_usb_power = usb_power_voltage(battery_voltage);
    battery_percentage = (int) volts_to_percentage((double) battery_voltage / 1000);
}

static void update_ui() {

    if (on_usb_power) {
        power_icon = LV_SYMBOL_USB;
        lv_label_set_text(lbl_power_mode, "USB Power");
        lv_label_set_text(lbl_battery_pct, "");
    } else {
        power_icon = battery_symbols[current_battery_symbol_idx];
        if (battery_percentage > 100) {
            battery_percentage = 100;
        }
        lv_label_set_text(lbl_power_mode, "Battery Power");
        lv_label_set_text_fmt(lbl_battery_pct, "Charge Level: %d %%", battery_percentage);
    }
    lv_label_set_text_fmt(lbl_voltage, "%d mV", battery_voltage);


    if (battery_percentage > 75 && battery_percentage <= 100) {
        current_battery_symbol_idx = 4;
    } else if (battery_percentage > 50 && battery_percentage <= 75) {
        current_battery_symbol_idx = 3;
    } else if (battery_percentage > 25 && battery_percentage <= 50) {
        current_battery_symbol_idx = 2;
    } else if (battery_percentage > 10 && battery_percentage <= 25) {
        current_battery_symbol_idx = 1;
    } else {
        current_battery_symbol_idx = 0;
    }

    lv_label_set_text(lbl_power_icon, power_icon);



    lv_label_set_text(lbl_wlan_icon, wlan_icon);
    lv_label_set_text(lbl_bluetooth_icon, bluetooth_icon);
    lv_label_set_text(lbl_gps_icon, gps_icon);
    lv_label_set_text(lbl_audio_icon, audio_icon);
    lv_label_set_text(lbl_video_icon, video_icon);
    lv_label_set_text(lbl_list_icon, list_icon);
}


static void ui_update_task(void *pvParam) {
    // setup the test ui
    lvgl_port_lock(0);
    ui_init();
    lvgl_port_unlock();

    while (1) {
        // update the ui every 50 milliseconds
        //vTaskDelay(pdMS_TO_TICKS(50));
        if (lvgl_port_lock(0)) {
            // update ui under lvgl semaphore lock
            update_ui();
            lvgl_port_unlock();
        }
    }

    // a freeRTOS task should never return ^^^
}

// increment lvgl timer
static void lvgl_ticker_timer_cb(void *arg)
{
    /* Tell LVGL how many milliseconds have elapsed */
    lv_tick_inc(LVGL_TICK_PERIOD_MS);
}

static void ui_lvgl_demos_task(void *pvParam) {
    // this is a workaround to get the lvgl demos working with esp-idf lvgl port
    // lvgl demos run in lv timers and so lvgl port locks are not called resulting in errors
    ESP_LOGI(TAG, "starting ui_lvgl_demos_task");

    // first acquire port lock
    lvgl_port_lock(0);

    // stop lvgl port (lvgl tick timer)
    if(lvgl_port_stop() == ESP_OK) {
        ESP_LOGI(TAG, "lvgl_port_stop ok");
    } else {
        ESP_LOGE(TAG, "lvgl_port_stop error!");
    }

    // since lv ticker is stopped by lvgl_port_stop, we need to create a timer for tick
    lv_timer_enable(true);
    const esp_timer_create_args_t lvgl_tick_timer_args = {
            .callback = &lvgl_ticker_timer_cb,
            .name = "LVGL tick",
    };
    esp_timer_handle_t tick_timer;
    esp_timer_create(&lvgl_tick_timer_args, &tick_timer);
    esp_timer_start_periodic(tick_timer, LVGL_TICK_PERIOD_MS * 1000);

/*
    // start the lvgl demos
#if defined CONFIG_LV_USE_DEMO_STRESS
    // if you specified CONFIG_LV_USE_DEMO_STRESS in sdkconfig, it will run lv_demo_stress
    lv_demo_stress();
#elif defined CONFIG_LV_USE_DEMO_BENCHMARK
    // if you specified CONFIG_LV_USE_DEMO_BENCHMARK in sdkconfig, it will run lv_demo_benchmark
    lv_demo_benchmark();
#endif

*/

    // infinite loop that handles the lv_timer_handler api calls
    // similar logic to lvgl port
    uint32_t task_delay_ms = 0;
    for(;;) {
        lvgl_port_lock(0);
        if (lv_display_get_default()) {
            task_delay_ms = lv_timer_handler();
        } else {
            task_delay_ms = 1;
        }
        if(task_delay_ms == LV_NO_TIMER_READY) {
            task_delay_ms = LVGL_MAX_SLEEP_MS;
        }
        vTaskDelay(pdMS_TO_TICKS(task_delay_ms));
        lvgl_port_unlock();
    }
}

void app_main(void) {
    // LVGL display handle
    static lv_display_t *disp_handle;

    // initialize the LCD
    // don't turn on backlight yet - demo of gradual brightness increase is shown below
    // otherwise you can set it to true to turn on the backlight at lcd init
    lcd_init(&disp_handle, false);

    /*
    #if defined CONFIG_LV_USE_DEMO_BENCHMARK || defined CONFIG_LV_USE_DEMO_STRESS
    lcd_set_brightness_step(100);
    // configure a FreeRTOS task, pinned to the second core (core 0 should be used for hw such as wifi, bt etc)
    TaskHandle_t lvgl_demo_task_hdl = NULL;
    xTaskCreatePinnedToCore(ui_lvgl_demos_task, "ui_lvgl_demos_task", 4096 * 2, NULL, 0, &lvgl_demo_task_hdl, 1);
    vTaskDelay(pdMS_TO_TICKS(5000));
    for(;;) {
        vTaskDelay(pdMS_TO_TICKS(500));
    }
#else
    */

    // otherwise it will show my example


    // Configure a periodic timer to update the battery voltage, brightness level etc
    const esp_timer_create_args_t periodic_timer_args = {
            .callback = &update_hw_info_timer_cb,
            .name = "update_hw_info_timer"
    };

    esp_timer_handle_t update_hw_info_timer_handle;
    ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &update_hw_info_timer_handle));
    // update the hw info 250 milliseconds
    ESP_ERROR_CHECK(esp_timer_start_periodic(update_hw_info_timer_handle, 250 * 1000));

    // configure a FreeRTOS task, pinned to the second core (core 0 should be used for hw such as wifi, bt etc)
    xTaskCreatePinnedToCore(ui_update_task, "update_ui", 4096 * 2, NULL, 0, NULL, 1);

    // demonstrate the lcd brightness fade using aw9364 driver
    lcd_set_brightness_pct_fade(100,3000);
//    vTaskDelay(pdMS_TO_TICKS(100));

    // de-initialize lcd and other components
    // lvgl_port_remove_disp(disp_handle);

//#endif
}
