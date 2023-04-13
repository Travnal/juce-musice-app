/*
  ==============================================================================

    PlaylistComponent.h
    Created: 30 Jul 2022 5:20:58pm
    Author:  d-gra

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include<vector>
#include <string>
#include "Song.h"
#include "DeckGUI.h"
#include "DJAudioPlayer.h" 
#include <fstream>
//==============================================================================
/*
*/
class PlaylistComponent  : public juce::Component,
                           public juce::TableListBoxModel,
                           public juce::Button::Listener,
                           public juce::TextEditor::Listener
{
public:
    PlaylistComponent(DeckGUI* _deckGUI1,
                      DeckGUI* _deckGUI2,
                      DJAudioPlayer* _MetaData);
    



    ~PlaylistComponent() override;
    // edit the input of the graphic
    void paint (juce::Graphics&) override;
    //to edit the GUI Layout
    void resized() override;
    // get the number of row 
    int getNumRows()override;
    // for the background color 
    void paintRowBackground(juce::Graphics & ,
                                 int rowNumber, 
                                 int width, 
                                 int height, 
                                bool rowIsSelected)override;
    //for the trackname and lenght column
   void paintCell(juce::Graphics& g,
                        int rowNumber,
                        int columnId,
                        int width,
                        int height,
                        bool rowIsSelected) override;

// To create the on of the column
Component* refreshComponentForCell(int rowNumber,
       int columnId,
       bool isRowSelected,
       Component* existingComponentToUpdate) override;
//when button is click get load the file
  void buttonClicked(juce::Button* button) override;

private:
   //vector and textbutton for store the track
    std::vector<Song> songs;
    juce::TextButton importrack{ "Import Audio" };
    juce::TextEditor search;
    juce::TableListBox library;
    juce::TextButton addButton1{ "Add to Top" };
    juce::TextButton addButton2{ "Add to Bottom" };

    DeckGUI* deckGUI1;
    DeckGUI* deckGUI2;

    DJAudioPlayer* MetaData;

    juce::String getDuration(juce::URL audioURL);
    juce::String secToMin(double seconds);


    //juce::TableListBox tableComponent;
    
    std::vector<int> trackDurations;
    

    void addToLibrary();
    void searchinLibrary(juce::String searchTitle);
    void savetoLibrary();
    void loadtoLibrary();
    void removeTracks(int id);
    bool playingtrack(juce::String nonextensionFileName);
    int  trackposition(juce::String searchTitle);
    void loadInDeck(DeckGUI* deckGUI);
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
