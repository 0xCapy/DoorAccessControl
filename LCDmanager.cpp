/**
 * LCDManager.cpp
 * Encapsulates LCD display handling including text printing, scrolling, and buffered updates.
 *        Works with 4-character displays for smooth UI management.
 *
 * Author: Hubo Zhu
 * Date: 26/03/2025
 */

#include "LCDManager.h"
#include <cstring>
#include <cstdio>
#include "mbed.h"

LCDManager::LCDManager(SLCD& lcdRef) : lcd(lcdRef), scrollIndex(0), lastScrollTime(0) {
    memset(currentScrollText, 0, sizeof(currentScrollText));
}

void LCDManager::print(const char* text) {
    lcd.clear();
    lcd.Home();
    lcd.printf("%.4s", text);  // Cut off anything longer than 4
}

void LCDManager::scrollMessageBlocking(const char* text, int scrollDelay) {
    const int displayWidth = 4;
    int len = strlen(text);

    if (len <= displayWidth) {
        print(text);
        ThisThread::sleep_for(1000ms);
        return;
    }

    char buffer[5] = {0};
    for (int i = 0; i <= len - displayWidth; i++) {
        strncpy(buffer, &text[i], displayWidth);
        buffer[4] = '\0';
        print(buffer);
        ThisThread::sleep_for(scrollDelay);
    }

    ThisThread::sleep_for(1000ms);
}

void LCDManager::scrollText(const char* text, int scrollDelay) {
    const int displayWidth = 4;
    int len = strlen(text);

    if (strcmp(currentScrollText, text) != 0) {
        strncpy(currentScrollText, text, sizeof(currentScrollText));
        scrollIndex = 0;
        lastScrollTime = 0;
    }

    if (Kernel::get_ms_count() - lastScrollTime < scrollDelay) return;
    lastScrollTime = Kernel::get_ms_count();

    char buffer[5] = {0};
    if (len <= displayWidth) {
        strncpy(buffer, text, displayWidth);
    } else {
        strncpy(buffer, &text[scrollIndex], displayWidth);
    }

    print(buffer);

    scrollIndex++;
    if (scrollIndex > len - displayWidth) {
        scrollIndex = 0;
    }
}
