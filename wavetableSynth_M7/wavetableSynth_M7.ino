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

// DAC & MIDI (M7 core)

#include <Arduino_AdvancedAnalog.h>    // DAC
#include "wavetables.h"                // Definitions for wavetables
#include <MIDI.h>
#include <RPC.h>

MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);

// AdvancedDAC dac0(A12);
AdvancedDAC dac1(A13);

unsigned long time_now = millis();  // for timer function, seed for random
uint16_t* lut = sine_wave;  // Initialize LUT with the sine wave LUT

uint16_t temp_lut[512];             // Temporary LUT for editing current waveform
float volume = 1.0;                // Volume level, 0.0 (mute) to 1.0 (max)

// ADSR parameters
float attack_time = 0.01;  // seconds
float decay_time = 0.01;    // seconds
float sustain_level = 1.0; // sustain level (0.0 to 1.0)
float release_time = 0.25;  // seconds

float semitone_range = 2.0; 

bool velocityMode = 1;
bool randomModeM7 = 0;

int pitch_bend = 0; // Global pitch bend value (-8192 to 8191)

// array of LUTs
const uint16_t* wave_luts[4] = { sine_wave, square_wave, sawtooth_wave, organ_pipe_wave };

// size of look-up table for DAC
static size_t lut_size = sizeof(sine_wave) / sizeof(sine_wave[0]);

const float sample_rate = 192000.0;
// const float sample_rate = 768000.0; // For stereo

// Used to lower MIDI latency (5ms) although only one DAC can be used
const int n_samples = 512;       // Define n_samples here
const int n_buffers = 4;         // Define n_buffers here

// Use if you want stereo - MIDI latency is much higher though (170 ms)
// const int n_samples = lut_size*8;
// const int n_buffers = 32;

// ADSR envelope stages
enum ADSRStage { ATTACK, DECAY, SUSTAIN, RELEASE, OFF };

// Structure for each voice
struct Voice {
  float frequency;
  float phase_accumulator;
  float phase_increment;
  ADSRStage stage;
  float adsr_level;
  int velocity;
  bool active;
};

// #define NUM_VOICES 4 // For stereo

#define NUM_VOICES 16
Voice voices[NUM_VOICES];

void startDAC(int dacMode){
  if (dacMode){
  dac1.begin(AN_RESOLUTION_12, sample_rate, n_samples, n_buffers);
  // dac1.begin(AN_RESOLUTION_8, sample_rate, n_samples, n_buffers);
  }
  else if (!dacMode){
  dac1.stop();
  // dac1.stop();
  }
}

void setVelocityMode(int velocityModeIn){
  if (velocityModeIn == 1){
    velocityMode = 1;
  }
  else {
    velocityMode = 0;
  }
}

void handlePitchBend(byte channel, int bendValue) {
  pitch_bend = bendValue; 
}

void setPitchBend(int range) {
  semitone_range = range; 
}

void setMidiChannel(int channel) {
  if (channel == 0){
    MIDI.begin(MIDI_CHANNEL_OMNI);
  }
  else {
    MIDI.begin(channel);
  }
}

// Convert to range to be used by voices
float calculatePitchBendFactor(int bendValue) {
  // Pitch bend range set to +/- 2 semitones
  return pow(2.0, (bendValue / 8192.0) * semitone_range / 12.0);
}


// Set up ADSR values for each stage based on sample rate
float adsr_increment[4] = {
  1.0 / (attack_time * sample_rate),
  -1.0 / (decay_time * sample_rate) * (1.0 - sustain_level),
  0.0,
  -1.0 / (release_time * sample_rate)
};

// Convert frequency to phase increment for reading from LUT  
void setVoiceFrequency(Voice& voice, float frequency) {
  voice.frequency = frequency;
  voice.phase_increment = (frequency * lut_size) / sample_rate;
}

void updateADSR(Voice& voice) {
  switch (voice.stage) {
    case ATTACK:
      voice.adsr_level += adsr_increment[ATTACK];
      if (voice.adsr_level >= 1.0) {
        voice.adsr_level = 1.0;
        voice.stage = DECAY;
      }
      break;
    case DECAY:
      voice.adsr_level += adsr_increment[DECAY];
      if (voice.adsr_level <= sustain_level) {
        voice.adsr_level = sustain_level;
        voice.stage = SUSTAIN;
      }
      break;
    case RELEASE:
      voice.adsr_level += adsr_increment[RELEASE];
      if (voice.adsr_level <= 0.0) {
        voice.adsr_level = 0.0;
        voice.stage = OFF;
        voice.active = false;
      }
      break;
    default:
      break;
  }
}

// Map note to frequency
float noteToFreq(int note) {
  float a = 440.0;
  return (a / 32) * pow(2, ((note - 9) / 12.0));
}

// Process and mix all voices
void updateDAC(AdvancedDAC& dac_out) {
  if (dac_out.available()) {
    SampleBuffer buf = dac_out.dequeue();

    float pitch_bend_factor = calculatePitchBendFactor(pitch_bend);

    for (size_t i = 0; i < buf.size(); i++) {
      float sample_sum = 0;

      // Process each voice
      for (int v = 0; v < NUM_VOICES; v++) {
        if (voices[v].active) {
          size_t index = static_cast<size_t>(voices[v].phase_accumulator) % lut_size;
          float sample = temp_lut[index] * volume * (voices[v].velocity / 127.0) * voices[v].adsr_level;
          sample_sum += sample / NUM_VOICES;
          float adjusted_increment = voices[v].frequency * pitch_bend_factor * lut_size / sample_rate;
          voices[v].phase_accumulator += adjusted_increment;

          if (voices[v].phase_accumulator >= lut_size) {
            voices[v].phase_accumulator -= lut_size;
          }
          updateADSR(voices[v]);
        }
      }

      buf[i] = sample_sum;  // Write mixed sample to buffer
    }

    dac_out.write(buf);
  }
}

// Function to turn off all notes
void panic() {
  for (int v = 0; v < NUM_VOICES; v++) {
    if (voices[v].active) {
      voices[v].stage = RELEASE;
    }
  }
}

// MIDI Note On handler 
void handleNoteOn(byte channel, byte pitch, byte velocity) {
  float newFrequency = noteToFreq(pitch);
  
  // Check for an inactive voice
  for (int v = 0; v < NUM_VOICES; v++) {
    if (!voices[v].active) {
      voices[v].active = true;
      voices[v].stage = ATTACK;
      voices[v].adsr_level = 0.0;
      if (velocityMode) {
        voices[v].velocity = velocity;
      } else {
        voices[v].velocity = 127;
      }
      setVoiceFrequency(voices[v], newFrequency);
      return; // Exit after assigning a voice
    }
  }
}

// MIDI Note Off handler
void handleNoteOff(byte channel, byte pitch, byte velocity) {
  float frequency = noteToFreq(pitch);
  for (int v = 0; v < NUM_VOICES; v++) {
    if (voices[v].active && voices[v].frequency == frequency) {
      voices[v].stage = RELEASE;
    }
  }
}

// Set volume function for RPC
void setVolume(float vol) {
  volume = vol;
}

void setAttack(float att){
  attack_time = att;
  adsr_increment[ATTACK] = 1.0 / (attack_time * sample_rate);
}

void setDecay(float dec){
  decay_time = dec;
  adsr_increment[DECAY] = -1.0 / (decay_time * sample_rate) * (1.0 - sustain_level);
}

void setSustain(float sus){
  sustain_level = sus;
  adsr_increment[DECAY] = -1.0 / (decay_time * sample_rate) * (1.0 - sustain_level);
}

void setRelease(float rel){
  release_time = rel;
  adsr_increment[RELEASE] = -1.0 / (release_time * sample_rate);
}

//Set whether random interpolates blocks (1) or curves (0)
void setRandomMode(bool mode){
  randomModeM7 = mode;
}

// Loads LUT into temp_lut
void loadLUT(int index) {
  memcpy(temp_lut, wave_luts[index], 512 * sizeof(uint16_t));
}

void updateLUT(int lutIndex, int newValue){
  temp_lut[lutIndex] = newValue;  // Update temporary LUT
}

// Retrieves a single element from temp_lut
int getTempLutElement(int index) {
  if (index >= 0 && index < lut_size) {
    return temp_lut[index];
  } else {
    return 0;  // Return a default value if index is out of bounds
  }
}

int getLutSize(){
  // size_t lut_size = sizeof(sine_wave) / sizeof(sine_wave[0]);
  // return lut_size;
  // return static_cast<uint32_t>(sizeof(sine_wave) / sizeof(sine_wave[0]));
  return 512;
}

// Retrieves a single element from temp_lut
int getWavLutElement(int wavetable, int index) {
  if (index >= 0 && index < lut_size) {
    return wave_luts[wavetable][index];
  } else {
    return 0; // Return a default value if index is out of bounds
  }
}
// Clamp function to prevent spike/folding issue
int clampToRange(int value, int min, int max) {
  return (value < min) ? min : (value > max) ? max : value;
}

// Generates a LUT of interpolated random values
void generateRandomArray(int step_size) {
  randomSeed(time_now);

  if (randomModeM7 == 1){ 
  // Fills start of every step with a random value
    for (int i = 0; i < lut_size; i += step_size) {
        int randomValue = random(256, 3840);
        temp_lut[i] = randomValue;

  // Fills indexes between steps with same value as that of the preceding step
        for (int j = i; j < i + step_size && j < lut_size; j++) {
            temp_lut[j] = randomValue;
        }
  }
}

else {
  
  // Fills start of every step with a random value
  for (int i = 0; i < lut_size; i += step_size) {
    temp_lut[i] = random(256, 3840);
  }

  //  Cubic interpolation between anchor points
  for (int i = 0; i < lut_size - step_size; i += step_size) {
    // Control points for cubic interpolation
    // (interpolatedValue % 256 + 256) % 256
    int p0 = (i >= step_size) ? temp_lut[i - step_size] : temp_lut[i]; // Previous anchor point (or duplicate of the first if out of bounds)
    int p1 = temp_lut[i]; // Start anchor point
    int p2 = temp_lut[i + step_size]; // End anchor point
    int p3 = (i + 2 * step_size < lut_size) ? temp_lut[i + 2 * step_size] : p2;  // Next anchor point (or duplicate of the last if out of bounds)

    
    // Interpolate between p1 and p2, cubic hermite spline
    for (int j = 1; j < step_size; j++) {
      float t = j / float(step_size);  // Normalized position between points (0 to 1)

      // Cubic Hermite spline formula
      float a0 = -0.5 * p0 + 1.5 * p1 - 1.5 * p2 + 0.5 * p3;
      float a1 = p0 - 2.5 * p1 + 2.0 * p2 - 0.5 * p3;
      float a2 = -0.5 * p0 + 0.5 * p2;
      float a3 = p1;

      // Calculate interpolated value
      int interpolatedValue = (int)(a0 * t * t * t + a1 * t * t + a2 * t + a3);
      // interpolatedValue = (interpolatedValue % 256 + 256) % 256;
      interpolatedValue = clampToRange(interpolatedValue, 0, 4095);

      temp_lut[i + j] = interpolatedValue;
    }
  }

  // Handle the last section 
  int lastStep = ((lut_size / step_size) - 1) * step_size;
  // Step 2: Perform cubic interpolation between anchor points
  for (int i = lastStep; i < lut_size; i += step_size) {
    // Define four control points for cubic interpolation
    int p0 = (i >= step_size) ? temp_lut[i - step_size] : temp_lut[i];           // Previous anchor point (or duplicate the first if out of bounds)
    int p1 = temp_lut[lastStep]; // Start anchor point
    int p2 = temp_lut[0]; // End anchor point
    int p3 = (i + 2 * step_size < lut_size) ? temp_lut[i + 2 * step_size] : p2;  // Next anchor point (or duplicate the last if out of bounds)

    // Interpolate between p1 and p2 using cubic Hermite spline
    for (int j = 1; j < step_size; j++) {
      float t = j / float(step_size);  // Normalized position between points (0 to 1)

      // Cubic Hermite spline formula
      float a0 = -0.5 * p0 + 1.5 * p1 - 1.5 * p2 + 0.5 * p3;
      float a1 = p0 - 2.5 * p1 + 2.0 * p2 - 0.5 * p3;
      float a2 = -0.5 * p0 + 0.5 * p2;
      float a3 = p1;

      // Calculate interpolated value
      int interpolatedValue = (int)(a0 * t * t * t + a1 * t * t + a2 * t + a3);
      // interpolatedValue = (interpolatedValue % 256 + 256) % 256;
      interpolatedValue = clampToRange(interpolatedValue, 0, 4095);

      temp_lut[i + j] = interpolatedValue;
     }
    }
  }
}


// Generates a LUT with all zero values
void clearArray() {
  for (int i = 0; i < lut_size; i++) {
    temp_lut[i] = 0;
  }
}

void setup() {
  Serial.begin(115200);
  // Ignore deepsleep mode in MBed core, without this deep sleep is enabled 
  // after a few minutes and midi input is delayed
  sleep_manager_lock_deep_sleep();

  // Midi handling
  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);
  MIDI.setHandlePitchBend(handlePitchBend);
  MIDI.begin(MIDI_CHANNEL_OMNI);

  if (!dac1.begin(AN_RESOLUTION_12, sample_rate, n_samples, n_buffers)) {
    Serial.println("Failed to start DAC1 !");
    while (1);
  }

  RPC.begin();
  RPC.bind("setVolume", setVolume);
  RPC.bind("loadLUT", loadLUT);
  RPC.bind("generateRandomArray", generateRandomArray);
  RPC.bind("clearArray", clearArray);
  RPC.bind("updateLUT", updateLUT);
  RPC.bind("getTempLutElement", getTempLutElement);
  RPC.bind("startDAC", startDAC);
  RPC.bind("getWavLutElement", getWavLutElement);
  RPC.bind("setAttack", setAttack);
  RPC.bind("setDecay", setDecay);
  RPC.bind("setSustain", setSustain);
  RPC.bind("setRelease", setRelease);
  RPC.bind("setRandomMode", setRandomMode);
  RPC.bind("setVelocityMode", setVelocityMode);
  RPC.bind("panic", panic);
  RPC.bind("setPitchBend", setPitchBend);
  RPC.bind("setMidiChannel", setMidiChannel);

  loadLUT(0);  // Load sine wave by default
}

void loop() {
  MIDI.read();
  updateDAC(dac1);
}
