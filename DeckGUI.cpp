/*
  ==============================================================================

    DeckGUI.cpp
    Created: 30 Jul 2022 4:41:04pm
    Author:  d-gra

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player,
    juce::AudioFormatManager& formatManagerTouse,
    juce::AudioThumbnailCache& cacheTouse) : player(_player),
    waveformDisplay(formatManagerTouse, cacheTouse)
{  
    //make it visable and component it
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);
    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);
    addAndMakeVisible(waveformDisplay);
    addAndMakeVisible(volLabel);
   
    //adding listener 
    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);

    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);


  


   
    //set the timer every 0.3 second
    startTimer(300);

    //set the text label and create slider textBox
    volSlider.setRange(0.1, 100.0);
    volSlider.setNumDecimalPlacesToDisplay(3);
    volSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, false, 50, 25);
    volLabel.setText("Volume", juce::dontSendNotification);
    volLabel.attachToComponent(&volSlider, false);

    speedSlider.setRange(0.1, 100.0);
    speedSlider.setNumDecimalPlacesToDisplay(3);
    speedSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    speedSlider.setTextBoxStyle(juce::Slider::TextBoxAbove, false, 50, 25);
    speedLabel.setText("Speed", juce::dontSendNotification);
    speedLabel.attachToComponent(&speedSlider, false);


    posSlider.setRange(0.1, 1.0);
    posSlider.setNumDecimalPlacesToDisplay(3);
    posSlider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    posSlider.setTextBoxStyle(juce::Slider::TextBoxAbove , true, 50, 25);
    posLabel.setText("Position", juce::dontSendNotification);
    posLabel.attachToComponent(&posSlider, false);
    

}



DeckGUI::~DeckGUI()
{  //allow the time to stop
    stopTimer();
}

void DeckGUI::paint (juce::Graphics& g)
{
    

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw the component of the outline
 
   
}

void DeckGUI::resized()
{
   
    //the set the bounce of the componet and the area of width and height
    double rowH = getHeight() / 8;
    double col = getWidth() / 3;
    playButton.setBounds(0, 0, col, rowH *1.2);
    stopButton.setBounds(col, 0, col, rowH *1.2);
    loadButton.setBounds(col * 2, 0, col, rowH *1.2);

    waveformDisplay.setBounds(0, rowH , getWidth(), rowH*1.8);
    volSlider.setBounds(0, rowH * 5, col, rowH * 3);
    speedSlider.setBounds(col, rowH * 5, col, rowH * 3);
    posSlider.setBounds(col * 2, rowH * 5, col, rowH * 3);




}

void DeckGUI::buttonClicked(juce::Button* button)
{ 
    if (button == &playButton)//press the play button to start
    {
        std::cout << "Play button was clicked " << std::endl;
        player->start();
    }
    if (button == &stopButton)//press the stop button to stop
    {
        std::cout << "Stop button was clicked " << std::endl;
        player->stop();

    }
    if (button == &loadButton)//press the load button to upload audio
    {
        juce::FileChooser chooser{ "Select a track audio...." };
        if (chooser.browseForFileToOpen())
        {
            player->loadURL(juce::URL{ chooser.getResult() });
            waveformDisplay.loadURL(juce::URL{ chooser.getResult() });
        }
    }
}

void DeckGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volSlider)//set the volume slider vol
    {
        player->setGain(slider->getValue());
    }

    if (slider == &speedSlider)//set the speedslider speed
    {
        player->setSpeed(slider->getValue());
    }

    if (slider == &posSlider)//set the poslider position
    {
        player->setPositionRelative(slider->getValue());
    }

}

bool DeckGUI::isInterestedInFileDrag(const juce::StringArray& files)
{ 
    DBG("DeckGUI::isInterestedInFileDrag");//able to let the file drag and drop
    return true;
}

void DeckGUI::filesDropped(const juce::StringArray& files, int x, int y)
{
    DBG("DeckGUI::filesDropped");// if true the file will drop 
    if (files.size() == 1)
    {
        player->loadURL(juce::URL{ juce::File{files[0]} });
    }

}

void DeckGUI::timerCallback()
{
    //set and get the relative posion 
    DBG("DeckGUI::timerCallBack()");
    waveformDisplay.setPositionRelative(
           player->getPositionRelative());

}

void DeckGUI::loadFile(juce::URL audioURL)
{  //load the file audio
    DBG("DeckGUI::loadFile called");
    player->loadURL(audioURL);
    waveformDisplay.loadURL(audioURL);
}




