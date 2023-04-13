/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 30 Jul 2022 2:11:46pm
    Author:  d-gra

  ==============================================================================
*/

#pragma once

#include"../JuceLibraryCode/JuceHeader.h"

class DJAudioPlayer : public juce::AudioSource 
{
public:
    DJAudioPlayer(juce::AudioFormatManager& _formatManager);

    ~DJAudioPlayer();
    //==============================================================================
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //load the audio files
    void loadURL(juce::URL audioURL);
    //set the audio volume
    void setGain(double gain);
    // set the audio speed
    void setSpeed(double ratio);
     //set the audio postion
    void setPosition(double posInScs);
    //set the relative postion of the audio
    void setPositionRelative(double pos);

    //start the audio file
    void start();
    //stop the audio file
    void stop();

    /** get the relative position of the playhead */
    double getPositionRelative();
    //get the length of in seconds 
    double getLengthInSeconds();

    private:
        juce::AudioFormatManager& formatManager;
        std::unique_ptr<juce::AudioFormatReaderSource>  readerSource;
        juce::AudioTransportSource transportSource;

        juce::ResamplingAudioSource resampleSource{ &transportSource , false,2 };
};