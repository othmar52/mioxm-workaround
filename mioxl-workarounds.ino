 
// alternative wiring for debugging
//
// instead of using RX, TX pins of hardware serial use different pins
// so we are able to use the debug monitor of Arduino IDE
//#define USE_SOFTWARE_SERIAL_PIN_12_13

// thanks to https://github.com/FortySevenEffects/arduino_midi_library
#include <MIDI.h>

#ifdef USE_SOFTWARE_SERIAL_PIN_12_13
#include <SoftwareSerial.h>
SoftwareSerial softSerial(12, 13); // RX, TX
MIDI_CREATE_INSTANCE(SoftwareSerial, softSerial, MIDI);
#else
MIDI_CREATE_DEFAULT_INSTANCE();
#endif



void setup() {
#ifdef USE_SOFTWARE_SERIAL_PIN_12_13
  Serial.begin(115200);
  Serial.println("starting serial with debug monitor");
  Serial.println("MIDI RX/TX pins are 12/13");
#endif
  
  MIDI.begin(MIDI_CHANNEL_OMNI); // Launch MIDI, by default listening to all channels
  MIDI.turnThruOff(); // we have to avoid channel based events beeing sent thru

  // set callbacks for all different event types
  MIDI.setHandleClock(handleMidiEventClock);
  MIDI.setHandleTick(handleMidiEventClock);
  MIDI.setHandleStop(handleMidiEventStop);
  MIDI.setHandleStart(handleMidiEventStart);
  MIDI.setHandleContinue(handleMidiEventContinue);
  MIDI.setHandleNoteOn(handleMidiEventNoteOn);
  MIDI.setHandleNoteOff(handleMidiEventNoteOff);
  MIDI.setHandleAfterTouchPoly(handleMidiEventAfterTouchPoly);
  MIDI.setHandleAfterTouchChannel(handleMidiEventAfterTouchChannel);
  MIDI.setHandleProgramChange(handleMidiEventProgramChange);
  MIDI.setHandleControlChange(handleMidiEventControlChange);
  MIDI.setHandlePitchBend(handleMidiEventPitchBend);
  MIDI.setHandleSystemExclusive(handleMidiEventSysEx);
  MIDI.setHandleTimeCodeQuarterFrame(handleMidiEventTimeCodeQuarterFrame);
  MIDI.setHandleSongPosition(handleMidiEventSongPosition);
  MIDI.setHandleSongSelect(handleMidiEventSongSelect);
  MIDI.setHandleTuneRequest(handleMidiEventTuneRequest);
  MIDI.setHandleSystemReset(handleMidiEventSystemReset);

}

void loop() {
  MIDI.read();
}


void handleMidiEventNoteOn(byte inChannel, byte inPitch, byte inVelocity) {
  MIDI.sendNoteOn(inPitch, inVelocity, inChannel);
}
void handleMidiEventNoteOff(byte inChannel, byte inPitch, byte inVelocity) {
  MIDI.sendNoteOff(inPitch, inVelocity, inChannel);
}

void handleMidiEventAfterTouchPoly(byte inChannel, byte inNote, byte inValue) {
  MIDI.sendAfterTouch(inNote, inValue, inChannel);
}

void handleMidiEventControlChange(byte inChannel, byte inControlNumber, byte inControlValue) {
  // transform incoming MIDI CC 80 with value 127 on MIDI channel 6 to realtime start
  if (inChannel == 6 && inControlNumber == 80 && inControlValue == 127) {
    MIDI.sendStart();
    // to not pass thru the incoming event
    return;
  }
  MIDI.sendControlChange(inControlNumber, inControlValue, inChannel);
}

void handleMidiEventProgramChange(byte inChannel, byte inProgramNumber) {
  MIDI.sendProgramChange(inProgramNumber, inChannel);
}

void handleMidiEventAfterTouchChannel(byte inChannel, byte inPressure) {
  MIDI.sendAfterTouch(inPressure, inChannel);
}

void handleMidiEventPitchBend(byte inChannel, int inPitchValue) {
  MIDI.sendPitchBend(inPitchValue, inChannel);
}

void handleMidiEventSysEx(byte* inData, unsigned inSize)
{
    MIDI.sendSysEx(inSize, inData, true);
}

void handleMidiEventTimeCodeQuarterFrame(byte inData)
{
    MIDI.sendTimeCodeQuarterFrame(inData);
}

void handleMidiEventSongPosition(unsigned inBeats)
{
    MIDI.sendSongPosition(inBeats);
}

void handleMidiEventSongSelect(byte inSongNumber)
{
    MIDI.sendSongSelect(inSongNumber);
}
void handleMidiEventTuneRequest()
{
    MIDI.sendTuneRequest();
}

void handleMidiEventSystemReset() {
  MIDI.sendSystemReset();
}

void handleMidiEventClock() {
  MIDI.sendClock();
}

void handleMidiEventStart() {
  MIDI.sendStart();
}

void handleMidiEventStop() {
  MIDI.sendStop();
}

void handleMidiEventContinue() {
  MIDI.sendContinue();
}
