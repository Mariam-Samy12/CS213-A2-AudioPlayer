#include "PlayerAudio.h"
#include <taglib/fileref.h>
#include <taglib/tag.h>

PlayerAudio::PlayerAudio()
{
    formatManager.registerBasicFormats();
}

PlayerAudio::~PlayerAudio() {}

void PlayerAudio::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}


void PlayerAudio::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    resampleSource.getNextAudioBlock(bufferToFill);

    //  LOOP 
    if (isLooping)
    {
        double currentPos = transportSource.getCurrentPosition();
        double totalLength = transportSource.getLengthInSeconds();

        if (currentPos >= totalLength - 0.01)
        {
            transportSource.setPosition(0.0);
        }
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
            transportSource.setSource(nullptr);
            readerSource.reset();
            readerSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);

            transportSource.setSource(readerSource.get(), 0, nullptr, reader->sampleRate);

            loadMetadata(file);


        }
    }
    return true;
}

void PlayerAudio::start() { transportSource.start(); }
void PlayerAudio::stop() { transportSource.stop(); }

void PlayerAudio::setSpeed(double ratio)
{
    if (ratio > 0.0)
        resampleSource.setResamplingRatio(ratio);
}
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

//  Load Metadata
void PlayerAudio::loadMetadata(const juce::File& file)
{
    TagLib::FileRef f(file.getFullPathName().toRawUTF8());
    if (!f.isNull() && f.tag())
    {
        TagLib::Tag* tag = f.tag();
        title = juce::String::fromUTF8(tag->title().toCString(true));
        artist = juce::String::fromUTF8(tag->artist().toCString(true));
        album = juce::String::fromUTF8(tag->album().toCString(true));
        year = tag->year();
    }
    else
    {
        title = file.getFileNameWithoutExtension();
        artist = "Unknown Artist";
        album = "Unknown Album";
        year = 0;
    }

    if (f.audioProperties())
    {
        auto* props = f.audioProperties();
        duration = props->length();
    }
}
void PlayerAudio::saveSession(const juce::File& file, double position)
{
    juce::File sessionFile = juce::File::getSpecialLocation(juce::File::userDocumentsDirectory)
        .getChildFile("lastSession.txt");

    juce::String content = "file=" + file.getFullPathName() + "\n" +
        "position=" + juce::String(position);

    sessionFile.replaceWithText(content);
}

bool PlayerAudio::loadSession(juce::File& file, double& position)
{
    juce::File sessionFile = juce::File::getSpecialLocation(juce::File::userDocumentsDirectory)
        .getChildFile("lastSession.txt");

    if (!sessionFile.existsAsFile())
        return false;

    juce::StringArray lines;
    sessionFile.readLines(lines);

    juce::String filePath;
    juce::String posStr;

    for (auto& line : lines)
    {
        if (line.startsWith("file="))
            filePath = line.substring(5);
        else if (line.startsWith("position="))
            posStr = line.substring(9);
    }

    if (filePath.isEmpty() || posStr.isEmpty())
        return false;

    file = juce::File(filePath);
    position = posStr.getDoubleValue();

    return true;
}