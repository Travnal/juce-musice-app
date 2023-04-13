/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 30 Jul 2022 1:56:16pm
    Author:  d-gra

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WaveformDisplay.h"

//==============================================================================
WaveformDisplay::WaveformDisplay(juce::AudioFormatManager& formatManager,
                                 juce::AudioThumbnailCache& thumbCache
)                                  : audioThumb(1000, formatManager, thumbCache),
                                     fileLoaded(false),position(0)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    audioThumb.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::loadURL(juce::URL audioURL)
{
    
    audioThumb.clear();
   fileLoaded =audioThumb.setSource(new juce::URLInputSource(audioURL));

   if (fileLoaded)
   {
       DBG("wtd: loadURL");
       repaint();
   }
   else
   {
       DBG("wfd not loadURL");
   }
}

void WaveformDisplay::setPositionRelative(double pos)
{  
    if (pos != position)
       {
        position = pos;
        repaint();

        }
    

}

void WaveformDisplay::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::orange);
    //when is file is load it will draw the waveform
    if (fileLoaded)
    {
        audioThumb.drawChannel(g, getLocalBounds(), 0,
            audioThumb.getTotalLength(), 0, //display the waveform
            1.5f
              );
            
        g.setColour(juce::Colours::lightgreen);//color of the rectangle on the waveform 
        g.drawRect(position * getWidth(), 0, getWidth() / 20, getHeight()); //set the postion of the width and hight
    }
    else
    {
        g.setFont(20.0f);
        g.drawText("File not loaded", getLocalBounds(),
            juce::Justification::centred, true);   // draw some placeholder text 
    }
  


}

void WaveformDisplay::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void WaveformDisplay::changeListenerCallback(juce::ChangeBroadcaster* source)
{
    DBG("wfg: change received!");
    repaint();
}
