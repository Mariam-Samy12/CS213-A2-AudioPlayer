
#pragma once

class Mute
{
public:
    Mute();

    // Toggle mute/unmute and return the volume to set
    float toggleMute(float currentVolume);

    // Check mute state
    bool getIsMuted() const;

private:
    bool isMuted;
    float lastVolume;
};
