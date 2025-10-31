#pragma once
#include <JuceHeader.h>

class PlayerAudio
{
public:
    PlayerAudio();
    ~PlayerAudio();

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
    void releaseResources();

    bool loadFile(const juce::File& file);
    void stop();
    void start();

    void setGain(float gain);
    void setPosition(double pos);
    double getPosition() const;
    double getLength() const;

    void setLooping(bool shouldLoop); // LOOP
    bool getLooping() const { return isLooping; } // LOOP

    // ?? Mute controls
    void setMuted(bool shouldMute);
    bool getMuted() const { return isMuted; }

private:
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;

    bool isLooping = false;  // LOOP
    bool isMuted = false;    // ??
    float lastGain = 0.5f;   // ?? store previous volume when muted
};