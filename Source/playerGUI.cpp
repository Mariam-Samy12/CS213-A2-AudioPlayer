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
//slider
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
}
void PlayerGUI::resized()
{
    int y = 20;
    loadButton.setBounds(20, y, 100, 40);
    restartButton.setBounds(140, y, 80, 40);
    stopButton.setBounds(240, y, 80, 40);
    loopButton.setBounds(340, y, 100, 40);
muteButton.setBounds(460, y, 80, 40); // 🔇 Mute button position

//slider
timeLabel.setBounds(20, 235, 100, 20);
positionSlider.setBounds(20, 210, getWidth() - 40, 20);
    playPauseButton.setBounds(20, 70, 80, 30);
    goStartButton.setBounds(120, 70, 80, 30);
    goEndButton.setBounds(220, 70, 80, 30);

volumeSlider.setBounds(20, 170, getWidth() - 40, 30);

//AB
setAButton.setBounds(20, 115, 80, 30);
setBButton.setBounds(120, 115, 80, 30);
clearABButton.setBounds(220, 115, 100, 30);
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

            


}

void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
  //slider
    if (slider == &volumeSlider)
    {
        if (!isMuted)
            playerAudio.setGain((float)slider->getValue());
        else
            lastVolume = slider->getValue(); // update stored volume while muted
    }
  else if (slider == &positionSlider)
{
    playerAudio.setPosition(positionSlider.getValue());
}



}

void PlayerGUI::timerCallback() {
    //slider
    double currentPos = playerAudio.getPosition();
    double totalLength = playerAudio.getLength();

    if (playerAudio.getLength() > 0.0) {
        positionSlider.setRange(0.0, totalLength, 0.01);
        positionSlider.setValue(currentPos, juce::dontSendNotification);
    }

    int minutes = static_cast<int>(currentPos) / 60;
    int seconds = static_cast<int>(currentPos) % 60;
    juce::String timeText = juce::String(minutes) + ":" + juce::String(seconds).paddedLeft('0', 2);
    timeLabel.setText(timeText, juce::dontSendNotification);

    //AB
    if (loopPointA >= 0.0 && loopPointB > loopPointA) {
        if (currentPos >= loopPointB) {
            playerAudio.setPosition(loopPointA);
        }
    }
}

