#include "MainComponent.h"

MainComponent::MainComponent()
{
    addAndMakeVisible(player1);
    addAndMakeVisible(player2);

    setAudioChannels(0, 2); // no input, stereo output
}

MainComponent::~MainComponent()
{
    shutdownAudio();
}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    juce::AudioBuffer<float> temp1(2, bufferToFill.numSamples);
    juce::AudioBuffer<float> temp2(2, bufferToFill.numSamples);

    temp1.clear();
    temp2.clear();

    juce::AudioSourceChannelInfo info1(&temp1, 0, bufferToFill.numSamples);
    juce::AudioSourceChannelInfo info2(&temp2, 0, bufferToFill.numSamples);

    player1.getNextAudioBlock(info1);
    player2.getNextAudioBlock(info2);

    for (int c = 0; c < bufferToFill.buffer->getNumChannels(); c++)
    {
        bufferToFill.buffer->addFrom(c, bufferToFill.startSample, temp1, c, 0, bufferToFill.numSamples);
        bufferToFill.buffer->addFrom(c, bufferToFill.startSample, temp2, c, 0, bufferToFill.numSamples);
    }
}

void MainComponent::releaseResources()
{
    player1.releaseResources();
    player2.releaseResources();
}

void MainComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgrey);
}

void MainComponent::resized()
{
    int halfWidth = getWidth() / 2;

    player1.setBounds(0, 0, halfWidth, getHeight());
    player2.setBounds(halfWidth, 0, halfWidth, getHeight());
}