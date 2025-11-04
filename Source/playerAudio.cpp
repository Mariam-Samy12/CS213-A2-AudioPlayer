#include "PlayerAudio.h"

PlayerAudio::PlayerAudio()
{
    formatManager.registerBasicFormats();
}

PlayerAudio::~PlayerAudio() {}

void PlayerAudio::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void PlayerAudio::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    transportSource.getNextAudioBlock(bufferToFill);

    // ✅ LOOP handling
    if (isLooping && !transportSource.isPlaying() && transportSource.getCurrentPosition() >= getLength())
    {
        transportSource.setPosition(0.0);
        transportSource.start();
    }
}

void PlayerAudio::releaseResources()
{
    transportSource.releaseResources();
}

bool PlayerAudio::loadFile(const juce::File& file)
{
    if (file.existsAsFile())
    {
        if (auto* reader = formatManager.createReaderFor(file))
        {
            transportSource.stop();

            readerSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);

            transportSource.setSource(readerSource.get(), 0, nullptr, reader->sampleRate);

            loadMetadata(file);

            transportSource.start();
            return true;
        }
    }
    return false;
}

void PlayerAudio::start() { transportSource.start(); }
void PlayerAudio::stop() { transportSource.stop(); }

void PlayerAudio::setGain(float gain)
{
    if (!isMuted)
    {
        transportSource.setGain(gain);
        lastGain = gain;
    }
}

void PlayerAudio::setPosition(double pos)
{
    transportSource.setPosition(pos);
}

double PlayerAudio::getPosition() const
{
    return transportSource.getCurrentPosition();
}

double PlayerAudio::getLength() const
{
    return transportSource.getLengthInSeconds();
}

void PlayerAudio::setLooping(bool shouldLoop)
{
    isLooping = shouldLoop;
}

void PlayerAudio::setMuted(bool shouldMute)
{
    if (shouldMute && !isMuted)
    {
        lastGain = transportSource.getGain();
        transportSource.setGain(0.0f);
    }
    else if (!shouldMute && isMuted)
    {
        transportSource.setGain(lastGain);
    }
    isMuted = shouldMute;
}

// 🟢 Load Metadata using JUCE only
void PlayerAudio::loadMetadata(const juce::File& file)
{
    // Store filename
    fileName = file.getFileNameWithoutExtension();

    // Get duration from transport source
    duration = getLength();

    // Try to read metadata from file using JUCE
    if (auto* reader = formatManager.createReaderFor(file))
    {
        // Get metadata from reader
        auto metadata = reader->metadataValues;

        // Extract common metadata keys
        title = metadata.getValue("title", fileName);
        artist = metadata.getValue("artist", "Unknown Artist");
        album = metadata.getValue("album", "Unknown Album");

        // If title is empty, use filename
        if (title.isEmpty())
            title = fileName;

        delete reader;
    }
    else
    {
        // Fallback if no metadata found
        title = fileName;
        artist = "Unknown Artist";
        album = "Unknown Album";
    }
}



