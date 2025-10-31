#include "playerGUI.h"

void PlayerGUI::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    playerAudio.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void PlayerGUI::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    playerAudio.getNextAudioBlock(bufferToFill);
}

void PlayerGUI::releaseResources()
{
    playerAudio.releaseResources();
}

void PlayerGUI::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgrey);
}

PlayerGUI::PlayerGUI()
{
    // Add buttons
    for (auto* btn : { &loadButton, &restartButton, &stopButton, &playPauseButton, &goStartButton, &goEndButton, &loopButton, &muteButton })
    {
        btn->addListener(this);
        addAndMakeVisible(btn);
    }

    // Volume slider
    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setValue(0.5);
    volumeSlider.addListener(this);
    addAndMakeVisible(volumeSlider);
}

void PlayerGUI::resized()
{
    int y = 20;
    loadButton.setBounds(20, y, 100, 40);
    restartButton.setBounds(140, y, 80, 40);
    stopButton.setBounds(240, y, 80, 40);
    loopButton.setBounds(340, y, 100, 40);
    muteButton.setBounds(460, y, 80, 40); // 🔇 Mute button position

    playPauseButton.setBounds(20, 70, 80, 30);
    goStartButton.setBounds(120, 70, 80, 30);
    goEndButton.setBounds(220, 70, 80, 30);
    infoLabel.setBounds(20, 160, getWidth() - 40, 60);//5

    volumeSlider.setBounds(20, 110, getWidth() - 40, 30);
}

PlayerGUI::~PlayerGUI()
{
}

void PlayerGUI::buttonClicked(juce::Button* button)
{
    if (button == &loadButton)
    {
        fileChooser = std::make_unique<juce::FileChooser>(
            "Select an audio file...",
            juce::File{},
            "*.wav;*.mp3;*.flac;*.aiff;*.aif;*.ogg;*.aac;*.mpeg"
        );


        fileChooser->launchAsync(
            juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
            [this](const juce::FileChooser& fc)
            {
                auto file = fc.getResult();
                if (file.existsAsFile())
                    playerAudio.loadFile(file);
            });

    }

    if (button == &restartButton)
    {
        playerAudio.start();
    }

    if (button == &stopButton)
    {
        playerAudio.stop();
        playerAudio.setPosition(0.0);
    }

    if (button == &playPauseButton)
    {
        static bool isPlaying = false;
        if (isPlaying)
        {
            playerAudio.stop();
            playPauseButton.setButtonText("Play");
        }
        else
        {
            playerAudio.start();
            playPauseButton.setButtonText("Pause");
        }
        isPlaying = !isPlaying;
    }

    if (button == &goStartButton)
    {
        playerAudio.setPosition(0.0);
        playerAudio.start();
    }

    if (button == &goEndButton)
    {
        playerAudio.setPosition(playerAudio.getLength());
        playerAudio.stop();
    }

    if (button == &loopButton)
    {
        bool newState = !playerAudio.getLooping();
        playerAudio.setLooping(newState);
        loopButton.setButtonText(newState ? "Loop: On" : "Loop: Off");
    }

    // 🔇 Mute button
    if (button == &muteButton)
    {
        if (!isMuted)
        {

            lastVolume = volumeSlider.getValue();


            playerAudio.setGain(0.0f);


            muteButton.setButtonText("Unmute");
        }
        else
        {

            playerAudio.setGain((float)lastVolume);
            volumeSlider.setValue(lastVolume);
            muteButton.setButtonText("Mute");
        }


        isMuted = !isMuted;
    }

}

void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider)
    {
        if (!isMuted)
            playerAudio.setGain((float)slider->getValue());
        else
            lastVolume = slider->getValue(); // update stored volume while muted
    }
}