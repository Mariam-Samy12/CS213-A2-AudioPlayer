#include "PlayerGUI.h"

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
    g.fillAll(juce::Colour(25, 25, 25)); // 🌑 Dark background
}

PlayerGUI::PlayerGUI(){
    // 🖼️ تحميل الصور من ملفات خارجية
    juce::File playFile("C:/Users/Yara/Downloads/play.png");
    juce::File pauseFile("C:/Users/Yara/Downloads/pause.png");

    playIcon = juce::ImageFileFormat::loadFrom(playFile);
    pauseIcon = juce::ImageFileFormat::loadFrom(pauseFile);

    // 🎮 إعداد زرار Play بصورة
    if (playIcon.isValid())
        playPauseButton.setImages(true, true, true, playIcon, 1.0f, {}, playIcon, 1.0f, {}, playIcon, 1.0f, {});
    else
        DBG("فشل تحميل صورة Play");

    addAndMakeVisible(playPauseButton);
    playPauseButton.addListener(this);

    for (juce::Button* btn : { &loadButton, &restartButton, &stopButton,
                           &goStartButton, &goEndButton, &loopButton, &muteButton,
                           &forwardButton, &backwardButton })
    {
        btn->addListener(this);
        addAndMakeVisible(btn);
    }

    addAndMakeVisible(volumeSlider);
    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setValue(0.5);
    volumeSlider.addListener(this);

    addAndMakeVisible(infoLabel);
    infoLabel.setColour(juce::Label::backgroundColourId, juce::Colour(40, 40, 40));
    infoLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    infoLabel.setFont(juce::Font(16.0f));
    infoLabel.setText("No file loaded", juce::dontSendNotification);
}

void PlayerGUI::resized()
{
    int y = 20;
    loadButton.setBounds(20, y, 100, 40);
    restartButton.setBounds(130, y, 80, 40);
    stopButton.setBounds(220, y, 80, 40);
    loopButton.setBounds(310, y, 100, 40);
    muteButton.setBounds(420, y, 80, 40);

    playPauseButton.setBounds(20, 70, 64, 64); // حسب حجم الصورة
    goStartButton.setBounds(120, 70, 80, 30);
    goEndButton.setBounds(220, 70, 80, 30);
    forwardButton.setBounds(320, 70, 80, 30);
    backwardButton.setBounds(420, 70, 80, 30);

    volumeSlider.setBounds(20, 110, getWidth() - 40, 30);
    infoLabel.setBounds(20, 150, getWidth() - 40, 40);
}

PlayerGUI::~PlayerGUI() {}

void PlayerGUI::buttonClicked(juce::Button* button)
{
    if (button == &loadButton)
    {
        fileChooser = std::make_unique<juce::FileChooser>(
            "Select an audio file...",
            juce::File{},
            "*.wav;*.mp3;*.flac;*.aiff;*.ogg;*.aac;*.mpeg"
        );

        fileChooser->launchAsync(
            juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
            [this](const juce::FileChooser& fc)
            {
                auto file = fc.getResult();
                if (file.existsAsFile())
                    playerAudio.loadFile(file);

                infoLabel.setText(
                    "Title: " + playerAudio.getTitle() +
                    " | Artist: " + playerAudio.getArtist() +
                    " | Album: " + playerAudio.getAlbum() +
                    " | Duration: " + juce::String(playerAudio.getDuration(), 2) + "s",
                    juce::dontSendNotification
                );
            });
    }

    if (button == &restartButton)
    {
        playerAudio.setPosition(0.0);
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
            if (playIcon.isValid())
                playPauseButton.setImages(true, true, true, playIcon, 1.0f, {}, playIcon, 1.0f, {}, playIcon, 1.0f, {});
        }
        else
        {
            playerAudio.start();
            if (pauseIcon.isValid())
                playPauseButton.setImages(true, true, true, pauseIcon, 1.0f, {}, pauseIcon, 1.0f, {}, pauseIcon, 1.0f, {});
        }

        isPlaying = !isPlaying;
    }

    if (button == &goStartButton)
        playerAudio.setPosition(0.0);

    if (button == &goEndButton)
        playerAudio.setPosition(playerAudio.getLength());

    if (button == &loopButton)
    {
        bool newState = !playerAudio.getLooping();
        playerAudio.setLooping(newState);
        loopButton.setButtonText(newState ? "Loop: On" : "Loop: Off");
    }

    if (button == &muteButton)
    {
        playerAudio.setMuted(!isMuted);
        muteButton.setButtonText(isMuted ? "Mute" : "Unmute");
        isMuted = !isMuted;
    }

    // ✅ Forward/Backward 10 seconds
    if (button == &forwardButton)
    {
        double newPos = playerAudio.getPosition() + 10.0;
        if (newPos > playerAudio.getLength())
            newPos = playerAudio.getLength();
        playerAudio.setPosition(newPos);
    }

    if (button == &backwardButton)
    {
        double newPos = playerAudio.getPosition() - 10.0;
        if (newPos < 0.0)
            newPos = 0.0;
        playerAudio.setPosition(newPos);
    }
}

void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider)
    {
        if (!isMuted)
            playerAudio.setGain((float)slider->getValue());
        else
            lastVolume = slider->getValue();
    }
}


