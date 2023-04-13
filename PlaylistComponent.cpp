/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 30 Jul 2022 5:20:58pm
    Author:  d-gra

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeckGUI.h"
#include "PlaylistComponent.h"

//==============================================================================


PlaylistComponent::PlaylistComponent(DeckGUI* _deckGUI1, DeckGUI* _deckGUI2, DJAudioPlayer* _MetaData)
                                     :deckGUI1(_deckGUI1),
                                     deckGUI2(_deckGUI2), MetaData(_MetaData)
{
     //create table for library
    //make it visable and component it
    addAndMakeVisible(importrack);
    addAndMakeVisible(search);
    addAndMakeVisible(library);
    addAndMakeVisible(addButton1);
    addAndMakeVisible(addButton2);

    //adding listener 
    importrack.addListener(this);
    search.addListener(this);
    addButton1.addListener(this);
    addButton2.addListener(this);


    search.setTextToShowWhenEmpty("Enter your track name", juce::Colours::lightgrey);
    search.onReturnKey = [this] { searchinLibrary(search.getText()); };

    library.getHeader().addColumn("Song", 1, 1);
    library.getHeader().addColumn("Time", 2, 1);
    library.getHeader().addColumn("", 3, 3);
    library.setModel(this);
    loadtoLibrary();







}

PlaylistComponent::~PlaylistComponent()
{  
    savetoLibrary();
}

void PlaylistComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::blue);
    g.setFont (14.0f);
   // g.drawText ("PlaylistComponent123", getLocalBounds(),
               // juce::Justification::centred, true);   // draw some placeholder text
}

void PlaylistComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    // 
 

  //set the bound of the height and width of the search , importand and addbutton
    search.setBounds(0, 0, getWidth(), getHeight() / 10);
    library.setBounds(0, 1 * getHeight() / 12, getWidth(), 11 * getHeight() / 14);

    importrack.setBounds(0, 15 * getHeight() / 17.3, getWidth(), getHeight() / 17);

    addButton1.setBounds(2, 15 * getHeight() / 16.2, getWidth() / 2, getHeight() / 14);
    addButton2.setBounds(getWidth() / 2, 15 * getHeight() / 16.2, getWidth() / 2, getHeight() / 14);


       library.getHeader().setColumnWidth(1, 5* getWidth() / 10);
       library.getHeader().setColumnWidth(2, 2* getWidth() / 10);
       library.getHeader().setColumnWidth(3, 2 * getWidth() / 10);

    
}

int PlaylistComponent::getNumRows()
{
    return songs.size(); // receive Song 
}

void PlaylistComponent::paintRowBackground(juce::Graphics& g, int rowNumber, int width, int height, bool rowIsSelected)
{
    if (rowIsSelected)
    {
        g.fillAll(juce::Colours::orange);
    }
    else
    {
        g.fillAll(juce::Colours::darkgrey);
    }
}

void PlaylistComponent::paintCell(juce::Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected)
{
    if (rowNumber < getNumRows())
    {
        if (columnId == 1)//first column of track name
        {
            g.drawText(songs[rowNumber].songname,
                2,
                0,
                width +3,
                height,
                juce::Justification::centredLeft,
                true
            );
        }
        if (columnId == 2) //second column of the length of the track
        {
            g.drawText(songs[rowNumber].duration,
                2,
                0,
                width +3,
                height,
                juce::Justification::centred,
                true
            );
        }
    }
}

juce::Component* PlaylistComponent::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component* existingComponentToUpdate)
{  
   if (columnId == 3) // third column for the right of the cancel button
    {
        if (existingComponentToUpdate == nullptr)
        {
           
      

            juce::TextButton* btn = new juce::TextButton{ "Cancel" };
            juce::String id{ std::to_string(rowNumber) };
            btn->setComponentID(id);

            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
    }
     return existingComponentToUpdate;


}

void PlaylistComponent::buttonClicked(juce::Button* button)
{
    
    if (button == &importrack)//import track from to the library
    {
        DBG("import track");
        addToLibrary();
        library.updateContent();
    }
    else if (button == &addButton1)
    {
        DBG(" Add to Top"); // adding track to the Top 
        loadInDeck(deckGUI1);
    }
    else if (button == &addButton2)
    {
        DBG("Add to bottom");//adding track to the bottom
        loadInDeck(deckGUI2);
    }
    else
    {
        int id = std::stoi(button->getComponentID().toStdString());//remove track
        DBG(songs[id].songname+" Delete library ");
        removeTracks(id);
        library.updateContent();
    }
}

juce::String PlaylistComponent::getDuration(juce::URL audioURL)
{
    MetaData->loadURL(audioURL);
    double seconds{ MetaData->getLengthInSeconds() };
    juce::String min{ secToMin(seconds) };

    return min;
}

juce::String PlaylistComponent::secToMin(double seconds)
{
    //converting sec and min to string to integer
    int roundoff{ int(std::round(seconds)) };
    juce::String min{ std::to_string(roundoff /60) };
    juce::String sec{ std::to_string(roundoff % 60) };

    if (sec.length() < 3) // if he lenght less than 3 is will show the output 
    {
        //adding the length to 3
        sec = sec.paddedLeft('0', 3);
    }
    return juce::String{ min + ":" + sec }; // output of the sec and min
}


void PlaylistComponent::addToLibrary()
{
    DBG("PlaylistComponent::importToLibrary called");

    //select the file 
    juce::FileChooser chooser{ "Select audio file" };
    if (chooser.browseForMultipleFilesToOpen())
    {
        for (const juce::File& file : chooser.getResults())
        {
            juce::String fileNameWithoutExtension{ file.getFileNameWithoutExtension() };
            if (!playingtrack(fileNameWithoutExtension)) // if file not track 
            {
                Song newSong{ file };
                juce::URL audioURL{ file };
                newSong.duration = getDuration(audioURL);
                songs.push_back(newSong);
                DBG("loaded audio file: " << newSong.songname);
            }
           
            }
        }
    }


void PlaylistComponent::searchinLibrary(juce::String searchTitle)
{   //search the library by the audio title
    DBG("Searching library for: " << searchTitle);
    if (searchTitle != "")
    {
        int rowNumber = trackposition(searchTitle);
        library.selectRow(rowNumber);
    }
    else
    {
        library.deselectAllRows();
    }
}

void PlaylistComponent::savetoLibrary()
{  
    // create csv file
    std::ofstream Library("library.csv");

    // save the file
    for (Song& t : songs)
    {
        Library << t.files.getFullPathName() << "," << t.duration << "\n";
    }
}

void PlaylistComponent::loadtoLibrary()
{   //create the file
    std::ifstream Library("library.csv");
    std::string filePath;
    std::string length;

    // open the data and read it 
    if (Library.is_open())
    {
        while (getline(Library, filePath, ',')) {
            juce::File file{ filePath };
            Song newSongs{ file };

            getline(Library, length);
            newSongs.duration = length;
            songs.push_back(newSongs);
        }
    }
    Library.close();
}


void PlaylistComponent::loadInDeck(DeckGUI* deckGUI)
{  
    int selectedRow{ library.getSelectedRow() };
    if (selectedRow != -1)
    {
        DBG("Adding: " << songs[selectedRow].songname << " to Player");
         deckGUI->loadFile(songs[selectedRow].URL);//adding the track to the deckGUI
    }
    else
    {   //display message when is not added track
        juce::AlertWindow::showMessageBox(juce::AlertWindow::AlertIconType::InfoIcon,
            "Deck info",
            "Adding new track to the deck?",
            "yes?",
            nullptr
        );
    }
}


void PlaylistComponent::removeTracks(int id)
{   //remove the track
    songs.erase(songs.begin() + id);
}

bool PlaylistComponent::playingtrack(juce::String nonextensionFileName)
{
    return (std::find(songs.begin(), songs.end(), nonextensionFileName) != songs.end());// if Song have something it will begin it nothing it will end
}

int PlaylistComponent::trackposition(juce::String searchTitle)
{     // find the audio name 
    auto it = find_if(songs.begin(), songs.end(),
        [&searchTitle](const Song& obj) {return obj.songname.contains(searchTitle); });
    int i = -1;

    if (it != songs.end())// if no it will end
    {
        i = std::distance(songs .begin(), it);
    }

    return i;
}
