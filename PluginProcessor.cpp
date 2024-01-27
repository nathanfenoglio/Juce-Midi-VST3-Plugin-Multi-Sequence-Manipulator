/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MidiEffectAudioProcessor::MidiEffectAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
    
#endif
{

}

MidiEffectAudioProcessor::~MidiEffectAudioProcessor()
{
}

//==============================================================================
const juce::String MidiEffectAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool MidiEffectAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool MidiEffectAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool MidiEffectAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double MidiEffectAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int MidiEffectAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int MidiEffectAudioProcessor::getCurrentProgram()
{
    return 0;
}

void MidiEffectAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String MidiEffectAudioProcessor::getProgramName (int index)
{
    return {};
}

void MidiEffectAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void MidiEffectAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void MidiEffectAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool MidiEffectAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void MidiEffectAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    // so all of the buffer business involves the AudioBuffer not the MidiBuffer
    // so maybe could do something similar with getting reference to the MidiBuffer "midiMessages" is what the parameter is called instead

    // trying commenting out the TempoSync tutorial stuff and trying to mimic https://forum.juce.com/t/midi-timing-notes-arpeggiator/28729
    // HEY THIS SEEMS TO BE WORKING PRETTY WELL
    // IT AUTOMATICALLY IS PLAYING THOUGH EVEN WITHOUT PUSHING PLAY IN ABLETON...
    // MIDI % IN ABLETON ISN'T CRAZY EITHER
    // BUT SEEMS LIKE PRETTY CLOSE TO ON THE BEAT, HE SAYS SOMETHING ABOUT THIS CODE SLIPPING A BIT, WHAT DID HE SAY HE ENDED UP CHANGING IDK SEEMS TO BE STEADY 1/8TH NOTES
    // IDK IT DOES SEEM A BIT SLIPPY THOUGH
    // BACK TO THINKING THAT IT SOUNDS OK LOL
    // I think that it's that it only starts on the beat on the 1st play, then every subsequent play results in it starting an 1/8th note off of the beat
    juce::AudioPlayHead* playHead = getPlayHead();
    if (playHead == nullptr) return; // Need a playhead
    juce::AudioPlayHead::CurrentPositionInfo positionInfo;
    playHead->getCurrentPosition(positionInfo);

    juce::MidiMessage midiMessage;
    int samplePos = 0;
    juce::MidiBuffer keepMessages;
    // _midiNoteIdx = 0; // SETTING TO 0 HERE CAUSES THE NOTE TO ALWAYS BE 0???
    if(positionInfo.isPlaying){ // this takes care of not sending midi messages when not playing in Ableton
        // Look for note ons and set held note, unhold with a note off if not playing
        for (juce::MidiBuffer::Iterator it(midiMessages); it.getNextEvent(midiMessage, samplePos);)
        {
            if (midiMessage.isNoteOn()) _heldNote = midiMessage.getNoteNumber();

            if (midiMessage.isNoteOff()) {
                _heldNote = -1;

                // YOU AREN'T EVER GOING INSIDE OF HERE BECAUSE YOU'RE CHECKING if(positionInfo.isPlaying) OUTSIDE
                if (!positionInfo.isPlaying) {
                    // not sure what this is doing, is he talking about letting the very last note sent get turned off since stop was pressed and the playhead shouldn't be moving?
                    keepMessages.addEvent(midiMessage, samplePos); // Let the note off go
                }
            }
        }
        // putting the midi messages that you are creating into the MidiBuffer
        // you actually add them in below with midiMessages.addEvent(juce::MidiMessage::noteOff(1, _heldNote, 0.0f), pos); // Note off
        keepMessages.swapWith(midiMessages); // MIDI messages now contain everything but note ons (we're generating them)
        if (_heldNote > -1)
        {
            _sampleCounter = _sampleCounter + buffer.getNumSamples(); // Increment our counter
        }
        else if (!positionInfo.isPlaying) // If the sequencer isn't playing, reset
        {
            _sampleCounter = 0;
            _nextStepInSamples = 0;
            return;
        }
        auto bpm = positionInfo.bpm; // get bpm from playhead's position info
        auto bps = (bpm / 60); // beats per second
        auto samplesPerStep = (_sampleRate / bps) / 4; // WELL IT SHOWS 44100 IN ABLETON NOT 48000 SO MAKING _sampleRate 44100 INSTEAD, At 120bpm / 48000, this is 6000 samples per step 

        // Time to play
        if (_sampleCounter >= _nextStepInSamples && _heldNote > -1)
        {
            int pos = 0;

            if (_nextStepInSamples > 0) pos = _sampleCounter - _nextStepInSamples - 1;
            if (pos == -1) pos = 0;

            // it's like it's on the beat the 1st time that you run it, but then the 2nd time it's off the beat
            // like it's starting with _wasNoteOn as true that next time
            // still sometimes behind the beat
            if (!positionInfo.isPlaying && _heldNote == -1)
            {
                _wasNoteOn = false; // Reset to false when not playing and no held notes
            }

            if (_wasNoteOn) // A simple bool for the note on / note off toggle
            {
                // midiMessages.addEvent(juce::MidiMessage::noteOff(1, _heldNote, 0.0f), pos); // Note off
                midiMessages.addEvent(juce::MidiMessage::noteOff(1, _midiNotes[_midiNoteIdx], 0.0f), pos); // Note off

                //_midiLog.push_back("Note off: " + String(_sampleCounter) + "+" + String(pos) + "=" + String(_sampleCounter + pos));

                // could increment a counter here to go to next midi note array spot to change _heldNote since it just got turned off
                // YUP THAT'S TOTALLY WORKING!
                _midiNoteIdx = (_midiNoteIdx + 1) % _midiNotes.size();
            }
            else
            {
                // midiMessages.addEvent(juce::MidiMessage::noteOn(1, _heldNote, 1.0f), pos); // Note on
                midiMessages.addEvent(juce::MidiMessage::noteOn(1, _midiNotes[_midiNoteIdx], 1.0f), pos); // Note on
    
                //_midiLog.push_back("Note on: " + String(_sampleCounter) + "+" + String(pos) + "=" + String(_sampleCounter + pos));
            }
            

            _wasNoteOn = !_wasNoteOn; // Toggle note on / note off

            _nextStepInSamples += samplesPerStep; // Set our next step

        }

    }
    // WAIT I THINK IT'S STILL DOING THE START JUST AFTER THE CLICK SOMETIMES THING
    else { // to fix _wasNoteOn being true sometimes after a start/stop in the daw so that it always starts again as false
        _wasNoteOn = false;
        // keepMessages.addEvent(midiMessage, samplePos); // Let the note off go
        _midiNoteIdx = 0; // this works for resetting the start index to 0 if play stops
        midiMessages.clear();
    }
    // keepMessages.addEvent(midiMessage, samplePos); // Let the note off go

    
}

//==============================================================================
bool MidiEffectAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* MidiEffectAudioProcessor::createEditor()
{
    return new MidiEffectAudioProcessorEditor (*this);
}

//==============================================================================
void MidiEffectAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::ignoreUnused(destData);
}

void MidiEffectAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    juce::ignoreUnused(data, sizeInBytes);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new MidiEffectAudioProcessor();
}
