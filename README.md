# DoorAccessControl

A password-based door access control system built on the NXP KL46Z microcontroller.

## Overview

This project implements a simple but functional access control system using a 4x3 keypad, an SLCD screen, and red/green status LEDs. The door strike (electromagnetic lock) should be connected externally and controlled via an output pin (code provided, relay wiring required).

- Users can enter passwords to unlock the door.
- Admin mode allows adding, editing, and deleting user passwords.
- Passwords are stored in internal flash memory.
- Red/green LEDs indicate door lock status.

## Features

- Admin authentication for access to user management
- Password verification for both users and admin
- Edit or delete existing user passwords
- LED feedback for access success or failure
- Simple UI through LCD scrolling text
- Flash-based password persistence

## Hardware Requirements

- NXP KL46Z Development Board
- 4x3 Keypad
- SLCD display (onboard)
- Red and Green LEDs (onboard or external)
- External relay or circuit to control door strike (not included in code)

## LED Behavior

| 🔴 Red LED   | Door locked (default state) |
| 🟢 Green LED | Door unlocked               |

## Workflow

See the flowchart below:

![Workflow](<docs/FLOW.png>)

### Key Classes

| Class        | Responsibility                                             |
|--------------|------------------------------------------------------------|
| `Keypad`     | Handle keypad scanning and key retrieval                   |
| `LCDManager` | Control LCD display text and scrolling messages            |
| `UserManager`| Store, verify, edit and delete passwords from flash memory |
| `DoorAccess` | Coordinates full access control logic                      |

## Getting Started

1. Clone the repository
2. Open in [Mbed Studio](https://os.mbed.com/studio/) or compatible IDE
3. Connect hardware as per requirements
4. Compile and flash to KL46Z

## File Overview

- `main.cpp` — Entry point
- `DoorAccess.*` — Core logic of the access control system
- `Keypad.*` — Keypad input reading
- `LCDManager.*` — Text display and scrolling
- `UserManager.*` — Password management and flash operations

## Notes

- Default admin password is set in `UserManager.cpp`.
- Flash memory handling does not include wear leveling.
- You must implement the final relay circuit externally for door strike control.
---
