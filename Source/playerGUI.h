#pragma once
#include <JuceHeader.h>
#include "PlayerAudio.h"

class PlayerGUI : public juce::Component,
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

    //  Buttons
    juce::TextButton loadButton{ "Load File" };
    juce::TextButton restartButton{ "Restart" };
    juce::TextButton stopButton{ "Stop" };
    juce::TextButton playPauseButton{ "Play" };
    juce::TextButton goStartButton{ "|<" };
    juce::TextButton goEndButton{ ">|" };
    juce::TextButton loopButton{ "Loop: Off" };
    juce::TextButton muteButton{ "Mute" };
    juce::TextButton forwardButton{ "+10s >>" };
    juce::TextButton backwardButton{ "<< -10s" };
    juce::Slider speedSlider;
    juce::Slider volumeSlider;
    juce::Label infoLabel;
    juce::Slider positionSlider; //slider
    juce::Label timeLabel;       //slider

    juce::TextButton setAButton{ "Set A" };      //AB
    juce::TextButton setBButton{ "Set B" };      //AB
    juce::TextButton clearABButton{ "Clear A-B" }; //AB
    juce::TextButton addMarkerButton{ "Add Marker" };// Track Markers
    juce::ComboBox markerList;// Track Markers
    juce::TextButton playFromMiddleButton{ " From Middle " };//Focous mood
    // nine
    juce::Slider progressSlider;
    juce::Label currentTimeLabel, totalTimeLabel;


    //play list
    std::vector<juce::File> playlist;
    juce::ComboBox playlistBox;
    juce::TextButton addToPlaylistButton{ "Add to Playlist" };
    juce::TextButton playSelectedButton{ "Play Selected" };
    juce::TextButton clearPlaylistButton{ "Clear Playlist" };
    juce::TextButton removeSelectedButton{ "Remove Selected" };
    bool isMuted = false;
    double lastVolume = 0.5;

    std::unique_ptr<juce::FileChooser> fileChooser;

    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;
    //AB
    double loopPointA = -1.0;
    double loopPointB = -1.0;
    //waves

    juce::AudioThumbnailCache thumbnailCache{ 5 };
    juce::AudioThumbnail thumbnail{ 512, formatManager, thumbnailCache };
    juce::AudioFormatManager formatManager;
    bool waveformLoaded = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI)
};
