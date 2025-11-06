#include "MainComponent.h"

MainComponent::MainComponent()
{
    addAndMakeVisible(player1);
    addAndMakeVisible(player2);

    //  mixer 
    addAndMakeVisible(mixerTitle);
    mixerTitle.setText("MIXER", juce::dontSendNotification);
    mixerTitle.setFont(juce::Font(18.0f, juce::Font::bold));
    mixerTitle.setColour(juce::Label::textColourId, juce::Colours::white);
    mixerTitle.setJustificationType(juce::Justification::centred);

    // Player 1 mixer control
    addAndMakeVisible(player1VolumeSlider);
    player1VolumeSlider.setRange(0.0, 1.0, 0.01);
    player1VolumeSlider.setValue(1.0);
    player1VolumeSlider.setSliderStyle(juce::Slider::LinearVertical);
    player1VolumeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    player1VolumeSlider.addListener(this);

    addAndMakeVisible(player1Label);
    player1Label.setText("Player 1", juce::dontSendNotification);
    player1Label.setFont(juce::Font(14.0f));
    player1Label.setColour(juce::Label::textColourId, juce::Colours::white);
    player1Label.setJustificationType(juce::Justification::centred);

    //  Player 2 mixer control
    addAndMakeVisible(player2VolumeSlider);
    player2VolumeSlider.setRange(0.0, 1.0, 0.01);
    player2VolumeSlider.setValue(1.0);
    player2VolumeSlider.setSliderStyle(juce::Slider::LinearVertical);
    player2VolumeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);
    player2VolumeSlider.addListener(this);

    addAndMakeVisible(player2Label);
    player2Label.setText("Player 2", juce::dontSendNotification);
    player2Label.setFont(juce::Font(14.0f));
    player2Label.setColour(juce::Label::textColourId, juce::Colours::white);
    player2Label.setJustificationType(juce::Justification::centred);

    setSize(900, 600);
    setAudioChannels(0, 2);
}

MainComponent::~MainComponent()
{
    shutdownAudio();
}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);

    // Mixer
    mixBuffer1.setSize(2, samplesPerBlockExpected);
    mixBuffer2.setSize(2, samplesPerBlockExpected);
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    // Mixer
    bufferToFill.clearActiveBufferRegion();

    // Mixer
    mixBuffer1.clear();
    juce::AudioSourceChannelInfo buffer1Info(&mixBuffer1, 0, bufferToFill.numSamples);
    player1.getNextAudioBlock(buffer1Info);

    // Mixer
    mixBuffer2.clear();
    juce::AudioSourceChannelInfo buffer2Info(&mixBuffer2, 0, bufferToFill.numSamples);
    player2.getNextAudioBlock(buffer2Info);

    // Mixer
    float gain1 = (float)player1VolumeSlider.getValue();
    float gain2 = (float)player2VolumeSlider.getValue();

    // Mixer
    for (int channel = 0; channel < bufferToFill.buffer->getNumChannels(); ++channel)
    {
        bufferToFill.buffer->addFrom(channel, bufferToFill.startSample,
            mixBuffer1, channel, 0, bufferToFill.numSamples, gain1);

        bufferToFill.buffer->addFrom(channel, bufferToFill.startSample,
            mixBuffer2, channel, 0, bufferToFill.numSamples, gain2);
    }
}

void MainComponent::releaseResources()
{
    player1.releaseResources();
    player2.releaseResources();
}

void MainComponent::resized()
{
    auto area = getLocalBounds();

    //  mixer controls on the right
    auto mixerArea = area.removeFromRight(120);

    // Mixer
    mixerTitle.setBounds(mixerArea.removeFromTop(40));

    // Player 1 mixer 
    player1Label.setBounds(mixerArea.removeFromTop(25).reduced(5));
    player1VolumeSlider.setBounds(mixerArea.removeFromTop(220).reduced(10));

    // Player 2 mixer control
    player2Label.setBounds(mixerArea.removeFromTop(25).reduced(5));
    player2VolumeSlider.setBounds(mixerArea.removeFromTop(220).reduced(10));

    //  // Mixer
    auto top = area.removeFromTop(area.getHeight() / 2);
    player1.setBounds(top);
    player2.setBounds(area);
}

void MainComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgrey);

   
    auto mixerArea = getLocalBounds().removeFromRight(120);
    g.setColour(juce::Colour(50, 50, 50));
    g.fillRect(mixerArea);

   
    g.setColour(juce::Colours::black);
    g.drawRect(mixerArea, 2);
}

void MainComponent::sliderValueChanged(juce::Slider* slider)
{
    
}
