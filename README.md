# 🎹 8051 Musical Keyboard

A musical keyboard built on the **8051 microcontroller**, capable of playing 13 chromatic notes (C4–C5) through a speaker, displaying the current note on a 7-segment LED display, and recording/playing back melodies — all driven by a 4×4 matrix keypad.

![Circuit Photo](8051%20final%20project.jpg)

---

## ✨ Features

- **13 Chromatic Notes** — Full chromatic scale from C4 to C5 (including sharps/flats)
- **Record & Playback** — Record a sequence of up to 100 notes with timing, then play it back
- **Pause / Resume** — Pause and resume playback at any time
- **Speed Control** — Toggle 2× playback speed during play mode
- **7-Segment Display** — Real-time note display on a 4-digit 7-segment LED
- **Sustained Notes** — In live mode, a note sustains until a different key is pressed

---

## 🔧 Hardware Components

| Component | Description |
|---|---|
| **8051 MCU** | Core microcontroller (Megawin-compatible) |
| **4×4 Matrix Keypad** | Input for notes and mode controls (connected to **P1**) |
| **7-Segment LED Display** | Displays current note/mode indicator (segments on **P0**, digit select on **P2**) |
| **Speaker / Buzzer** | Audio output via square wave on **P2.3** |

### Pin Mapping

| Port | Function |
|---|---|
| `P0` | 7-segment display segments (a–g + dp) |
| `P1` | 4×4 keypad (rows + columns) |
| `P2.0`–`P2.2` | 7-segment digit select |
| `P2.3` | Speaker output |

---

## 🎵 Keypad Layout & Note Mapping

The 4×4 keypad maps keys `0`–`12` to musical notes, with keys `13`–`15` reserved for controls:

| Key | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 |
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
| **Note** | C4 | C4♯ | D4 | D4♯ | E4 | F4 | F4♯ | G4 | G4♯ | A4 | A4♯ | B4 | C5 |

| Key | Function |
|---|---|
| `13` | Toggle 2× speed (in Play mode) / Silence (in Live mode) |
| `14` | Cycle operating mode (Live → Record → Play → reset) |
| `15` | Pause / Resume playback |

---

## 🔄 Operating Modes

The system cycles through three modes via key `14`:

### Mode 0 — Live Play 🎶
Press any note key to play it. The note **sustains continuously** until a different key is pressed. The 7-segment display shows the current note number.

### Mode 1 — Record 🔴
Notes are recorded with their **timing duration** into a buffer of up to 100 entries. The display shows `r` to indicate recording. Press keys to record your melody in real time.

### Mode 2 — Playback ▶️
The recorded melody plays back automatically, looping when it reaches the end. Use:
- **Key 15** to pause/resume
- **Key 13** to toggle 2× playback speed

Pressing **key 14** again resets everything back to Live mode and clears the recording buffer.

---

## 🏗️ Software Architecture

### Timer Interrupts

| Timer | Purpose | Frequency |
|---|---|---|
| **Timer 0** | Keypad scanning + duration tracking | ~100 Hz (every 10 ms) |
| **Timer 1** | Square wave generation for speaker | Variable (note-dependent frequency) |

### Source Files

| File | Description |
|---|---|
| `final_project.c` | Main program — initialization, mode logic, timers, display |
| `Keypad4x4.c` | 4×4 matrix keypad scanning driver |
| `Keypad4x4.h` | Header for the keypad driver |
| `myIO.lib` | Precompiled I/O library (linked at build time) |

### Key Data Structures

```c
typedef struct {
    unsigned char note_num;  // Note index (0–12, or 13 for silence)
    short duration;          // Duration in ~10 ms ticks
} Recorded_note;

__xdata Recorded_note record[100];  // Recording buffer in external RAM
```

---

## 🛠️ Build & Flash

### Prerequisites

- [**SDCC**](https://sdcc.sourceforge.net/) (Small Device C Compiler) — for compiling 8051 C code
- **sdar** / **sdranlib** — SDCC archiver tools (included with SDCC)
- **packihx** — Intel HEX packager (included with SDCC)

### Build

```bash
make
```

This will:
1. Compile `Keypad4x4.c` into a relocatable object (`.rel`)
2. Archive it into `myIO.lib` using `sdar`
3. Compile and link `final_project.c` with `myIO.lib`
4. Generate `final_project.hex` for flashing

### Flash to MCU

Use the included **Megawin ICP Programmer** (Windows):

```
MegawinIcpProgrammer/IcpProgrammer.exe
```

Load `final_project.hex` and flash it to the target 8051 chip via the ICP interface.

---

## 📁 Project Structure

```
8051_keyboard/
├── final_project.c          # Main application source
├── final_project.hex        # Compiled Intel HEX (ready to flash)
├── Keypad4x4.c              # 4×4 keypad scanner driver
├── Keypad4x4.h              # Keypad driver header
├── myIO.lib                 # Precompiled I/O library
├── Makefile                 # Build automation (SDCC)
├── 8051 final project.jpg   # Circuit/wiring photo
├── MegawinIcpProgrammer/    # Flashing tool (Windows)
│   ├── IcpProgrammer.exe
│   ├── HexEdit.dll
│   └── MegawinOCD.DLL
└── README.md                # This file
```



