/*
2 * Wavetable Synthesizer 
3 * 
4 * Copyright (c) Guy Bartell 2024
5 *
6 * This code is licensed under the Creative Commons Attribution-NonCommercial 4.0 International (CC BY-NC 4.0).
7 * 
8 * You are free to:
9 * - Share: Copy and redistribute the material in any medium or format.
10 * - Adapt: Remix, transform, and build upon the material.
11 * 
12 * Under the following terms:
13 * - Attribution: You must give appropriate credit, provide a link to the license, 
14 *   and indicate if changes were made. You may do so in any reasonable manner, 
15 *   but not in any way that suggests the licensor endorses you or your use.
16 * - NonCommercial: You may not use the material for commercial purposes.
17 * 
18 * No additional restrictions: You may not apply legal terms or technological 
19 * measures that legally restrict others from doing anything the license permits.
20 * 
21 * Full license text can be found at:
22 * https://creativecommons.org/licenses/by-nc/4.0/
23 *
24 * Third-party libraries and components included in this software are licensed 
25 * under their respective licenses, as listed in the THIRD_PARTY_LICENSES file.
26 *
27 * For commercial use or licensing inquiries, please contact guybartell.ext@gmail.com.
28 */

// DISPLAY (M4 core)

#include "Arduino_GigaDisplay_GFX.h"   // Display graphics
#include "Arduino_GigaDisplayTouch.h"  // Touch screen functionality
#include <RPC.h>

GigaDisplay_GFX display;
Arduino_GigaDisplayTouch touchDetector;

#define WHITE 0xffff
#define BLACK 0x0000
#define CYAN 0x2c70
#define GREY 0x7430
#define DISPLAY_HEIGHT 480
#define DISPLAY_WIDTH 800

unsigned long lastTouchHandler = 0; // Used for non-blocking delay
unsigned long lastWaveformDraw = 0;  // Variable non-blocking delay defined by step size (for drawWaveform)

int step_size = 1;
int threshold = 20;  // time in milliseconds for button threshold
int wavThreshold;    // time in milliseconds for button threshold
// bool on = 1;      // to keep track of on/off state for on/off switch

int downsample_factor = 512 / 64;  // Used for rendering the wavetable button waveforms

const float BIT_SCALER = 4095 / 255; // Used for scaling between 12Bit and 8Bit look-up tables(LUTs) for loading waveform and writing to LUT

// Constants for button dimensions
const int BUTTON_HEIGHT = 32;
const int BUTTON_WIDTH = 64;

// Coordinates for buttons and display
const int LEFT_BOUNDARY = 144;
const int WAVEFORM_DISPLAY_X = 112;
const int BOTTOM_BAR_X = 40;
const int TOP_BAR_X = 408;

const int RND_X = BOTTOM_BAR_X;
const int RND_Y = 499;
const int CLR_X = TOP_BAR_X;
const int CLR_Y = 38;

const int ON_X = TOP_BAR_X;
const int ON_Y = LEFT_BOUNDARY;
const int VOL_X = TOP_BAR_X;
const int VOL_Y = 254;
const int DRAW_MODE_X = BOTTOM_BAR_X;
const int DRAW_MODE_Y = 38;
const int LEFT_BAR_X = 335;
const int LEFT_BAR_Y = 38;
const int RND_MODE_X = BOTTOM_BAR_X;
const int RND_MODE_Y = 588;

// Co-ordinates for envelope display
const int ATTACK_X = 333;
const int ATTACK_Y = 254;
const int DECAY_X = ATTACK_X - 74;
const int DECAY_Y = ATTACK_Y;
const int SUSTAIN_X = DECAY_X - 74;
const int SUSTAIN_Y = ATTACK_Y;
const int RELEASE_X = SUSTAIN_X - 74;
const int RELEASE_Y = ATTACK_Y;
const int ATTACK_TEXT_X = ATTACK_X - 5;
const int ATTACK_TEXT_Y = 143;
const int DECAY_TEXT_X = ATTACK_TEXT_X - 74;
const int DECAY_TEXT_Y = ATTACK_TEXT_Y;
const int SUSTAIN_TEXT_X = DECAY_TEXT_X - 74;
const int SUSTAIN_TEXT_Y = ATTACK_TEXT_Y;
const int RELEASE_TEXT_X = SUSTAIN_TEXT_X - 74;
const int RELEASE_TEXT_Y = ATTACK_TEXT_Y;
const int VOL_TEXT_X = ATTACK_TEXT_X + 74;
const int VOL_TEXT_Y = ATTACK_TEXT_Y;

const int WAV_X = 334;
const int WAV_Y = 700;
const int ENV_X = 260;
const int ENV_Y = 700;
const int MIDI_X = 186;
const int MIDI_Y = 700;

const int VEL_X = WAVEFORM_DISPLAY_X;
const int VEL_Y = 700;

const int PANIC_X = 186;
const int PANIC_Y = SUSTAIN_Y;

const int LUT_DISPLAY_BIT_DEPTH = 256;

// ADSR ranges
const float ATTACK_RANGE = 1.0;
const float DECAY_RANGE = 1.0;
const float SUSTAIN_RANGE = 1.0;
const float RELEASE_RANGE = 1.0;

// For updating M7
float attack = 0.0025;   // seconds
float decay = 0.0025;    // seconds
float sustain = 1.0;     // sustain level (0.0 to 1.0)
float release = 0.0025;  // seconds

// For updating ADSR sliders
int attackVal = 0;
int decayVal = 0;
int sustainVal = 400;
int releaseVal = 0;

// For updating pitch bend and midi channel in sliders & M7 
int pbRangeSlider = 50;
int pbRange = 2;
int midiChannelSlider = 0;
int midiChannel = 0;

// Toggle button handlers, not currently implemented
// bool on = 1;
// bool onOld = 0;
// bool onState = 0;
// bool onStateOld = 0;

// For handling draw/erase button
bool draw = 1;
bool drawOld = 0;
bool drawState = 0;
bool drawStateOld = 0;
bool drawMode = 1;    // Controls whether drawing adds (1) or clears (0) waveform

// For handling curve/block random generation
bool rndMode = 0;
bool rndModeOld = 1;
bool rndModeState = 0;
bool rndModeStateOld = 0;

// For handling envelope button
bool env = 0;
bool envOld = 1;
bool envState = 0;
bool envStateOld = 0;

// For handling wav button
bool wav = 1;
bool wavOld = 0;
bool wavState = 0;
bool wavStateOld = 0;

// For handling velocity sensitivity button (not currently implemented)
bool velocityMode = 1;
bool velocityModeOld = 0;
bool velocityModeState = 0;
bool velocityModeStateOld = 0;

int velocitySend = 1;

// MIDI panic (all voices off) to update M7
bool panic = 0;
bool panicOld = 0;
bool panicState = 0;
bool panicStateOld = 0;

// For handling MIDI button
bool midi = 0;
bool midiOld = 1;
bool midiState = 0;
bool midiStateOld = 0;

// Blocks touchscreen while wav, env & midi screens are loading
int screenLoading = 0;

// For handling clear button
bool clr = 0;
bool clrOld = 0;

// For handling random button
bool rnd = 0;
bool rndOld = 0;

// array of random step sizes
const char* steps[8] = { "512", "256", "128", "64", "32", "16", "8", "4" };
const float steps_values[8] = { 1.0, 2.0, 4.0, 8.0, 16.0, 32.0, 64.0, 128.0 };

// array of MIDI channel values
const char* CH_steps[17] = { "ALL", "  1", "  2", "  3", "  4", "  5", "  6", "  7", "  8", "  9", " 10", " 11", " 12", " 13", " 14", " 15", " 16" };

// array of pitch bend values
const char* PB_steps[25] = { "  0", " +1", " +2", " +3", " +4", " +5", " +6", " +7", " +8", " +9", "+10", "+11", "+12", "+13", "+14", "+15", "+16", "+17", "+18", "+19", "+20", "+21", "+22", "+23", "+24" };

// array of LUT names
const char* wavs[4] = { "sine_wave", "square_wave", "sawtooth_wave", "organ_pipe_wave" };

// number of elements in wavs array, for generating wavetable buttons
static size_t wavs_size = sizeof(wavs) / sizeof(wavs[0]);

// number of elements in steps array, for step size selector
static size_t steps_size = sizeof(steps) / sizeof(steps[0]);

int selectedButton = 0;     // Tracks the currently selected wavetable button
int selectedButtonOld = 0;  // Tracks the previously selected wavetable button
int selectedStep = 0;       // Tracks the currently selected step button

// Size of LUT
static size_t lut_size = 512;

// Function to map floats
float fmap(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
// Function to map to common logarithm scale
float logmap(float x, float in_min, float in_max, float out_min, float out_max) {
  // Normalize input to 0.0 – 1.0
  float normalized = (x - in_min) / (in_max - in_min);
  normalized = max(0.0, min(1.0, normalized));  // Clamp between 0 and 1

  // Logarithmic transformation
  float logNormalized = log10(1 + 9 * normalized);

  // Map back to output range
  return (logNormalized * (out_max - out_min)) + out_min;
}


void gigaTouchHandler(uint8_t contacts, GDTpoint_t* points) {
  if (!screenLoading) { // Only when screens have finished loading
  // Reset all momentary buttons to 0 (no press)
    clr = 0;
    rnd = 0;

    wavThreshold = calculateThreshold(step_size);

    for (int i = 0; i < contacts; i++) {
      buttonHandler(points[i].x, points[i].y);
      volumeSlider(points[i].x, points[i].y);
      randomStepSize(points[i].x, points[i].y);
      // Only call drawWaveform if the global threshold has passed
      if (millis() - lastWaveformDraw > wavThreshold) {
        drawWaveform(points[i].x, points[i].y, step_size);
        lastWaveformDraw = millis();  // Update the last execution time
      }
      ADSRSlider(points[i].x, points[i].y);
      midiSlider(points[i].x, points[i].y);
    }
    lastTouchHandler = millis();
  }
}

// Non-blocking delay threshold dynamically updated from step-size
int calculateThreshold(int stepSize) {
  // Map step sizes to thresholds
  if (stepSize <= 128 && stepSize >= 64) {
    return 80;
  } else if (stepSize <= 32 && stepSize >= 16) {
    return 60;
  } else if (stepSize < 16 && stepSize >= 8) {
    return 40;
  } else if (stepSize == 4) {
    return 20;
  } else if (stepSize == 2) {
    return 5;
  } else if (stepSize == 1) {
    return 1;
  } else {
    return 1;  // Default threshold
  }
}

// Handles button presses
void buttonHandler(int x, int y) {
  // Normalise x & y to between 0.0 & 1.0
  float normalisedWav_X = float(x - WAV_X) / BUTTON_HEIGHT;
  float normalisedWav_Y = float(y - WAV_Y) / BUTTON_WIDTH;
  float normalisedEnv_X = float(x - ENV_X) / BUTTON_HEIGHT;
  float normalisedEnv_Y = float(y - ENV_Y) / BUTTON_WIDTH;
  float normalisedMidi_X = float(x - MIDI_X) / BUTTON_HEIGHT;
  float normalisedMidi_Y = float(y - MIDI_Y) / BUTTON_WIDTH;
  // float normalisedOn_X = float(x - ON_X) / BUTTON_HEIGHT;
  // float normalisedOn_Y = float(y - ON_Y) / BUTTON_WIDTH;
  // float normalisedVelocity_X = float(x - VEL_X) / (BUTTON_HEIGHT);
  // float normalisedVelocity_Y = float(y - VEL_Y) / BUTTON_WIDTH;

  // Mark wav button as pressed if within bounds
  if (normalisedWav_X >= 0.0 && normalisedWav_X <= 1.0 && normalisedWav_Y >= 0.0 && normalisedWav_Y <= 1.0) {
    wav = 1;
  }
  // Mark envelope button as pressed if within bounds
  if (normalisedEnv_X >= 0.0 && normalisedEnv_X <= 1.0 && normalisedEnv_Y >= 0.0 && normalisedEnv_Y <= 1.0) {
    env = 1;
  }

  // Mark MIDI button as pressed if within bounds
  if (normalisedMidi_X >= 0.0 && normalisedMidi_X <= 1.0 && normalisedMidi_Y >= 0.0 && normalisedMidi_Y <= 1.0) {
    midi = 1;
  }

  // Mark on button as pressed if within bounds
  // if (normalisedOn_X >= 0.0 && normalisedOn_X <= 1.0 && normalisedOn_Y >= 0.0 && normalisedOn_Y <= 1.0) {
  //   onState = 1;
  // }


  // Mark velocity mode button as pressed if within bounds
  // if (normalisedVelocity_X >= 0.0 && normalisedVelocity_X <= 1.0 && normalisedVelocity_Y >= 0.0 && normalisedVelocity_Y <= 1.0) {
  //   velocityModeState = 1;
  // }

  if (wav) { // Only when waveform screen is displayed

  // Normalise x & y to between 0.0 & 1.0
    float normalisedDraw_X = float(x) / (BUTTON_HEIGHT + 52);
    float normalisedDraw_Y = float(y - DRAW_MODE_Y) / BUTTON_WIDTH;
    float normalisedRandomMode_X = float(x) / (BUTTON_HEIGHT + 52);
    float normalisedRandomMode_Y = float(y - RND_MODE_Y) / BUTTON_WIDTH;

    float normalisedClr_X = float(x - CLR_X) / BUTTON_HEIGHT;
    float normalisedClr_Y = float(y - CLR_Y) / BUTTON_WIDTH;
    float normalisedRnd_X = float(x) / 72;
    float normalisedRnd_Y = float(y - RND_Y) / BUTTON_WIDTH;

    // Mark clear button as pressed if within bounds
    if (normalisedClr_X >= 0.0 && normalisedClr_X <= 1.0 && normalisedClr_Y >= 0.0 && normalisedClr_Y <= 1.0) {
      clr = 1;
    }

    // Mark random button as pressed if within bounds
    if (normalisedRnd_X >= 0.0 && normalisedRnd_X <= 1.0 && normalisedRnd_Y >= 0.0 && normalisedRnd_Y <= 1.0) {
      rnd = 1;
    }

    // Mark draw mode button as pressed if within bounds
    if (normalisedDraw_X >= 0.0 && normalisedDraw_X <= 1.0 && normalisedDraw_Y >= 0.0 && normalisedDraw_Y <= 1.0) {
      drawState = 1;
    }

    // Mark random mode button as pressed if within bounds
    if (normalisedRandomMode_X >= 0.0 && normalisedRandomMode_X <= 1.0 && normalisedRandomMode_Y >= 0.0 && normalisedRandomMode_Y <= 1.0) {
      rndModeState = 1;
    }

    // Select waveform select button as pressed if within bounds
    for (int i = 0; i < wavs_size; i++) {
      if (x >= 0 && x <= 74 && y >= (LEFT_BOUNDARY + (89 * i)) && y <= (LEFT_BOUNDARY + (89 * i) + BUTTON_WIDTH + 2)) {
        if (i != selectedButton) {
          // Set new LUT and load it into the temporary LUT
          selectedButton = i;
        }
      }
    }
  }

  if (midi){ // Only when MIDI screen is displayed
    float normalisedPanic_X = float(x - PANIC_X) / BUTTON_HEIGHT;
    float normalisedPanic_Y = float(y - PANIC_Y) / BUTTON_WIDTH;

    // Mark panic button as pressed if within bounds
    if (normalisedPanic_X >= 0.0 && normalisedPanic_X <= 1.0 && normalisedPanic_Y >= 0.0 && normalisedPanic_Y <= 1.0) {
      panic = 1;
    }
  }
}

// Select LUT from example LUTs in wavetable buttons
void waveformSelection() {
  if (selectedButton != selectedButtonOld && selectedButton != -1) {
    screenLoading = 1;
    RPC.call("loadLUT", selectedButton); // Load LUT from M7
    // Redraw all buttons
    generateWavetableButtons();
    // Update main waveform display
    updateMainWaveformDisplay();
  } else if (selectedButton != selectedButtonOld && selectedButton == -1) {
    generateWavetableButtons();
  }
  selectedButtonOld = selectedButton;
}


// Handles buttons for step size for random & drawing functions
void randomStepSize(int x, int y) {
  for (int i = 0; i < steps_size; i++) {
    if (x >= LEFT_BAR_X - (BUTTON_HEIGHT * i) && x <= (LEFT_BAR_X - (BUTTON_HEIGHT * i) + BUTTON_HEIGHT) && y >= LEFT_BAR_Y && y <= LEFT_BAR_Y + BUTTON_WIDTH && wav) {
      if (i != selectedStep) {
        selectedStep = i;
        step_size = steps_values[i]; // Selects step size name
        // Redraw all buttons
        generateRandomStepButtons();
      }
    }
  }
}

// Draws 'random' button
void drawRandom() {
  if (rnd != rndOld) {
    if (rnd == 1) {  // momentary pressed
      screenLoading = 1;
      selectedButton = -1;
      buttonOn(RND_X, RND_Y, 6, "RND");
      RPC.call("generateRandomArray", step_size); // Generate temp LUT in M7
      generateWavetableButtons();
      updateMainWaveformDisplay();
    } else {  // momentary released
      buttonOff(RND_X, RND_Y, 6, "RND");
    }
    // Update the old state of the momentary
    rndOld = rnd;
  }
}

// Draws 'clear' button
void drawClear() {
  if (clr != clrOld) {
    if (clr == 1) {  // momentary pressed
      screenLoading = 1;
      selectedButton = -1;
      buttonOn(CLR_X, CLR_Y, 6, "CLR"); // Clear temp LUT in M7
      RPC.call("clearArray");
      generateWavetableButtons();
      updateMainWaveformDisplay();
    } else {  // momentary released
      buttonOff(CLR_X, CLR_Y, 6, "CLR");
    }
    // Update the old state of the momentary
    clrOld = clr;
  }
}

// Draws on/off button, currently not implemented
// void drawOn() {
//   if (onState != onStateOld) {
//     if (onState == 1) {  // momentary pressed
//       on = !on;
//     }
//     // Update the old state of the momentary
//     onStateOld = onState;
//   }
//   if (on != onOld) {
//     if (on == 1) {
//       buttonOn(ON_X, ON_Y, 14, "ON");
//       // Starts DAC on 'ON' button press
//       // RPC.call("startDAC", 1);
//       // Serial.println("toggle on");
//     } else if (on == 0) {
//       buttonOff(ON_X, ON_Y, 6, "OFF");
//       // Starts DAC on 'OFF' button press
//       // RPC.call("startDAC", 0);
//       // Serial.println("toggle off");
//     }
//     onOld = on;
//   }
// }

// Draws 'panic' button
void drawPanic() {
  if (panicState != panicStateOld) {
    if (panicState == 1) {  // momentary pressed
      panic = !panic;
    }
    // Update the old state of the momentary
    panicStateOld = panicState;
  }
  if (panic != panicOld) {
    if (panic == 1) {
      buttonOn(PANIC_X, PANIC_Y, 6, " ! ");
      RPC.call("panic"); // Executes all voices off in M7
    } else if (panic == 0) {
      buttonOff(PANIC_X, PANIC_Y, 6, " ! ");
    }
    panicOld = panic;
  }
}

// Generates and updates the envelope display
void generateEnvelopeDisplay() {

  display.fillRect(0, LEFT_BOUNDARY - 1, LEFT_BOUNDARY + LUT_DISPLAY_BIT_DEPTH, lut_size + 2, WHITE);  // Clear previous display
  display.fillRect(0, 0, DISPLAY_HEIGHT, 142, WHITE);  // Clear left bar

  display.drawRect(ATTACK_X, ATTACK_Y, BUTTON_HEIGHT + 2, 402, CYAN);
  display.fillRect(ATTACK_X + 1, ATTACK_Y, BUTTON_HEIGHT, attackVal, CYAN);
  display.drawRect(DECAY_X, DECAY_Y, BUTTON_HEIGHT + 2, 402, CYAN);
  display.fillRect(DECAY_X + 1, DECAY_Y, BUTTON_HEIGHT, decayVal, CYAN);
  display.drawRect(SUSTAIN_X, SUSTAIN_Y, BUTTON_HEIGHT + 2, 402, CYAN);
  display.fillRect(SUSTAIN_X + 1, SUSTAIN_Y, BUTTON_HEIGHT, sustainVal, CYAN);
  display.drawRect(RELEASE_X, RELEASE_Y, BUTTON_HEIGHT + 2, 402, CYAN);
  display.fillRect(RELEASE_X + 1, RELEASE_Y, BUTTON_HEIGHT, releaseVal, CYAN);
  labelText(ATTACK_TEXT_X, ATTACK_TEXT_Y, "ATT");
  labelText(DECAY_TEXT_X, DECAY_TEXT_Y, "DEC");
  labelText(SUSTAIN_TEXT_X, SUSTAIN_TEXT_Y, "SUS");
  labelText(RELEASE_TEXT_X, RELEASE_TEXT_Y, "REL");
  screenLoading = 0; // Update screen loading state once everything has loaded
}

// Generates and updates the MIDI display
void generateMIDIDisplay() {

  display.fillRect(0, LEFT_BOUNDARY - 1, LEFT_BOUNDARY + LUT_DISPLAY_BIT_DEPTH, lut_size + 2, WHITE);  // Clear previous display
  display.fillRect(0, 0, DISPLAY_HEIGHT, 142, WHITE);  // Clear left bar
  display.drawRect(ATTACK_X, ATTACK_Y, BUTTON_HEIGHT + 2, 402, CYAN);
  display.fillRect(ATTACK_X + 1, ATTACK_Y, BUTTON_HEIGHT, pbRangeSlider, CYAN);
  display.drawRect(DECAY_X, DECAY_Y, BUTTON_HEIGHT + 2, 402, CYAN);
  display.fillRect(DECAY_X + 1, DECAY_Y, BUTTON_HEIGHT, midiChannelSlider, CYAN);
  valuesText(ATTACK_TEXT_X, ATTACK_TEXT_Y + 52, pbRange, PB_steps);
  valuesText(DECAY_TEXT_X, DECAY_TEXT_Y + 52, midiChannel, CH_steps);
  labelText(ATTACK_TEXT_X, ATTACK_TEXT_Y, "PB:");
  labelText(DECAY_TEXT_X, DECAY_TEXT_Y, "CH:");
  labelText(SUSTAIN_TEXT_X, SUSTAIN_TEXT_Y, "PANIC");
  buttonOff(PANIC_X, PANIC_Y, 6, " ! ");
  screenLoading = 0; // Update screen loading state once everything has loaded
}

// Draws wav screen select button
void drawWav() {
  if (wavState != wavStateOld) {
    if (wavState == 1) {  // momentary pressed
      wav = !wav;
    }
    // Update the old state of the momentary
    wavStateOld = wavState;
  }
  if (wav != wavOld) {
    if (wav) {
      screenLoading = 1;
      buttonOn(WAV_X, WAV_Y, 6, "WAV");
      env = 0;
      midi = 0;
      drawEnv();
      drawMidi();
      generateRandomStepButtons();
      generateWavetableButtons();
      redrawDrawMode();
      display.fillRect(RND_X+16, RND_Y+64, 1, 25, CYAN);
      redrawRandomMode();
      buttonOff(RND_X, RND_Y, 6, "RND");
      buttonOff(CLR_X, CLR_Y, 6, "CLR");
      updateMainWaveformDisplay();
    } else if (!wav) {
      buttonOff(WAV_X, WAV_Y, 6, "WAV");
    }
    wavOld = wav;
  }
}

// Draws envelope screen select button
void drawEnv() {
  if (envState != envStateOld) {
    if (envState == 1) {  // momentary pressed
      env = !env;
    }
    // Update the old state of the momentary
    envStateOld = envState;
  }
  if (env != envOld) {
    if (env == 1) {
      screenLoading = 1;
      buttonOn(ENV_X, ENV_Y, 6, "ENV");
      wav = 0;
      midi = 0;
      generateEnvelopeDisplay();
    } else if (env == 0) {
      buttonOff(ENV_X, ENV_Y, 6, "ENV");
    }
    envOld = env;
  }
}

// Draws MIDI screen select button
void drawMidi() {
  if (midiState != midiStateOld) {
    if (midiState == 1) {  // momentary pressed
      midi = !midi;
    }
    // Update the old state of the momentary
    midiStateOld = midiState;
  }
  if (midi != midiOld) {
    if (midi == 1) {
      screenLoading = 1;
      buttonOn(MIDI_X, MIDI_Y, 6, "MID");
      wav = 0;
      env = 0;
      generateMIDIDisplay();
    } else if (midi == 0) {
      buttonOff(MIDI_X, MIDI_Y, 6, "MID");
    }
    midiOld = midi;
  }
}
// Draws velocity sensitivity mode button, currently not implemented
// void drawVelocityMode() {
//   if (velocityModeState != velocityModeStateOld) {
//     if (velocityModeState == 1) {  // momentary pressed
//       velocityMode = !velocityMode;
//     }
//     // Update the old state of the momentary
//     velocityModeStateOld = velocityModeState;
//   }
//   if (velocityMode != velocityModeOld) {
//     if (velocityMode == 1) {
//       buttonOn(VEL_X, VEL_Y, 6, "VEL");
//       velocitySend = 1;

//     } else if (velocityMode == 0) {
//       buttonOff(VEL_X, VEL_Y, 6, "VEL");
//       velocitySend = 0;
//     }
//     RPC.call("setVelocityMode", velocitySend);
//     velocityModeOld = velocityMode;
//   }
// }

// Draws draw/erase mode button
void drawDrawMode() {
  if (drawState != drawStateOld) {
    if (drawState == 1) {  // momentary pressed
      draw = !draw;
    }
    // Update the old state of the momentary
    drawStateOld = drawState;
  }
  if (draw != drawOld) {
    if (draw == 1) {
      buttonOn(DRAW_MODE_X, DRAW_MODE_Y, 6, "ADD");
      drawMode = 1;

    } else if (draw == 0) {
      buttonOff(DRAW_MODE_X, DRAW_MODE_Y, 6, "SUB");
      drawMode = 0;
    }
    drawOld = draw;
  }
}

void redrawDrawMode(){
    if (draw == 1) {
      buttonOn(DRAW_MODE_X, DRAW_MODE_Y, 6, "ADD");
    } else if (draw == 0) {
      buttonOff(DRAW_MODE_X, DRAW_MODE_Y, 6, "SUB");
    }
  }

void redrawRandomMode() {
  if (rndMode == 1) {
    buttonOn(RND_MODE_X, RND_MODE_Y, 6, "SQR");
  } else if (rndMode == 0) {
    buttonOff(RND_MODE_X, RND_MODE_Y, 6, "CUR");
  }
}

// Draws random mode button
void drawRandomMode() {
  if (rndModeState != rndModeStateOld) {
    if (rndModeState == 1) {  // momentary pressed
      rndMode = !rndMode;
    }
    // Update the old state of the momentary
    rndModeStateOld = rndModeState;
  }
  if (rndMode != rndModeOld) {
    if (rndMode == 1) {
      buttonOn(RND_MODE_X, RND_MODE_Y, 6, "SQR");
    } else if (rndMode == 0) {
      buttonOff(RND_MODE_X, RND_MODE_Y, 6, "CUR");
    }

    RPC.call("setRandomMode", rndMode);

    rndModeOld = rndMode;
  }
}

// Handle touch within the main waveform area
void drawWaveform(int x, int y, int step_size) {
  int step_size_multiplier = step_size; // Controls width of bars drawn

  if (x >= WAVEFORM_DISPLAY_X && x <= WAVEFORM_DISPLAY_X + LUT_DISPLAY_BIT_DEPTH && y >= LEFT_BOUNDARY && y <= (LEFT_BOUNDARY + lut_size) && wav) {

    int lutIndex = (y - LEFT_BOUNDARY);                  // Map Y to LUT index
    int blockIndex = lutIndex / step_size_multiplier;    // Determine which block the LUT index belongs to
    int blockStart = blockIndex * step_size_multiplier;  // Calculate the start of the block
    int blockEnd = blockStart + step_size_multiplier;    // Calculate the end of the block
    int newValue;

    // Ensure block indices stay within bounds
    if (blockStart >= 0 && blockEnd <= lut_size) {
      if (drawMode) {
        newValue = map(x, WAVEFORM_DISPLAY_X, WAVEFORM_DISPLAY_X + LUT_DISPLAY_BIT_DEPTH, 0, 255);  // Map X to new LUT value
      } else {
        newValue = 0;
      }

      // Update LUT values for the entire block and redraw it
      for (int i = blockStart; i < blockEnd; ++i) {
        int upscaledNewValue = newValue * BIT_SCALER;
        RPC.call("updateLUT", i, upscaledNewValue);
      }

      // Draw the block on the display
      for (int i = blockStart; i < blockEnd; ++i) {
        int displayY = i + 144;                                                               // Map LUT index back to Y-coordinate on display
        display.fillRect(WAVEFORM_DISPLAY_X, displayY, newValue, 1, CYAN);                    // Filled part
        display.fillRect(WAVEFORM_DISPLAY_X + newValue, displayY, 255 - newValue, 1, WHITE);  // Remaining part
      }
    }
    selectedButton = -1;
  }
}

// Handle touch within the volume slider
void volumeSlider(int x, int y) {
  if (x >= VOL_X && x <= VOL_X + BUTTON_HEIGHT && y >= VOL_Y && y <= VOL_Y + 401) {
    display.fillRect(VOL_X + 1, VOL_Y + 1, BUTTON_HEIGHT, y - VOL_Y + 1, CYAN);
    display.fillRect(VOL_X + 1, y + 1, BUTTON_HEIGHT, VOL_Y + 401 - y, WHITE);
    float volume = fmap(float(y), VOL_Y, VOL_Y + 402, 0.0, 1.0);
    RPC.call("setVolume", volume);
  }
}

// Handle touch within the adsr sliders
void ADSRSlider(int x, int y) {
  if (env == 1) {
    if (x >= ATTACK_X && x <= ATTACK_X + BUTTON_HEIGHT && y >= ATTACK_Y && y <= ATTACK_Y + 401) {
      attackVal = y - ATTACK_Y + 1;
      display.fillRect(ATTACK_X + 1, ATTACK_Y + 1, BUTTON_HEIGHT, attackVal, CYAN);
      display.fillRect(ATTACK_X + 1, y + 1, BUTTON_HEIGHT, 400 - attackVal, WHITE);
      attack = logmap(float(attackVal), 0, 400, 0.005, ATTACK_RANGE);
      RPC.call("setAttack", attack);
    }

    if (x >= DECAY_X && x <= DECAY_X + BUTTON_HEIGHT && y >= DECAY_Y && y <= DECAY_Y + 401) {
      decayVal = y - DECAY_Y + 1;
      display.fillRect(DECAY_X + 1, DECAY_Y + 1, BUTTON_HEIGHT, decayVal, CYAN);
      display.fillRect(DECAY_X + 1, y + 1, BUTTON_HEIGHT, 400 - decayVal, WHITE);
      decay = logmap(float(decayVal), 0, 400, 0.005, DECAY_RANGE);
      RPC.call("setDecay", decay);
    }

    if (x >= SUSTAIN_X && x <= SUSTAIN_X + BUTTON_HEIGHT && y >= SUSTAIN_Y && y <= SUSTAIN_Y + 401) {
      sustainVal = y - SUSTAIN_Y + 1;
      display.fillRect(SUSTAIN_X + 1, SUSTAIN_Y + 1, BUTTON_HEIGHT, sustainVal, CYAN);
      display.fillRect(SUSTAIN_X + 1, y + 1, BUTTON_HEIGHT, 400 - sustainVal, WHITE);
      sustain = fmap(float(sustainVal), 0, 400, 0.0, SUSTAIN_RANGE);
      RPC.call("setSustain", sustain);
    }

    if (x >= RELEASE_X && x <= RELEASE_X + BUTTON_HEIGHT && y >= RELEASE_Y && y <= RELEASE_Y + 401) {
      releaseVal = y - RELEASE_Y + 1;
      display.fillRect(RELEASE_X + 1, RELEASE_Y + 1, BUTTON_HEIGHT, releaseVal, CYAN);
      display.fillRect(RELEASE_X + 1, y + 1, BUTTON_HEIGHT, 400 - releaseVal, WHITE);
      release = logmap(float(releaseVal), 0, 400, 0.005, RELEASE_RANGE);
      RPC.call("setRelease", release);
    }
  }
}

// Handle touch within the MIDI sliders
void midiSlider(int x, int y) {
  if (midi == 1) {
    if (x >= ATTACK_X && x <= ATTACK_X + BUTTON_HEIGHT && y >= ATTACK_Y && y <= ATTACK_Y + 401) {
      pbRangeSlider = y - ATTACK_Y + 1;
      display.fillRect(ATTACK_X + 1, ATTACK_Y + 1, BUTTON_HEIGHT, pbRangeSlider, CYAN);
      display.fillRect(ATTACK_X + 1, y + 1, BUTTON_HEIGHT, 400 - pbRangeSlider, WHITE);
      pbRange = map(pbRangeSlider, 0, 400, 0, 24);
      valuesText(ATTACK_TEXT_X, ATTACK_TEXT_Y + 52, pbRange, PB_steps);
      RPC.call("setPitchBend", pbRange); // Update range in M7
    }

    if (x >= DECAY_X && x <= DECAY_X + BUTTON_HEIGHT && y >= DECAY_Y && y <= DECAY_Y + 401) {
      midiChannelSlider = y - DECAY_Y + 1;
      display.fillRect(DECAY_X + 1, DECAY_Y + 1, BUTTON_HEIGHT, midiChannelSlider, CYAN);
      display.fillRect(DECAY_X + 1, y + 1, BUTTON_HEIGHT, 400 - midiChannelSlider, WHITE);
      midiChannel = map(midiChannelSlider, 0, 400, 0, 16);
      valuesText(DECAY_TEXT_X, DECAY_TEXT_Y + 52, midiChannel, CH_steps);
      RPC.call("setMidiChannel", midiChannel); // Update channel in M7
    }
  }
}

// Generates and updates wavetable buttons state (rnd & clr buttons also update this)
void generateWavetableButtons() {
  for (int i = 0; i < wavs_size; i++) {
    // Determine colour for outline and wavetable button
    int colour = (i == selectedButton) ? CYAN : GREY;
    // Draw button outline
    display.drawRect(BOTTOM_BAR_X, (LEFT_BOUNDARY + (89 * i)), BUTTON_HEIGHT + 2, BUTTON_WIDTH + 2, colour);
    // Draw waveform inside button
    for (int j = 0; j < 64; j++) {
      int downsampledIndex = j * downsample_factor;
      auto downsampled_value = RPC.call("getWavLutElement", i, downsampledIndex).as<int>();
      display.fillRect((BOTTOM_BAR_X + 1), ((LEFT_BOUNDARY + 1) + (89 * i)) + j, downsampled_value / BIT_SCALER / 8, 1, colour);
    }
  }
}

// Updates random step size buttons state
void generateRandomStepButtons() {
  for (int i = 0; i < steps_size; i++) {
    // Determine colour for outline and wavetable button
    int stepBoxColour = (i == selectedStep) ? CYAN : WHITE;
    int stepTextColour = (i == selectedStep) ? WHITE : CYAN;
    int xOffset = 5;
    // Draw button outline
    display.drawRect((LEFT_BAR_X - (BUTTON_HEIGHT * i)), LEFT_BAR_Y, BUTTON_HEIGHT, BUTTON_WIDTH, CYAN);
    display.fillRect((LEFT_BAR_X - (BUTTON_HEIGHT * i) + 1), LEFT_BAR_Y + 1, 30, BUTTON_WIDTH - 2, stepBoxColour);
    // Draw button text
    display.setRotation(1);  // rotate display so that text is oriented correctly
    int xText = LEFT_BAR_Y;
    int yText = 480 - (LEFT_BAR_X - (BUTTON_HEIGHT * i)) - BUTTON_HEIGHT;
    display.setCursor((xText + xOffset), yText + 5);  // text position
    display.setTextSize(3);                           // adjust text size
    display.setTextColor(stepTextColour);              // set text colour
    display.print(steps[i]);                          // print text
    display.setRotation(0);                           // reset screen orientation
  }
}

// Generates and updates the main waveform display on loading of LUT, waveform drawing, random & clear buttons
void updateMainWaveformDisplay() {
  display.drawRect(WAVEFORM_DISPLAY_X - 1, LEFT_BOUNDARY - 1, 257, 514, CYAN);
  display.fillRect(WAVEFORM_DISPLAY_X, LEFT_BOUNDARY, 255, lut_size, WHITE);  // Clear previous display

  for (int i = 0; i < lut_size; i++) {

    // Retrieve each element of temp_lut from the server
    auto lut_value = RPC.call("getTempLutElement", i).as<int>();
    int downscaledLutValue = lut_value / BIT_SCALER;
    display.fillRect(WAVEFORM_DISPLAY_X, LEFT_BOUNDARY + i, downscaledLutValue, 1, CYAN);  // Draw waveform

    if (i == lut_size - 1) {
      screenLoading = 0;
    }
  }
}
// Used to display ADSR legends
void labelText(int x, int y, String text) {
  display.setRotation(1);  // rotate display so that text is oriented correctly
  int newX = y;
  int newY = 480 - x - BUTTON_HEIGHT;
  display.setCursor(newX, newY);  // text position
  display.setTextSize(3);         // adjust text size
  display.setTextColor(CYAN);     // set text colour
  display.print(text);            // print text
  display.setRotation(0);         // reset screen orientation
}

// Used to display MIDI legends
void valuesText(int x, int y, int val, const char* arrayName[]) {
  display.fillRect(x, y, 32, 59, WHITE);
  display.setRotation(1);  // rotate display so that text is oriented correctly
  int newX = y;
  int newY = 480 - x - BUTTON_HEIGHT;
  display.setCursor(newX, newY);  // text position
  display.setTextSize(3);         // adjust text size
  display.setTextColor(CYAN);     // set text colour
  display.print(arrayName[val]);            // print text
  display.setRotation(0);         // reset screen orientation
}

// Generates an 'on' button at specified coordinates, text position offset and  with button text
void buttonOn(int x, int y, int xOffset, String buttonText) {
  display.fillRect(x, y, BUTTON_HEIGHT, BUTTON_WIDTH, CYAN);
  display.setRotation(1);  // rotate display so that text is oriented correctly
  int xText = y;
  int yText = 480 - x - BUTTON_HEIGHT;
  display.setCursor((xText + xOffset), yText + 5);  // text position
  display.setTextSize(3);                           // adjust text size
  display.setTextColor(WHITE);                      // set text colour
  display.print(buttonText);                        // print text
  display.setRotation(0);                           // reset screen orientation
}

// Generates an 'off' button at specified coordinates, text position offset and  with button text
void buttonOff(int x, int y, int xOffset, String buttonText) {
  display.fillRect(x, y, BUTTON_HEIGHT, BUTTON_WIDTH, WHITE);  // clear previous 'ON' filled rectangle
  display.drawRect(x, y, BUTTON_HEIGHT, BUTTON_WIDTH, CYAN);
  display.setRotation(1);
  int xText = y;
  int yText = 480 - x - BUTTON_HEIGHT;
  display.setCursor((xText + xOffset), yText + 5);
  display.setTextSize(3);
  display.setTextColor(CYAN);
  display.print(buttonText);
  display.setRotation(0);
}

void setup() {
  // Initialize RPC communication
  RPC.begin();

  // Initialize touchscreen
  touchDetector.begin();
  touchDetector.onDetect(gigaTouchHandler);

  // Begin drawing display
  display.begin();
  display.fillScreen(WHITE);

  // Draws waveform boundary box
  display.drawRect(WAVEFORM_DISPLAY_X - 1, LEFT_BOUNDARY - 1, 257, 514, CYAN);

  // Draws volume bar and sets volume level to max
  display.drawRect(VOL_X, VOL_Y, BUTTON_HEIGHT + 2, 403, CYAN);
  display.fillRect(VOL_X + 1, VOL_Y + 1, BUTTON_HEIGHT, 401, CYAN);

  // Draws volume label
  labelText(VOL_TEXT_X, VOL_TEXT_Y, "VOL");

  display.fillRect(RND_X+16, RND_Y+64, 1, 25, CYAN); // line joining rnd & rnd mode

}

void loop() {

  if (millis() - lastTouchHandler > threshold) {
    clr = 0;
    rnd = 0;
    panic = 0;
    // onState = 0;
    drawState = 0;
    rndModeState = 0;
    velocityModeState = 0;
  }

  delay(10);
  drawRandom();
  drawClear();
  drawPanic();
  // drawOn();
  drawDrawMode();
  // drawVelocityMode();
  drawRandomMode();
  drawEnv();
  drawWav();
  drawMidi();
  waveformSelection();
}
