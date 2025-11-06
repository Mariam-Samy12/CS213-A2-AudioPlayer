#pragma once
#include <JuceHeader.h>
#include "PlayerGUI.h"

class MainComponent : public juce::AudioAppComponent,
    public juce::Slider::Listener
{
public:
    MainComponent();
    ~MainComponent() override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    void resized() override;
    void paint(juce::Graphics& g) override;

    void sliderValueChanged(juce::Slider* slider) override;

private:
    PlayerGUI player1;
    PlayerGUI player2;

    // Mixer
    juce::AudioBuffer<float> mixBuffer1;
    juce::AudioBuffer<float> mixBuffer2;

    // Mixer 
    juce::Slider player1VolumeSlider;
    juce::Slider player2VolumeSlider;
    juce::Label player1Label, player2Label;
    juce::Label mixerTitle;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
