#include "Stored.h"
#include "Hardware.h"
#include "Midi.h"

using namespace Stored;
using namespace Hardware;

namespace Midi
{
  bool padStates[PADS_COUNT] = {false};
  bool previousPadStates[PADS_COUNT] = {false};
  bool midiStates[PADS_COUNT] = {false};
  bool previousMidiStates[PADS_COUNT] = {false};
  ActiveNotes activeNotes;

  void sendNoteOff(int note, int velocity, int channel)
  {
    if (channel < 0 || channel > 15)
      return;
    uint8_t status = 0x80 | (channel);
    uint8_t usb_packet[] = {status, (uint8_t)note, (uint8_t)velocity};
    usb_midi.write(usb_packet, 3);
    Serial1.write(status);
    Serial1.write(note);
    Serial1.write(velocity);
  }

  void killChannelNotes(int channelNoteCounts[], int midiChannel)
  {
    for (int i = 0; i < 128; i++)
    {
      if (channelNoteCounts[i] > 0)
      {
        sendNoteOff(i, 0, midiChannel);
        channelNoteCounts[i] = 0;
      }
    }
  }

  // Menu helper function to kill all notes in reference
  void killAllNotes()
  {
    killChannelNotes(activeNotes.channelA, settings.midiOutputAChannel);
    killChannelNotes(activeNotes.channelB, settings.midiOutputBChannel);
    killChannelNotes(activeNotes.channelC, settings.midiOutputCChannel);
    killChannelNotes(activeNotes.channelD, settings.midiOutputDChannel);
  }

  void forwardMIDI(uint8_t status, uint8_t data1, uint8_t data2)
  {
    // Forward to USB MIDI
    uint8_t usb_packet[] = {status, data1, data2};
    usb_midi.write(usb_packet, 3);

    // Forward to Serial MIDI
    Serial1.write(status);
    Serial1.write(data1);
    Serial1.write(data2);
  }

  void processIncomingMIDI(uint8_t status, uint8_t data1, uint8_t data2)
  {
    uint8_t command = status & 0xF0; // Mask out the channel bits
    uint8_t channel = status & 0x0F; // Get the channel
    if (settings.midiThru)
    {
      forwardMIDI(status, data1, data2);
    }

    if (channel == settings.midiInChannel && (command == 144 || command == 128))
    {
      for (int i = 0; i < PADS_COUNT; i++)
      {
        if (data1 == pads[i].triggerNote)
        {
          if (data2 > 0 && command == 144)
          {
            midiStates[i] = true;
          }
          else
          {
            midiStates[i] = false;
          }
        }
      }
    }
  }

  void updateMIDI()
  {
    for (int i = 0; i < PADS_COUNT; i++)
    {
      previousMidiStates[i] = midiStates[i];
      previousPadStates[i] = padStates[i];
    }

    if (midiMessageReady)
    {
      midiMessageReady = false; // Reset the flag
      // Process the MIDI message (status, data1, data2)
      processIncomingMIDI(status, data1, data2);
    }

    // Check for incoming USB MIDI messages
    if (usb_midi.available())
    {
      uint32_t packet = usb_midi.read();
      uint8_t status = (packet >> 8) & 0xFF; // Status byte
      uint8_t data1 = (packet >> 16) & 0xFF; // Data1 (e.g., note)
      uint8_t data2 = (packet >> 24) & 0xFF; // Data2 (e.g., velocity)
      processIncomingMIDI(status, data1, data2);
    }
  }

  void sendNoteOn(int note, int velocity, int channel)
  {
    if (channel < 0 || channel > 15)
      return;
    // 0x90 is "Note On", and channel is adjusted to 0-based
    uint8_t status = 0x90 | (channel);
    uint8_t usb_packet[] = {status, (uint8_t)note, (uint8_t)velocity};
    usb_midi.write(usb_packet, 3);
    Serial1.write(status);
    Serial1.write(note);
    Serial1.write(velocity);
  }

  void stopChannelNote(int channelNoteCounts[], int note, int midiChannel)
  {
    if (channelNoteCounts[note] > 0)
    {
      channelNoteCounts[note]--;
      if (channelNoteCounts[note] == 0)
      {
        sendNoteOff(note, 0, midiChannel);
      }
    }
  }

  void stopNote(int pad, int j)
  {
    int note = constrain(settings.rootNote +
                             pads[pad].chord.intervals[j] +
                             pads[pad].chord.octaveModifiers[j] * 12 +
                             pads[pad].chord.semitoneModifiers[j],
                         12, 127);

    switch (pads[pad].chord.channel[j])
    {
    case 0:
      stopChannelNote(activeNotes.channelA, note, settings.midiOutputAChannel);
      break;
    case 1:
      stopChannelNote(activeNotes.channelB, note, settings.midiOutputBChannel);
      break;
    case 2:
      stopChannelNote(activeNotes.channelC, note, settings.midiOutputCChannel);
      break;
    case 3:
      stopChannelNote(activeNotes.channelD, note, settings.midiOutputDChannel);
      break;
    }
  }

  void playChannelNote(int channelNoteCounts[], int note, int velocity, int midiChannel)
  {
    if (channelNoteCounts[note] > 0)
    {
      sendNoteOff(note, velocity, midiChannel);
    }
    channelNoteCounts[note]++;
    sendNoteOn(note, velocity, midiChannel);
  }

  void playNote(int pad, int j)
  {
    int note = constrain(settings.rootNote +
                             pads[pad].chord.intervals[j] +
                             pads[pad].chord.octaveModifiers[j] * 12 +
                             pads[pad].chord.semitoneModifiers[j],
                         12, 127);
    float velocitySum = (settings.velocityScaling * pads[pad].padVelocity) +
                        pads[pad].chord.velocityModifiers[j] +
                        random(-pads[pad].velocityRandom, pads[pad].velocityRandom);
    int velocity = constrain(velocitySum, 1, 127);

    switch (pads[pad].chord.channel[j])
    {
    case 0:
      playChannelNote(activeNotes.channelA, note, velocity, settings.midiOutputAChannel);
      break;
    case 1:
      playChannelNote(activeNotes.channelB, note, velocity, settings.midiOutputBChannel);
      break;
    case 2:
      playChannelNote(activeNotes.channelC, note, velocity, settings.midiOutputCChannel);
      break;
    case 3:
      playChannelNote(activeNotes.channelD, note, velocity, settings.midiOutputDChannel);
      break;
    }
  }

  void stopChord(int i)
  {
    for (int j = 0; j < 8; j++)
    {
      if (pads[i].chord.isActive[j])
      {
        stopNote(i, j);
      }
    }
  }

  void playChord(int i)
  {
    for (int j = 0; j < 8; j++)
    {
      if (pads[i].chord.isActive[j])
      {
        playNote(i, j);
      }
    }
  }
}
