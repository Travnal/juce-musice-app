/*
  ==============================================================================

    DJAudioPlayer.cpp
    Created: 30 Jul 2022 2:11:46pm
    Author:  d-gra

  ==============================================================================
*/

#include "DJAudioPlayer.h"


DJAudioPlayer::DJAudioPlayer(juce::AudioFormatManager& _formatManager):formatManager(_formatManager)
{

}

DJAudioPlayer::~DJAudioPlayer()
{

}
//==============================================================================
void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    //get the data from the transportsource to the audio system
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    //get the data from the resamplesource to the audio system
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}
void DJAudioPlayer::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{   //get the audio data from resmaplesouce
    resampleSource.getNextAudioBlock(bufferToFill);
}
void DJAudioPlayer::releaseResources() 
{  //put the steam back to the unprepared state
    transportSource.releaseResources();
    resampleSource.releaseResources();
}


void DJAudioPlayer::loadURL(juce::URL audioURL)
{
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));//audioURL.CreateInputSteam is to unpack the audioURL can converted into inputSteam.
    if (reader != nullptr) 
    {
        std::unique_ptr<juce::AudioFormatReaderSource> newSource(new juce::AudioFormatReaderSource(reader, true));//After that passes to the AudioFormatManager and is created as a reader 
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource.reset(newSource.release()); //if anything goes wrong it will exit out of that function and it will clear up the memory, 
        transportSource.start();  //if goes correct it will passes the right to the class scope variable and lastly, it will we call trasnportsource.start()
             
    }
}

void DJAudioPlayer::setGain(double gain)
{  
    if (gain < 0 || gain > 1.0)
    {
        DBG("DJAudioPlayer::setGain gain should be between 0 and 1");
    }
    else {
        transportSource.setGain(gain);
    }
 
}
void DJAudioPlayer::setSpeed(double ratio)
{
    if (ratio < 0 || ratio > 100.0)
    {
        DBG("DJAudioPlayer::setspeemd  be between 1 and 100");
    }
    else {
        resampleSource.setResamplingRatio(ratio);
    }

}
void DJAudioPlayer::setPosition(double posInScs)
{
    transportSource.setPosition(posInScs);
}

void DJAudioPlayer::setPositionRelative(double pos)
{
    if (pos < 0 || pos > 100.0)
    {
        DBG("DJAudioPlayer::setPostivitonRelative pos shoudl be bewteen0 and ");
    }
    else {
        double posInScs = transportSource.getLengthInSeconds() * pos;
        setPosition(posInScs);
    }
}

void DJAudioPlayer::start()
{
    transportSource.start();
}
void DJAudioPlayer::stop() 
{
    transportSource.stop();
}

double DJAudioPlayer::getPositionRelative()
{
    return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();

}

double DJAudioPlayer::getLengthInSeconds()
{
    return transportSource.getLengthInSeconds();
}
