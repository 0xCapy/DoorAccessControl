/**
 * File: DoorAccess.cpp
 * Brife:Implementation of the DoorAccess class, responsible for handling all UI interactions,
 *       including password entry, user menu navigation, and communication with the UserManager
 *       for password storage and verification.
 * Author: Hubo Zhu
 * Date: 20:46 /25/03/2025
 */
#pragma once
#include "mbed.h"
#include "FlashIAP.h"
#include "SLCD.h"
#include "keypad.h"
#include "LCDmanager.h"

#define PASSWORD_LENGTH 8  
#define MAX_USERS 9  
#define FLASH_START_ADDRESS  0x0003FC00  
#define FLASH_SECTOR_SIZE    1024    
#include "UserManager.h"

// Add this member to the DoorAccess class:

class DoorAccess {
public:
    DoorAccess(PinName row0, PinName row1, PinName row2, PinName row3, 
               PinName col0, PinName col1, PinName col2, 
               PinName greenLedPin, PinName redLedPin); 

    void run(); 

private:
    void enterPassword(); 
    void verifyAdmin(); 
    void adminMenu(); 
    void editUserPassword(int userIndex);
    void deleteUser(int userIndex);
    void storePassword(int userIndex, const char* password); 
    bool verifyPassword(const char* password); 
    void deleteLastChar(); 
    void indicateSuccess();  
    void indicateFailure();  
    void resetEnteredPassword();
    void updateLCDDisplay();
    void PasswordManager(); 
    void passwordManager();
    void blink();
    Keypad keypad;
    SLCD lcd;  
    DigitalOut greenLED;  
    DigitalOut redLED;    
    FlashIAP flash;
    UserManager userManager;
    LCDManager lcdManager;
    char enteredPassword[PASSWORD_LENGTH + 1]; 
    int digitCount; 
    bool isAdminMode; 
};
