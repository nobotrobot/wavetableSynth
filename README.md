# Arduino GIGA Wavetable Synthesiser #

#### An interactive polyphonic wavetable synthesiser for the Arduino® GIGA R1 WiFi & GIGA Display Shield ####

![P1077058-HDR Large](https://github.com/user-attachments/assets/e8ba34bb-b105-439a-b869-094baa0ce5bb)

This project is a MIDI controlled polyphonic wavetable synthesiser that allows real-time drawing of waveforms using a touchscreen.  It uses the <a href="Arduino GIGA R1 WiFi">Arduino GIGA R1 WiFi</a>, <a href="https://store.arduino.cc/products/giga-display-shield">GIGA Display Shield</a>, a 3d-printed case and an <a href="https://www.olimex.com/Products/Duino/Shields/SHIELD-MIDI/open-source-hardware">Olimex MIDI Shield</a> (although any midi input circuit that connects with RX0 on the GIGA should work). 

Many people will already be familiar with subtractive synthesis and FM synthesis. Subtractive synthesis is the most common synthesis method found in analog synthesisers, and works by removing frequencies in harmonically complex waveforms using filters. FM synthesis on the other hand creates complex harmonic overtones by modulating the frequency of one waveform with another waveform. Wavetable synthesis exists somewhere between these two, providing the warmth of subtractive synthesis with the digital complexity of FM synthesis. It uses digital representations of waveforms, which can be as complex as the wavetable allows. It can also be more efficient than analog modelling of waveforms, as waveforms are simply read and played back, requiring minimal real-time computation. 

This particular synthesiser allows the user to draw and edit waveforms using the GIGA Display Shield touchscreen, interacting directly with the wavetable at the sample level. It is velocity sensitive, up to 16 note polyphonic, has a simple ADSR envelope and also a function for generating complex random waveforms which can then be further edited. 
The  program also makes full use of the GIGA’s dual cores, partitioning the flash memory and running the synth engine on the Cortex-M7 and the interactive display on the Cortex-M4. 

#### Instructions ####

First, if you don't already have it, you need to install the <a href="https://www.arduino.cc/en/software">Arduino IDE</a> software. Once you have that, you need to install the Arduino_GigaDisplay_GFX, Arduino_GigaDisplayTouch, Arduino_AdvancedAnalog and 'MIDI library' libraries from the Arduino Library Manager. 

Next, before you upload the code to your GIGA, you need to partition the memory. After connecting your GIGA and selecting Arduino Giga R1 in the Arduino IDE in Tools > Board: > Arduino Mbed OS Giga Boards > Arduino Giga R1, go to Tools > Flash split: and select 1.5MB M7 + 0.5MB M4. There are also full instructions for this <a href="https://docs.arduino.cc/tutorials/giga-r1-wifi/giga-dual-core/#partitioning-the-flash-memory">on the Arduino website</a>. 

You can now program each core separately. Make sure that the M4 and M7 sketches are in separate folders, and that the M7 sketch has wavetables.cpp and wavetables.h in the same folder. When you compile and upload to the M7 core, make sure that Tools > Target core: "Main Core" is selected beforehand. To compile and upload to the M4 core, select Tools > Target core: "M4 Co-processor". 

#### Usage ####

![Frame 1](https://github.com/user-attachments/assets/00b58462-38d3-4a91-abd7-7b3e4f9ce1fe)

When the program starts up you will be presented with a screen containing a sine wave. This is the main screen for drawing waveforms which the synth engine will use in its sound generation. WAVEFORMS contains four sample waveforms (sine wave, square wave, sawtooth wave and a pipe organ sound) which can be loaded by clicking on the respective icon. You can draw in the waveform area directly; when you begin you will be drawing bars which each represent one sample in the wavetable, but this can be changed by choosing one of the buttons in the BLOCKSIZE section, which selects how many bars the screen is divided up into. The DRAW/ERASE button selects whether you are drawing the bars ("ADD") or erasing ("SUB"). The CLR button clears the wavetable. 

The RND button in the RANDOM section generates a random waveform, and this waveform is dependent upon the BLOCKSIZE and the random mode button to the right of the RND button (CUR/SQR). The BLOCKSIZE determines how many points are generated on the screen, and the random mode button determines how the values between these points are interpolated; CUR will interpolate using a curve function whereas SQR will create blocks of the same value between each point. With CUR enabled you can generate some excellent organ emulations, whereas SQR, particularly at the lower block size values, can create great chip-style sounds. 

With the SCREEN SELECT buttons you can choose to edit the ADSR envelope (ENV), the midi parameters (MID), or return to the main waveform drawing screen (WAV). 

![Frame 2](https://github.com/user-attachments/assets/7aa9c011-7748-46ca-a767-e93043afebd2)

The envelope screen contains a simple implementation of an ADSR envelope to be used by the synth engine. 

![Frame 3](https://github.com/user-attachments/assets/e75c9472-e446-4bd7-b120-b9320b9b1a2b)

The MIDI settings screen allows you to control the range of pitch bend (+/- 24 semitones), which midi channel the synth is set to receive on, and also features a panic button for silencing stuck notes by sending an all notes off message. 
