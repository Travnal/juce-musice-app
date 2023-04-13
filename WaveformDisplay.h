/*
  ==============================================================================

    WaveformDisplay.h
    Created: 30 Jul 2022 1:56:16pm
    Author:  d-gra

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class WaveformDisplay  : public juce::Component,
                         public juce::ChangeListener
{
public:
    WaveformDisplay(
        juce::AudioFormatManager& formatManagerTouse,
        juce::AudioThumbnailCache& cacheTouser);
    ~WaveformDisplay() override;
    //creating load file listener 
    void loadURL(juce::URL audioURL);
    //set the relative postion of the audio
    void setPositionRelative(double pos);
    //edit the graphic input
    void paint (juce::Graphics&) override;
    //rezsing the componet of the application 
    void resized() override;
   void changeListenerCallback(juce::ChangeBroadcaster * source);

private:
     juce::AudioThumbnail audioThumb;
     bool fileLoaded;
     double position;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};
