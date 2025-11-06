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
    g.fillAll(juce::Colour(25, 25, 25));

    // waves
    if (waveformLoaded)
    {
        
        int waveformY = infoLabel.getBottom() + 10;
        int waveformHeight = 60;

       
        g.setColour(juce::Colours::lightgreen);
        thumbnail.drawChannels(
            g,
            juce::Rectangle<int>(20, waveformY, getWidth() - 40, waveformHeight),
            0.0,
            thumbnail.getTotalLength(),
            1.0f
        );

        
        double currentX = juce::jmap(playerAudio.getPosition(), 0.0, playerAudio.getLength(), 20.0, (double)getWidth() - 20.0);
        g.setColour(juce::Colours::red);
        g.drawLine((float)currentX, (float)waveformY, (float)currentX, (float)(waveformY + waveformHeight), 2.0f);
    }
}
PlayerGUI::PlayerGUI()
{
    formatManager.registerBasicFormats();//waves
    for (auto* btn : { &loadButton, &restartButton, &stopButton, &playPauseButton,
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
    //speed
    speedSlider.setRange(0.5, 2.0, 0.01); // half speed to double speed
    speedSlider.setValue(1.0);            // normal speed
    speedSlider.addListener(this);
    addAndMakeVisible(speedSlider);

    addAndMakeVisible(infoLabel);
    infoLabel.setColour(juce::Label::backgroundColourId, juce::Colour(40, 40, 40));
    infoLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    infoLabel.setFont(juce::Font(16.0f));
    infoLabel.setText("No file loaded", juce::dontSendNotification);
    //slider
    positionSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    positionSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    positionSlider.addListener(this);
    addAndMakeVisible(positionSlider);
    startTimer(500);

    addAndMakeVisible(timeLabel);
    timeLabel.setText("0:00", juce::dontSendNotification);
    timeLabel.setJustificationType(juce::Justification::centredLeft);//slider
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
    addAndMakeVisible(clearPlaylistButton);
    addAndMakeVisible(removeSelectedButton);
    clearPlaylistButton.addListener(this);
    removeSelectedButton.addListener(this);


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
    //nine
    positionSlider.onDragStart = [this]() { stopTimer(); };
    positionSlider.onDragEnd = [this]() { startTimer(500); };
    addAndMakeVisible(timeLabel);
    timeLabel.setText("0:00", juce::dontSendNotification);
    timeLabel.setJustificationType(juce::Justification::centredLeft);
    addAndMakeVisible(totalTimeLabel);
    totalTimeLabel.setText("0:00", juce::dontSendNotification);
    totalTimeLabel.setJustificationType(juce::Justification::centredRight);


    // Load last session
    juce::File lastFile;
    double lastPos = 0.0;

    if (playerAudio.loadSession(lastFile, lastPos))
    {
        if (lastFile.existsAsFile())
        {
            playerAudio.loadFile(lastFile);
            playerAudio.setPosition(lastPos);
        }
    }
}


void PlayerGUI::resized()
{
    int y = 20;
    loadButton.setBounds(20, y, 100, 40);
    restartButton.setBounds(130, y, 80, 40);
    stopButton.setBounds(220, y, 80, 40);
    loopButton.setBounds(310, y, 100, 40);
    muteButton.setBounds(420, y, 80, 40);
    playFromMiddleButton.setBounds(520, y, 80, 40);
    addAndMakeVisible(playFromMiddleButton);
    playFromMiddleButton.addListener(this);

    playPauseButton.setBounds(620, y, 80, 40);
    goStartButton.setBounds(710, y, 80, 40);
    goEndButton.setBounds(800, y, 80, 40);
    forwardButton.setBounds(890, y, 80, 40);
    backwardButton.setBounds(980, y, 80, 40);

    //  Playlist
    y += 50;
    playlistBox.setBounds(20, y, 200, 30);
    addToPlaylistButton.setBounds(230, y, 100, 30);
    playSelectedButton.setBounds(340, y, 100, 30);
    removeSelectedButton.setBounds(450, y, 100, 30);
    clearPlaylistButton.setBounds(560, y, 100, 30);

    addAndMakeVisible(addToPlaylistButton);
    addAndMakeVisible(playlistBox);
    addAndMakeVisible(playSelectedButton);
    addAndMakeVisible(clearPlaylistButton);
    addAndMakeVisible(removeSelectedButton);

    //  A/B
    y += 40;
    setAButton.setBounds(20, y, 80, 30);
    setBButton.setBounds(120, y, 80, 30);
    clearABButton.setBounds(220, y, 100, 30);

    // Marker
    y += 40;
    addMarkerButton.setBounds(20, y, 100, 30);
    markerList.setBounds(140, y, 200, 30);

    // sliders
    y += 50;
    speedSlider.setBounds(20, y, 560, 20); 

    y += 40;
    timeLabel.setBounds(20, y, 60, 20);
    positionSlider.setBounds(90, y, 490, 20);
    totalTimeLabel.setBounds(590, y, 60, 20);

    y += 40;
    volumeSlider.setBounds(20, y, 560, 20);

    // InfoLabel
    y += 50;
    infoLabel.setBounds(20, y, getWidth() - 40, 40);
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
                //waves
                thumbnail.clear();
                thumbnail.setSource(new juce::FileInputSource(file));
                waveformLoaded = true;
                //___________________________________________________
                playerAudio.saveSession(file, 0.0);
                playerAudio.start();

                
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
            //waves
            thumbnail.clear();
            thumbnail.setSource(new juce::FileInputSource(playlist[selectedIndex]));
            waveformLoaded = true;
            //______________________________________________________________________
            playerAudio.start();
            infoLabel.setText(
                "Title: " + playerAudio.getTitle() +
                " | Artist: " + playerAudio.getArtist() +
                " | Album: " + playerAudio.getAlbum() +
                " | Duration: " + juce::String(playerAudio.getDuration(), 2) + "s",
                juce::dontSendNotification
            );
            

        }
    }

    if (button == &clearPlaylistButton)
    {
        playlist.clear();
        playlistBox.clear();
    }
    if (button == &removeSelectedButton)
    {
        int selectedIndex = playlistBox.getSelectedItemIndex();
        if (selectedIndex >= 0 && selectedIndex < playlist.size())
        {
            playlist.erase(playlist.begin() + selectedIndex);

            playlistBox.clear();
            for (int i = 0; i < playlist.size(); ++i)
            {
                playlistBox.addItem(playlist[i].getFileName(), i + 1);
            }
        }
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
    //speed slider
    if (slider == &speedSlider)
    {
        playerAudio.setSpeed(speedSlider.getValue());
    }




}

void PlayerGUI::timerCallback() {

    //slider
    double totalLength = playerAudio.getLength();
    double currentPos = playerAudio.getPosition();

    if (totalLength > 0.0) {
        positionSlider.setRange(0.0, totalLength, 0.01);
        positionSlider.setValue(currentPos, juce::dontSendNotification);
    }

    int minutes = static_cast<int>(currentPos) / 60;
    int seconds = static_cast<int>(currentPos) % 60;
    juce::String timeText = juce::String(minutes) + ":" + juce::String(seconds).paddedLeft('0', 2);
    timeLabel.setText(timeText, juce::dontSendNotification);

    juce::File lastFile;
    double savedPos = 0.0;
    if (playerAudio.loadSession(lastFile, savedPos))
        playerAudio.saveSession(lastFile, playerAudio.getPosition());

    //nine
    int totalMinutes = static_cast<int>(totalLength) / 60;
    int totalSeconds = static_cast<int>(totalLength) % 60;
    juce::String totalText = juce::String(totalMinutes) + ":" + juce::String(totalSeconds).paddedLeft('0', 2);
    totalTimeLabel.setText(totalText, juce::dontSendNotification);

    //AB
    if (loopPointA >= 0.0 && loopPointB > loopPointA) {
        if (currentPos >= loopPointB) {
            playerAudio.setPosition(loopPointA);
        }
    }
    //waves
    repaint();
}

