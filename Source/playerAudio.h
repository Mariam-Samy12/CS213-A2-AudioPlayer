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

    void setLooping(bool shouldLoop);
    bool getLooping() const { return isLooping; }

    void setMuted(bool shouldMute);
    bool getMuted() const { return isMuted; }

    
    juce::String getTitle() const { return title; }
    juce::String getArtist() const { return artist; }
    juce::String getAlbum() const { return album; }
    int getYear() const { return year; }
    double getDuration() const { return duration; }

private:
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;

    bool isLooping = false;
    bool isMuted = false;
    float lastGain = 0.5f;

    juce::String title, artist, album;
    int year = 0;
    double duration = 0.0;

    void loadMetadata(const juce::File& file);
};
