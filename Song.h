/*
  ==============================================================================

    Song.h
    Created: 2 Aug 2022 6:15:26pm
    Author:  d-gra

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class Song
{
public:
    Song(juce::File _files);
    juce::File files;
    juce::URL URL;
    juce::String songname;
    juce::String duration;


    bool operator ==(const juce::String& other) const;
};