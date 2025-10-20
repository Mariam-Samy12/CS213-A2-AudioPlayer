/*
  ==============================================================================

    mute.cpp
    Created: 21 Oct 2025 1:54:53pm
    Author:  Hp

  ==============================================================================
*/
//kidi c

#include "Mute.h"

Mute::Mute()
    : isMuted(false), lastVolume(0.5f)  // default volume = 0.5
{
}

float Mute::toggleMute(float currentVolume)
{
    if (isMuted)
    {
        // Unmute → restore previous volume
        isMuted = false;
        return lastVolume;
    }
    else
    {
        // Mute → save current volume and set to 0
        isMuted = true;
        lastVolume = currentVolume;
        return 0.0f;
    }
}

bool Mute::getIsMuted() const
{
    return isMuted;
}
  
