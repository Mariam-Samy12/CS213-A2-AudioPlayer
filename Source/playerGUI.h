#pragma once
#include <JuceHeader.h>
#include "PlayerAudio.h"


class PlayerGUI :
    public juce::Component,
    public juce::Button::Listener,
    public juce::Slider::Listener,
    public juce::Timer //slider
{
public:
    PlayerGUI();
    ~PlayerGUI() override;

    void resized() override;
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
    void releaseResources();
    void paint(juce::Graphics& g) override;

    void timerCallback() override; //slider
    // Track Markers
    struct Marker {
        double timeInSeconds;
        juce::String label;
    };

    std::vector<Marker> markers;
    int markerCount = 0;
private:
    PlayerAudio playerAudio;

    // GUI elements

    juce::TextButton loadButton{ "Load File" };
    juce::TextButton restartButton{ "Restart" };
    juce::TextButton stopButton{ "Stop" };
    juce::TextButton playPauseButton{ "Play" };
    juce::TextButton goStartButton{ "|<" };
    juce::TextButton goEndButton{ ">|" };
    juce::TextButton loopButton{ "Loop: Off" };

    juce::TextButton muteButton{ "Mute" }; //  New Mute button
juce::Slider positionSlider; //slider
juce::Label timeLabel;       //slider

juce::TextButton setAButton{ "Set A" };      //AB
juce::TextButton setBButton{ "Set B" };      //AB
juce::TextButton clearABButton{ "Clear A-B" }; //AB
juce::Slider volumeSlider;
juce::TextButton addMarkerButton{ "Add Marker" };// Track Markers
juce::ComboBox markerList;// Track Markers
juce::TextButton playFromMiddleButton{ " From Middle " };//Focous mood
  juce::TextButton forwardButton{ "+10s >>" };
    juce::TextButton backwardButton{ "<< -10s" };
    std::unique_ptr<juce::FileChooser> fileChooser;
    //play list
    std::vector<juce::File> playlist;               
    juce::ComboBox playlistBox;                     
    juce::TextButton addToPlaylistButton{ "Add to Playlist" };  
    juce::TextButton playSelectedButton{ "Play Selected" };    
    juce::TextButton clearPlaylistButton{ "Clear Playlist" };
    juce::TextButton removeSelectedButton{ "Remove Selected" };
        // Event handlers
    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;

    //  For mute state
    bool isMuted = false;
    double lastVolume = 0.5;
 juce::Label infoLabel;

//AB
double loopPointA = -1.0;
double loopPointB = -1.0;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI)//
};

