/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class MidiEffectAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    MidiEffectAudioProcessor();
    ~MidiEffectAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif
    void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    std::vector<int> _midiNotes{60, 62, 64, 66, 68, 70, 72}; // setting default scale, but takes input from user

private:
    // adding from https://forum.juce.com/t/midi-timing-notes-arpeggiator/28729
    int _heldNote = 60; // just setting it to C, only using this for logic though setting the actual midi notes from _midiNotes array

    // I guess try to go through sending an array of notes 1 by 1 as the 1st experiment...
    // int _midiNotes[7] = {60, 62, 64, 66, 68, 70, 72};
    int _midiNoteIdx = 0;

    int _sampleCounter;
    int _nextStepInSamples; 
    int _sampleRate = 44100; // what Ableton shows my sample rate is 
    bool _wasNoteOn = false; // to toggle between playing a note and turning it off

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiEffectAudioProcessor)
};
