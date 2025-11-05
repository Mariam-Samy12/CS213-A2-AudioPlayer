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
    void start();
    void stop();

    void setGain(float gain);
    void setPosition(double pos);
    double getPosition() const;
    double getLength() const;

    void setSpeed(double ratio);
    void setLooping(bool shouldLoop);
    bool getLooping() const { return isLooping; }

    void setMuted(bool shouldMute);
    bool getMuted() const { return isMuted; }

    void saveSession(const juce::File& file, double position);
    bool loadSession(juce::File& file, double& position);

private:
    juce::CriticalSection lock;
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    juce::ResamplingAudioSource resampleSource{ &transportSource, false, 2 };

    bool isLooping = false;
    bool isMuted = false;
    float lastGain = 0.5f;
};