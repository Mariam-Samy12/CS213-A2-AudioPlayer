#include "playerAudio.h"
//5
#include <taglib/fileref.h>
#include <taglib/tag.h>
//

PlayerAudio::PlayerAudio()
{
    formatManager.registerBasicFormats();
}

PlayerAudio::~PlayerAudio()
{
}

void PlayerAudio::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void PlayerAudio::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    transportSource.getNextAudioBlock(bufferToFill);

    // LOOP handling
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
            // Disconnect old source
            transportSource.stop();
            transportSource.setSource(nullptr);
            readerSource.reset();

            // Create new reader source
            readerSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);

            // Attach safely
            transportSource.setSource(readerSource.get(), 0, nullptr, reader->sampleRate);
            transportSource.start();
            //5
            TagLib::FileRef f(file.getFullPathName().toRawUTF8());
            if (!f.isNull() && f.tag())
            {
                TagLib::Tag* tag = f.tag();
                title = juce::String::fromUTF8(tag->title().toCString(true));
                artist = juce::String::fromUTF8(tag->artist().toCString(true));
                album = juce::String::fromUTF8(tag->album().toCString(true));
            }
            else
            {
                title = file.getFileNameWithoutExtension();
                artist = "Unknown Artist";
                album = "Unknown Album";
            }

            duration = reader->lengthInSamples / reader->sampleRate;
            return true;
;
            //

        }
    }
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

// 🔇 Mute/Unmute
void PlayerAudio::setMuted(bool shouldMute)
{
    if (shouldMute && !isMuted)
    {
        lastGain = transportSource.getGain();
        
    }
    else if (!shouldMute && isMuted)
    {
        transportSource.setGain(lastGain);
        transportSource.setGain(0.0f);
    }

    isMuted = shouldMute;
}
