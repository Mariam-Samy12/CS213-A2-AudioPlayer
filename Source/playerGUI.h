#pragma once // PlayerGUI.h
#include <JuceHeader.h>
#include "PlayerAudio.h"
class PlayerGUI : public juce::Component,
	public juce::Button::Listener,
	public juce::Slider::Listener
{
public:
	PlayerGUI();
	~PlayerGUI() override;

	void resized() override;

	void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
	void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
	void releaseResources();
	void paint(juce::Graphics& g) override;
private:
	PlayerAudio playerAudio;
	// GUI elements

	juce::TextButton loadButton{ "Load File" };  
juce::TextButton restartButton{ "Restart" };  
juce::TextButton stopButton{ "Stop" };  
juce::TextButton playPauseButton{ "Play" };  
juce::TextButton goStartButton{ "<" };  
juce::TextButton goEndButton{ ">" };
juce::Label infoLabel;
	juce::Slider volumeSlider;
	juce::TextButton loopButton{ "Loop: Off" };//LOOP
	
	std::unique_ptr < juce::FileChooser> fileChooser;
	// Event handlers
	void buttonClicked(juce::Button* button) override;
	void sliderValueChanged(juce::Slider* slider) override;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI)
};