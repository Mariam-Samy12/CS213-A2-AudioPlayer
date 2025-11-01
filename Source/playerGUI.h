#pragma once
#include <JuceHeader.h>
#include "PlayerAudio.h"

class PlayerGUI :
    public juce::Component,
    public juce::Button::Listener,
    public juce::Slider::Listener,
    public juce::Timer//slider

{
public:
    PlayerGUI();
    ~PlayerGUI() override;

    void resized() override;
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
    void releaseResources();
    void paint(juce::Graphics& g) override;
    void timerCallback() override;//slider

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
    juce::Label timeLabel;//slider

    juce::Slider volumeSlider;

    std::unique_ptr<juce::FileChooser> fileChooser;

    // Event handlers
    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;

    //  For mute state
    bool isMuted = false;
    double lastVolume = 0.5;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI)
};