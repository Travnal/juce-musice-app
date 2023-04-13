/*
  ==============================================================================

    Song.cpp
    Created: 2 Aug 2022 6:15:27pm
    Author:  d-gra

  ==============================================================================
*/

#include "Song.h"

Song::Song(juce::File _files) :files(_files), songname(_files.getFileNameWithoutExtension()),
URL(juce::URL{ _files })
{
    DBG(" name is create" << songname);
}


bool Song::operator==(const juce::String& other) const
{
    return false;
}
