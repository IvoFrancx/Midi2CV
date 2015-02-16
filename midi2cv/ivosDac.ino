

//0
///c  = 0;
///c# = 337;
///d  = 420;
///d# = 505;
///e  = 590;
///f  = 676;
///f# = 762;
///g  = 847;
///g# = 930;
///a  = 1015;
///a# = 1098;
///b  = 1181; 

//1
///c  = 1268;
///c# = 1353;
///d  = 1438;
///d# = 1523;
///e  = 1604;
///f  = 1686;
///f# = 1770;
///g  = 1852;
///g# = 1935;
///a  = 2020;
///a# = 2104;
///b  = 2189; 

int notes_46[] = {
  0,     337,  420,  505,  590,  676,  762,  847,  930, 1015, 1098, 1181, 
  1268, 1353, 1438, 1523, 1604, 1686, 1770, 1852, 1935, 2020, 2104, 2189, 
  2272, 2357, 2442, 2527, 2611, 2695, 2780, 2864, 2949, 3033, 3117, 3202, 
  3286, 3370, 3454, 3539, 3621, 3702, 3789, 3873, 3958, 4044};

//2
///c  = 2272;
///c# = 2357;
///d  = 2442;
///d# = 2527;
///e  = 2611;
///f  = 2695;
///f# = 2780;
///g  = 2864;
///g# = 2949;
///a  = 3033;
///a# = 3117;
///b  = 3202; 

//3
///c  = 3286;
///c# = 3370;
///d  = 3454;
///d# = 3539;
///e  = 3621;
///f  = 3702;
///f# = 3789;
///g  = 3873;
///g# = 3958;
///a  = 4044;
///a# = ;
///b  = ; 

#include <SPI.h>
#include <MIDI.h>
#include <midi_Defs.h>
#include <midi_Message.h>
#include <midi_Namespace.h>
#include <midi_Settings.h>

MIDI_CREATE_DEFAULT_INSTANCE();

const int led = 2;
const int SS_PIN = 10; //SPI select slave pin
const int gate_1 = 7;

int val,serialGet = 0;
int preValue;




void setup () {
  pinMode(led, OUTPUT);
  pinMode(SS_PIN, OUTPUT);
  pinMode(gate_1, OUTPUT);
  SPI.begin();
  MIDI.begin(MIDI_CHANNEL_OMNI);
  MIDI.setHandleNoteOn(HandleNoteOn);
  MIDI.setHandleNoteOff(HandleNoteOff);
  MIDI.setHandleControlChange(HandleControlChange);
}

void HandleNoteOn(byte channelIn, byte pitchIn, byte velocity) { 

  // Do whatever you want when you receive a Note On.
  if(pitchIn >= 60) {
    digitalWrite(gate_1, HIGH);
    digitalWrite(2, HIGH);
    sendNote(notes_46[pitchIn-60], 0);
    digitalWrite(led,HIGH);
  }

  if (velocity == 0) {
    // This acts like a NoteOff.
    digitalWrite(gate_1, LOW);
    digitalWrite(2, LOW);
    digitalWrite(led,LOW);
  }

  // Try to keep your callbacks short (no delays ect) as the contrary would slow down the loop()
  // and have a bad impact on real-time performance.
}

void HandleNoteOff(byte channelIn, byte pitchIn, byte velocity) { 
  digitalWrite(gate_1, LOW);
  digitalWrite(2, LOW);
  digitalWrite(led,LOW);
}

void sendNote (int val, int channel) {
  int pitch = val; //
  digitalWrite(SS_PIN, LOW); //DAC is selected when LOW
  SPI.transfer(0x0f + (pitch >> 8) | channel << 7);
  // SPI.transfer(0x0f + (pitch >> 8));
  SPI.transfer(pitch & 0xff); // - 0x80
  digitalWrite(SS_PIN, HIGH); //Deselect DAC
}

void HandleControlChange (byte channel, byte number, byte value) {
 // value *= 32; ///get incoming CCs to 12-bit
    if (number == 1) {  
      sendNote(value*32, 1);
    }
}

void loop () {
  MIDI.read();
  
  

}


/*
  if (number == 1 && value < preValue) {
    digitalWrite(led, HIGH);
    for (int i=value;i++;i<preValue) {
      sendNote(i, 1);
      value=i;
    }
    preValue = value;
  }
  
    if (number == 1 && value > preValue) {
    for (int i=value;i--;i>preValue) {
      sendNote(i, 1);
      value=i;
    }
    preValue = value;
  }
*/
