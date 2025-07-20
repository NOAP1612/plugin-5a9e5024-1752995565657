#include "PluginProcessor.h"
#include "PluginEditor.h"

StereoReverbAudioProcessorEditor::StereoReverbAudioProcessorEditor (StereoReverbAudioProcessor& p,
                                                                    juce::AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), processor (p), apvts(vts)
{
    setSize (380, 220);

    // Room Size Knob
    roomSizeSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    roomSizeSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 20);
    roomSizeSlider.setRange(0.0, 1.0, 0.001);
    roomSizeSlider.setSkewFactor(0.7f);
    roomSizeSlider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::deepskyblue);
    roomSizeSlider.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::darkslategrey);
    roomSizeLabel.setText("Room Size", juce::dontSendNotification);
    roomSizeLabel.attachToComponent(&roomSizeSlider, false);
    addAndMakeVisible(roomSizeSlider);
    addAndMakeVisible(roomSizeLabel);

    // Decay Time Knob
    decaySlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    decaySlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 20);
    decaySlider.setRange(0.1, 10.0, 0.01);
    decaySlider.setSkewFactorFromMidPoint(1.0);
    decaySlider.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::orange);
    decaySlider.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::darkslategrey);
    decayLabel.setText("Decay Time (s)", juce::dontSendNotification);
    decayLabel.attachToComponent(&decaySlider, false);
    addAndMakeVisible(decaySlider);
    addAndMakeVisible(decayLabel);

    // Attachments
    roomSizeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts, "ROOM_SIZE", roomSizeSlider);
    decayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        apvts, "DECAY", decaySlider);
}

StereoReverbAudioProcessorEditor::~StereoReverbAudioProcessorEditor() {}

void StereoReverbAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour(0xff22232a));
    g.setColour (juce::Colours::white);
    g.setFont (22.0f);
    g.drawFittedText ("Stereo Reverb", 0, 10, getWidth(), 30, juce::Justification::centred, 1);

    g.setFont (14.0f);
    g.setColour (juce::Colours::lightgrey);
    g.drawFittedText ("Room Size", 60, 140, 100, 20, juce::Justification::centred, 1);
    g.drawFittedText ("Decay Time", 220, 140, 100, 20, juce::Justification::centred, 1);
}

void StereoReverbAudioProcessorEditor::resized()
{
    const int sliderW = 100, sliderH = 100;
    roomSizeSlider.setBounds (60, 40, sliderW, sliderH);
    decaySlider.setBounds    (220, 40, sliderW, sliderH);
}
```

```cmake
#