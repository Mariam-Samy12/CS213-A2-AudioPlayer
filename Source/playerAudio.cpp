#include "playerAudio.h"

PlayerAudio::PlayerAudio()
{
    formatManager.registerBasicFormats();
}

PlayerAudio::~PlayerAudio()
{
    releaseResources();
}

void PlayerAudio::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    const juce::ScopedLock sl(lock);
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void PlayerAudio::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    resampleSource.getNextAudioBlock(bufferToFill);

    if (isLooping && transportSource.getLengthInSeconds() > 0.0
        && transportSource.getCurrentPosition() >= transportSource.getLengthInSeconds())
    {
        transportSource.setPosition(0.0);
        transportSource.start();
    }
}

void PlayerAudio::releaseResources()
{
    const juce::ScopedLock sl(lock);
    resampleSource.releaseResources();
    transportSource.releaseResources();
}

bool PlayerAudio::loadFile(const juce::File& file)
{
    if (!file.existsAsFile())
        return false;

    auto* reader = formatManager.createReaderFor(file);
    if (reader == nullptr)
        return false;

    const juce::ScopedLock sl(lock);

    transportSource.stop();
    transportSource.setSource(nullptr);
    readerSource.reset();

    readerSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);

    transportSource.setSource(
        readerSource.get(),
        0,
        nullptr,
        reader->sampleRate
    );

    return true;


}

void PlayerAudio::start()
{
    transportSource.start();
}

void PlayerAudio::stop()
{
    transportSource.stop();
}

void PlayerAudio::setGain(float gain)
{
    const juce::ScopedLock sl(lock);
    lastGain = gain;
    if (!isMuted)
        transportSource.setGain(gain);
}

void PlayerAudio::setPosition(double pos)
{
    const juce::ScopedLock sl(lock);
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

void PlayerAudio::setSpeed(double ratio)
{
    if (ratio > 0.0)
        resampleSource.setResamplingRatio(ratio);
}

void PlayerAudio::setLooping(bool shouldLoop)
{
    isLooping = shouldLoop;
}

void PlayerAudio::setMuted(bool shouldMute)
{
    const juce::ScopedLock sl(lock);
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

void PlayerAudio::saveSession(const juce::File& file, double position)
{
    auto session = juce::File::getSpecialLocation(juce::File::userDocumentsDirectory).getChildFile("lastSession.txt");
    juce::String content = "file=" + file.getFullPathName() + "\n" + "position=" + juce::String(position);
    session.replaceWithText(content);
}

bool PlayerAudio::loadSession(juce::File& file, double& position)
{
    auto session = juce::File::getSpecialLocation(juce::File::userDocumentsDirectory).getChildFile("lastSession.txt");
    if (!session.existsAsFile())
        return false;

    juce::StringArray lines;
    session.readLines(lines);

    juce::String path;
    juce::String posStr;
    for (auto& l : lines)
    {
        if (l.startsWith("file=")) path = l.substring(5);
        else if (l.startsWith("position=")) posStr = l.substring(9);
    }

    if (path.isEmpty() || posStr.isEmpty())
        return false;

    file = juce::File(path);
    position = posStr.getDoubleValue();
    return true;
}