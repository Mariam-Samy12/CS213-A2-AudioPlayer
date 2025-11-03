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
    for (auto* b : { &loadButton, &restartButton, &stopButton, &playPauseButton, &goStartButton, &goEndButton, &loopButton, &muteButton })
    {
        b->addListener(this);
        addAndMakeVisible(b);
    }

    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setValue((float)lastVolume);
    volumeSlider.addListener(this);
    addAndMakeVisible(volumeSlider);

    //slider


    speedSlider.setRange(0.5, 2.0, 0.01);
    speedSlider.setValue(1.0);
    speedSlider.addListener(this);
    addAndMakeVisible(speedSlider);

    positionSlider.setRange(0.0, 1.0, 0.001);

    positionSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    positionSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    positionSlider.addListener(this);
    addAndMakeVisible(positionSlider);

    startTimer(500); //slider

    addAndMakeVisible(timeLabel);
    timeLabel.setText("0:00", juce::dontSendNotification);
    timeLabel.setJustificationType(juce::Justification::centredLeft);

    //AB
    for (auto* btn : { &setAButton, &setBButton }) {
        btn->addListener(this);
        addAndMakeVisible(btn);
    }
    clearABButton.addListener(this);
    addAndMakeVisible(clearABButton);
    //play list
    addAndMakeVisible(addToPlaylistButton);
    addAndMakeVisible(playlistBox);
    addAndMakeVisible(playSelectedButton);

    addToPlaylistButton.addListener(this);
    playSelectedButton.addListener(this);
    // Track Markers
    addMarkerButton.addListener(this);
    addAndMakeVisible(addMarkerButton);

    markerList.onChange = [this]() {
        int selectedIndex = markerList.getSelectedId() - 1;
        if (selectedIndex >= 0 && selectedIndex < markers.size()) {
            playerAudio.setPosition(markers[selectedIndex].timeInSeconds);
            playerAudio.start();
        }

        };
    addAndMakeVisible(markerList);


    timeLabel.setText("00:00 / 00:00", juce::dontSendNotification);
    addAndMakeVisible(timeLabel);


    startTimer(50);

    juce::File lastFile;
    double lastPos = 0.0;
    if (playerAudio.loadSession(lastFile, lastPos) && lastFile.existsAsFile())
    {
        playerAudio.loadFile(lastFile);
        playerAudio.setPosition(lastPos);
    }
}

PlayerGUI::~PlayerGUI() {}

void PlayerGUI::resized()
{
    int y = 20;
    loadButton.setBounds(20, y, 100, 40);
    restartButton.setBounds(140, y, 80, 40);
    stopButton.setBounds(240, y, 80, 40);
    loopButton.setBounds(340, y, 100, 40);

    muteButton.setBounds(460, y, 80, 40); //  Mute button position

    //slider
    timeLabel.setBounds(20, 240, 100, 20);
    positionSlider.setBounds(70, 240, 510, 20);

    muteButton.setBounds(460, y, 80, 40);


    playPauseButton.setBounds(20, 70, 80, 30);
    goStartButton.setBounds(120, 70, 80, 30);
    goEndButton.setBounds(220, 70, 80, 30);


    volumeSlider.setBounds(20, 270, 560, 20);

    //AB
    setAButton.setBounds(20, 115, 80, 30);
    setBButton.setBounds(120, 115, 80, 30);
    clearABButton.setBounds(220, 115, 100, 30);
    // Track Markers
    addMarkerButton.setBounds(20, 160, 100, 30);
    markerList.setBounds(140, 160, 200, 30);
    //Focous mood
    addAndMakeVisible(playFromMiddleButton);
    playFromMiddleButton.setBounds(460, y, 80, 40);
    playFromMiddleButton.addListener(this);
    //play list
    addToPlaylistButton.setBounds(10, 210, 150, 30);
    playlistBox.setBounds(170, 210, 200, 30);
    playSelectedButton.setBounds(380, 210, 150, 30);
}

    volumeSlider.setBounds(20, 110, getWidth() - 40, 30);
    speedSlider.setBounds(10, 150, 200, 20);


    positionSlider.setBounds(20, 190, getWidth() - 40, 20);
    timeLabel.setBounds(20, 215, getWidth() - 40, 20);
}

void PlayerGUI::buttonClicked(juce::Button* button)
{
    if (button == &loadButton)
    {
        fileChooser = std::make_unique<juce::FileChooser>(
            "Select an audio file...",
            juce::File::getSpecialLocation(juce::File::userDesktopDirectory),
            "*");

        fileChooser->launchAsync(
            juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
            [this](const juce::FileChooser& fc)
            {
                auto file = fc.getResult();
                if (file.existsAsFile())
                {
                    if (playerAudio.loadFile(file))
                    {
                        playerAudio.setPosition(0.0);
                        playerAudio.setGain((float)lastVolume);
                        playerAudio.setMuted(false);
                        playerAudio.setSpeed(speedSlider.getValue());
                        playerAudio.saveSession(file, 0.0);
                    }
                }
            });
    }
    else if (button == &restartButton)
    {
        playerAudio.setPosition(0.0);
        playerAudio.start();
    }
    else if (button == &stopButton)
    {
        playerAudio.stop();
        playerAudio.setPosition(0.0);
    }
    else if (button == &playPauseButton)
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
    else if (button == &goStartButton)
    {
        playerAudio.setPosition(0.0);
        playerAudio.start();
    }
    else if (button == &goEndButton)
    {
        playerAudio.setPosition(playerAudio.getLength());
        playerAudio.stop();
    }
    else if (button == &loopButton)
    {
        bool state = !playerAudio.getLooping();
        playerAudio.setLooping(state);
        loopButton.setButtonText(state ? "Loop: On" : "Loop: Off");
    }
    else if (button == &muteButton)
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

    //AB
    if (button == &setAButton) {
        loopPointA = playerAudio.getPosition();
        setAButton.setButtonText("A: " + juce::String(loopPointA, 2));
    }

    if (button == &setBButton) {
        loopPointB = playerAudio.getPosition();
        setBButton.setButtonText("B: " + juce::String(loopPointB, 2));
    }

    if (button == &clearABButton) {
        loopPointA = -1.0;
        loopPointB = -1.0;
        setAButton.setButtonText("Set A");
        setBButton.setButtonText("Set B");
    }
    // Track Markers
    if (button == &addMarkerButton)
    {
        double currentTime = playerAudio.getPosition();
        markerCount++;
        juce::String label = "Marker " + juce::String(markerCount) + " (" +
            juce::String((int)currentTime / 60) + ":" +
            juce::String((int)currentTime % 60).paddedLeft('0', 2) + ")";

        markers.push_back({ currentTime, label });
        markerList.addItem(label, markerCount);
    }
    //Focous mood
    if (button == &playFromMiddleButton)
    {
        double length = playerAudio.getLength();
        double middle = length / 2.0;
        playerAudio.setPosition(middle);
        playerAudio.start();
    }
    //play list
    if (button == &addToPlaylistButton)
    {
        fileChooser = std::make_unique<juce::FileChooser>("Select Audio Files", juce::File{}, "*.wav;*.mp3");
        fileChooser->launchAsync(juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectMultipleItems,
            [this](const juce::FileChooser& chooser)
            {
                auto files = chooser.getResults();
                for (auto& file : files)
                {
                    playlist.push_back(file);
                    playlistBox.addItem(file.getFileName(), playlistBox.getNumItems() + 1);
                }
            });
    }
    else if (button == &playSelectedButton)
    {
        int selectedIndex = playlistBox.getSelectedItemIndex();
        if (selectedIndex >= 0 && selectedIndex < playlist.size())
        {
            playerAudio.stop();
            playerAudio.loadFile(playlist[selectedIndex]);
            playerAudio.start();
        }
    }

        isMuted = !isMuted;
    }

}

void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{

    //slider


    if (slider == &volumeSlider)
    {
        if (!isMuted)
            playerAudio.setGain((float)slider->getValue());
        else
            lastVolume = slider->getValue();
    }
    else if (slider == &speedSlider)
    {
        playerAudio.setSpeed(speedSlider.getValue());
    }
    else if (slider == &positionSlider)
    {
        double len = playerAudio.getLength();
        if (len > 0)
        {
            playerAudio.setPosition(positionSlider.getValue() * len);
            // save new position once user seeks
            juce::File lastFile;
            double pos;
            if (playerAudio.loadSession(lastFile, pos))
                playerAudio.saveSession(lastFile, playerAudio.getPosition());
        }
    }
else if (slider == &positionSlider)
{
    playerAudio.setPosition(positionSlider.getValue());
}
}

void PlayerGUI::timerCallback()
{
    double pos = playerAudio.getPosition();
    double len = playerAudio.getLength();

    if (len > 0.0)
    {
        positionSlider.setValue(pos / len, juce::dontSendNotification);

        int curSec = (int)pos;
        int lenSec = (int)len;

        auto toMMSS = [](int sec)
            {
                int m = sec / 60;
                int s = sec % 60;
                return juce::String::formatted("%02d:%02d", m, s);
            };

        timeLabel.setText(toMMSS(curSec) + " / " + toMMSS(lenSec), juce::dontSendNotification);
    }
// auto-save playback position
juce::File lastFile;
double savedPos = 0.0;
if (playerAudio.loadSession(lastFile, savedPos))
    playerAudio.saveSession(lastFile, playerAudio.getPosition());
}