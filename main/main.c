// SPDX-FileCopyrightText: © 2025 Hiruna Wijesinghe <hiruna.kawinda@gmail.com>
// SPDX-License-Identifier: MIT

#include <stdio.h>
#include <esp_log.h>
#include <esp_timer.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "t_display_s3.h"
#include "lvgl.h"

static int symbol_draw_index = 0;

#define TAG "main"

lv_obj_t *top_bar;

TaskHandle_t lcd_brightness_task_hdl;
esp_timer_handle_t lcd_brightness_timer_hdl;

extern lv_obj_t *top_bar;

extern const char *symbols[] = {
    LV_SYMBOL_BULLET,
    LV_SYMBOL_AUDIO,
    LV_SYMBOL_VIDEO,
    LV_SYMBOL_LIST,
    LV_SYMBOL_OK,
    LV_SYMBOL_CLOSE,
    LV_SYMBOL_POWER,
    LV_SYMBOL_SETTINGS,
    LV_SYMBOL_HOME,
    LV_SYMBOL_DOWNLOAD,
    LV_SYMBOL_DRIVE,
    LV_SYMBOL_REFRESH,
    LV_SYMBOL_MUTE,
    LV_SYMBOL_VOLUME_MID,
    LV_SYMBOL_VOLUME_MAX,
    LV_SYMBOL_IMAGE,
    LV_SYMBOL_TINT,
    LV_SYMBOL_PREV,
    LV_SYMBOL_PLAY,
    LV_SYMBOL_PAUSE,
    LV_SYMBOL_STOP,
    LV_SYMBOL_NEXT,
    LV_SYMBOL_EJECT,
    LV_SYMBOL_LEFT,
    LV_SYMBOL_RIGHT,
    LV_SYMBOL_PLUS,
    LV_SYMBOL_MINUS,
    LV_SYMBOL_EYE_OPEN,
    LV_SYMBOL_EYE_CLOSE,
    LV_SYMBOL_WARNING,
    LV_SYMBOL_SHUFFLE,
    LV_SYMBOL_UP,
    LV_SYMBOL_DOWN,
    LV_SYMBOL_LOOP,
    LV_SYMBOL_DIRECTORY,
    LV_SYMBOL_UPLOAD,
    LV_SYMBOL_CALL,
    LV_SYMBOL_CUT,
    LV_SYMBOL_COPY,
    LV_SYMBOL_SAVE,
    LV_SYMBOL_BARS,
    LV_SYMBOL_ENVELOPE,
    LV_SYMBOL_CHARGE,
    LV_SYMBOL_PASTE,
    LV_SYMBOL_BELL,
    LV_SYMBOL_KEYBOARD,
    LV_SYMBOL_GPS,
    LV_SYMBOL_FILE,
    LV_SYMBOL_WIFI,
    LV_SYMBOL_BATTERY_FULL,
    LV_SYMBOL_BATTERY_3,
    LV_SYMBOL_BATTERY_2,
    LV_SYMBOL_BATTERY_1,
    LV_SYMBOL_BATTERY_EMPTY,
    LV_SYMBOL_USB,
    LV_SYMBOL_BLUETOOTH,
    LV_SYMBOL_TRASH,
    LV_SYMBOL_EDIT,
    LV_SYMBOL_BACKSPACE,
    LV_SYMBOL_SD_CARD,
    LV_SYMBOL_NEW_LINE,
    LV_SYMBOL_DUMMY,
};

void ui_init()
{
    top_bar = lv_obj_create(lv_screen_active());
    lv_obj_align(top_bar, LV_ALIGN_BOTTOM_RIGHT, 0, 0);
    // lv_obj_set_width(top_bar, LCD_H_RES);
    // lv_obj_set_height(top_bar, 130);
    lv_obj_set_width(top_bar, LCD_H_RES);  //* set width to full screen width
    lv_obj_set_height(top_bar, LCD_V_RES); // set height to full screen height
    lv_obj_remove_flag(top_bar, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_radius(top_bar, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(top_bar, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(top_bar, 255, LV_PART_MAIN | LV_STATE_DEFAULT); // Full opacity
    lv_obj_set_style_bg_color(top_bar, lv_color_hex(0xADD8E6), LV_PART_MAIN | LV_STATE_DEFAULT);
}


#define SYMBOLS_PER_ROW 15
#define SPACING_X 20
#define SPACING_Y 20

void create_symbol_grid()
{
    lv_obj_clean(top_bar); // Alte Inhalte löschen (optional)

    // Grid-Spalten definieren (15 Spalten mit fester Breite + spacing)
    static lv_coord_t col_dsc[SYMBOLS_PER_ROW + 1];
    for (int i = 0; i < SYMBOLS_PER_ROW; ++i)
        col_dsc[i] = LV_GRID_FR(1);  // gleiche Verteilung
    col_dsc[SYMBOLS_PER_ROW] = LV_GRID_TEMPLATE_LAST;

    // Zeilenanzahl berechnen (abhängig von Symbolanzahl)
    int total_symbols = sizeof(symbols) / sizeof(symbols[0]);
    int rows = (total_symbols + SYMBOLS_PER_ROW - 1) / SYMBOLS_PER_ROW;

    static lv_coord_t row_dsc[100]; // ausreichend viele Zeilen
    for (int i = 0; i < rows; ++i)
        row_dsc[i] = LV_GRID_CONTENT;
    row_dsc[rows] = LV_GRID_TEMPLATE_LAST;

    // Grid aktivieren
    lv_obj_set_layout(top_bar, LV_LAYOUT_GRID);
    lv_obj_set_style_pad_column(top_bar, SPACING_X, 0);
    lv_obj_set_style_pad_row(top_bar, SPACING_Y, 0);
    lv_obj_set_grid_dsc_array(top_bar, col_dsc, row_dsc);

    // Symbole als Labels hinzufügen
    for (int i = 0; i < total_symbols; ++i)
    {
        lv_obj_t *label = lv_label_create(top_bar);
        lv_label_set_text(label, symbols[i]);

        // Optional: Stil setzen
        lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_clear_flag(label, LV_OBJ_FLAG_SCROLLABLE);

        // Im Grid positionieren
        int col = i % SYMBOLS_PER_ROW;
        int row = i / SYMBOLS_PER_ROW;
        lv_obj_set_grid_cell(label, LV_GRID_ALIGN_CENTER, col, 1,
                                       LV_GRID_ALIGN_CENTER, row, 1);
    }
}

static void ui_update_task(void *pvParam)
{
    lvgl_port_lock(0);
    ui_init();
    lvgl_port_unlock();

    while (1)
    {
        if (lvgl_port_lock(0))
        {
            create_symbol_grid();
            lvgl_port_unlock();
        }
        vTaskDelay(pdMS_TO_TICKS(50)); // Kurze Pause, damit der Task nicht blockiert
    }
}
/*
// increment lvgl timer
static void lvgl_ticker_timer_cb(void *arg)
{
    lv_tick_inc(LVGL_TICK_PERIOD_MS);
}
*/


void app_main(void)
{
    // LVGL display handle
    static lv_display_t *disp_handle;
    lcd_init(&disp_handle, true);

    // configure a FreeRTOS task, pinned to the second core (core 0 should be used for hw such as wifi, bt etc)
    xTaskCreatePinnedToCore(ui_update_task, "update_ui", 4096 * 2, NULL, 0, NULL, 1);

    // demonstrate the lcd brightness fade using aw9364 driver
    lcd_set_brightness_pct_fade(100, 3000);

    // de-initialize lcd and other components
    // lvgl_port_remove_disp(disp_handle);
}
