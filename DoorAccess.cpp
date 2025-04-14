/**
 * File: DoorAccess.cpp
 * Brife:Implementation of the DoorAccess class, responsible for handling all UI interactions,
 *       including password entry, user menu navigation, and communication with the UserManager
 *       for password storage and verification.
 * Author: Hubo Zhu
 * Date: 20:46 /25/03/2025
 */
#include "DoorAccess.h"
#include <cstdio>
#include <cstring>
#include <ctime>
bool mainpage_flag = true;
DoorAccess::DoorAccess(PinName row0, PinName row1, PinName row2, PinName row3, PinName col0, PinName col1, PinName col2, PinName greenLedPin, PinName redLedPin)
    : keypad(row0, row1, row2, row3, col0, col1, col2),
      lcd(),
      lcdManager(lcd),
      greenLED(greenLedPin), redLED(redLedPin),
      digitCount(0), isAdminMode(false) {

    memset(enteredPassword, 0, sizeof(enteredPassword));

    greenLED = 0;
    redLED = 0;

    lcd.clear();
    lcd.Contrast(15);
    lcd.printf("0000");
    ThisThread::sleep_for(500ms);
    lcd.clear();
    greenLED = 1;
    redLED = 1;
    userManager.init();
}

void DoorAccess::run() {
    lcd.clear();
    lcd.Home();
    
    while (true) {
        enterPassword();
    }
}

void DoorAccess::enterPassword() {
    resetEnteredPassword();
    bool scrolling = true;

    const int holdThreshold = 2000;
    const int pollInterval = 50;
    const int releaseTimeout = 100;

    while (true) {
        // Scroll prompt until input
        if (scrolling) {
            lcdManager.scrollText("ENtr Pass", 500);
        }

        char key = keypad.ReadKey();
        if (key == NO_KEY) {
            ThisThread::sleep_for(10ms);
            continue;
        }

        // Stop scrolling once input begins
        scrolling = false;

        // Check for long press on '#'
        if (key == '#') {
            uint64_t start = Kernel::get_ms_count();
            uint64_t lastSeen = start;
            bool held = false;

            while (Kernel::get_ms_count() - start < holdThreshold) {
                if (keypad.GetCurrentKey() == '#') {
                    lastSeen = Kernel::get_ms_count();
                    held = true;
                } else if (Kernel::get_ms_count() - lastSeen > releaseTimeout) {
                    held = false;
                    break;
                }
                ThisThread::sleep_for(pollInterval);
            }

            if (held) {
                verifyAdmin();
                return;
            }
        }

        // First input handling
        if (key == '*') {
            deleteLastChar();
        } else if (key == '#') {
            // Early submit
            enteredPassword[digitCount] = '\0';

            if (digitCount >= 4 && digitCount <= 8 &&
                userManager.verifyUserPassword(enteredPassword)) {
                indicateSuccess();
            } else {
                indicateFailure();
            }

            resetEnteredPassword();
            return;
        } else if (key >= '0' && key <= '9') {
            if (digitCount < 8) {
                enteredPassword[digitCount++] = key;
                updateLCDDisplay();
            }
        }

        // Continue accepting input
        while (true) {
            char k = keypad.ReadKey();
            if (k == NO_KEY) {
                ThisThread::sleep_for(10ms);
                continue;
            }

            if (k == '*') {
                deleteLastChar();
            } else if (k == '#') {
                enteredPassword[digitCount] = '\0';

                if (digitCount >= 4 && digitCount <= 8 &&
                    userManager.verifyUserPassword(enteredPassword)) {
                    indicateSuccess();
                } else {
                    indicateFailure();
                }

                resetEnteredPassword();
                return;
            } else if (k >= '0' && k <= '9') {
                if (digitCount < 8) {
                    enteredPassword[digitCount++] = k;
                    updateLCDDisplay();
                }
            }
        }
    }
}


void DoorAccess::verifyAdmin() {
    resetEnteredPassword();
    bool scrolling = true;

    while (true) {
        if (scrolling) {
            lcdManager.scrollText("ADn_PASS", 500);
        }

        char key = keypad.ReadKey();
        if (key == NO_KEY) {
            ThisThread::sleep_for(10ms);
            continue;
        }

        scrolling = false;

        if (key == '*') {
            deleteLastChar();
        } else if (key == '#') {
            enteredPassword[digitCount] = '\0';

            if (userManager.verifyAdminPassword(enteredPassword)) {
                indicateSuccess();
                adminMenu();
            } else {
                indicateFailure();
            }

            resetEnteredPassword();
            return;

        } else if (key >= '0' && key <= '9') {
            if (digitCount < 8) {
                enteredPassword[digitCount++] = key;
                updateLCDDisplay();  // will stop scrollText
            }
        }
    }
}




void DoorAccess::adminMenu() {
    resetEnteredPassword();

    bool scrolling = true;
    const char* message = "SELE USE";
    lcdManager.scrollText(message, 500);

    while (true) {
        if (scrolling) {
            lcdManager.scrollText(message, 500);  // keep updating scroll
        }

        char key = keypad.ReadKey();
        if (key == NO_KEY) {
            ThisThread::sleep_for(10ms);
            continue;
        }

        scrolling = false;  // stop scrolling on key press

        if (key >= '0' && key <= '9') {
            int userIndex = key - '0';

            char userLabel[16] = "";
            snprintf(userLabel, sizeof(userLabel), userIndex == 0 ? "Admn EDI" : "USEr%del ", userIndex);
            lcdManager.scrollText(userLabel, 700);

            resetEnteredPassword();
            const char* actionPrompt = "1EdI2DELE";
            scrolling = true;  // re-enable scrolling for next screen
            lcdManager.scrollText(actionPrompt, 600);

            while (true) {
                if (scrolling) {
                    lcdManager.scrollText(actionPrompt, 700);
                }

                char actionKey = keypad.ReadKey();
                if (actionKey == NO_KEY) {
                    ThisThread::sleep_for(10ms);
                    continue;
                }

                scrolling = false;

                if (actionKey == '1') {
                    editUserPassword(userIndex);
                    return;
                } else if (actionKey == '2') {
                    userManager.deleteUser(userIndex);
                    char delMsg[16];
                    snprintf(delMsg, sizeof(delMsg), "U%d DELE", userIndex);
                    uint64_t startTime = Kernel::get_ms_count();
                    while (Kernel::get_ms_count() - startTime < 1000) {
                    lcdManager.scrollText(delMsg, 1000);
                    ThisThread::sleep_for(50ms);  // to prevent hogging CPU
                    int temp = 0;
                    blink();
                }
                    return;
                } else if (actionKey == '*' || actionKey == '#') {
                    lcdManager.scrollText("Exit", 700);
                    ThisThread::sleep_for(1000ms);
                    return;
                }
            }

        } else if (key == '*' || key == '#') {
            lcd.clear();
            lcd.Home();
            printf("bye");
            ThisThread::sleep_for(1000ms);
            return;
        }
    }
}


void DoorAccess::editUserPassword(int userIndex) {
    resetEnteredPassword();
    bool scrolling = true;

    while (true) {
        // Show scrolling message until typing begins
        if (scrolling) {
            lcdManager.scrollText("NE Pass", 700);
        }

        char key = keypad.ReadKey();
        if (key == NO_KEY) {
            ThisThread::sleep_for(10ms);
            continue;
        }
        // Stop scrolling once user types a key
        scrolling = false;

        if (key == '#') {
            enteredPassword[digitCount] = '\0';
            if (digitCount >= 4 && digitCount <= 8) {
                userManager.storeUserPassword(userIndex, enteredPassword);
                blink();
                return;
            } else {
                indicateFailure();
                lcdManager.scrollMessageBlocking("Too Short", 500);
                return;
            }
        } else if (key == '*') {
            deleteLastChar();
        } else if (key >= '0' && key <= '9') {
            if (digitCount < 8) {
                enteredPassword[digitCount++] = key;
                updateLCDDisplay();  // shows entered digits
            }
        }
    }
}

void DoorAccess::deleteLastChar() {
    if (digitCount > 0) {
        digitCount--;
        enteredPassword[digitCount] = '\0';
        updateLCDDisplay();
    }
}

void DoorAccess::resetEnteredPassword() {
    memset(enteredPassword, 0, sizeof(enteredPassword));
    digitCount = 0;
    lcd.clear();
}

void DoorAccess::updateLCDDisplay() {
    lcd.clear();
    lcd.Home();
    int startIndex = (digitCount > 4) ? digitCount - 4 : 0;
    for (int i = startIndex; i < digitCount; i++) {
        lcd.putc(enteredPassword[i]);
    }
}

void DoorAccess::indicateSuccess() {
    greenLED = 0;
    redLED = 1;
    ThisThread::sleep_for(1s);
    greenLED = 1;
}

void DoorAccess::indicateFailure() {
    greenLED = 1;
    redLED = 0;
    ThisThread::sleep_for(1s);
    redLED = 1;
}

void DoorAccess::blink(){
    int temp = 1;
    while(temp <= 10){
    greenLED = 0;
    ThisThread::sleep_for(50ms);
    greenLED = 1;
    ThisThread::sleep_for(50ms);
    temp ++;
                    }
}

