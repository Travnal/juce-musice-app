/*
  ==============================================================================

    DeckGUI.h
    Created: 30 Jul 2022 4:41:04pm
    Author:  d-gra

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
//==============================================================================
/*
*/
class DeckGUI  : public juce::Component,
                 public juce::Button::Listener,
                 public juce::Slider::Listener,
                 public juce::FileDragAndDropTarget,
                 public juce::Timer
{
public:
    DeckGUI(DJAudioPlayer* player,
        juce::AudioFormatManager& formatManagerTouse,
        juce::AudioThumbnailCache& cacheTouser);
    ~DeckGUI() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    // Implement buttonClicked listener
    void buttonClicked(juce::Button*) override;
    //Implement sliderValueChanged listener
    void sliderValueChanged(juce::Slider* slider) override;
    //make the file able to drag to the deck 
    bool isInterestedInFileDrag(const juce::StringArray& files) override;
    void filesDropped(const juce::StringArray& files, int x, int y) override;
    //create to change the waveform
    void timerCallback()override;
    //creating load file listener 
    void loadFile(juce::URL audioURL);

private:
    int id;
    //create button
    juce::TextButton playButton{ "PLAY" };
    juce::TextButton stopButton{ "STOP" };
    juce::TextButton loadButton{ "LOAD" };
    //create sliders
    juce::Slider volSlider;
    juce::Slider speedSlider;
    juce::Slider posSlider;
    //create label
    juce::Label volLabel;
    juce::Label   speedLabel;
    juce::Label posLabel;

    
    
    DJAudioPlayer* player;
    WaveformDisplay waveformDisplay;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
