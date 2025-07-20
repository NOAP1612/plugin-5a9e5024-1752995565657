#include "PluginProcessor.h"
#include "PluginEditor.h"

StereoReverbAudioProcessor::StereoReverbAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                       ),
#endif
    apvts(*this, nullptr, "Parameters", createParameters())
{
    roomSizeParam = apvts.getRawParameterValue("ROOM_SIZE");
    decayParam    = apvts.getRawParameterValue("DECAY");
}

StereoReverbAudioProcessor::~StereoReverbAudioProcessor() {}

juce::AudioProcessorValueTreeState::ParameterLayout StereoReverbAudioProcessor::createParameters()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "ROOM_SIZE", "Room Size", juce::NormalisableRange<float>(0.0f, 1.0f, 0.001f), 0.5f,
        "Room Size"));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "DECAY", "Decay Time", juce::NormalisableRange<float>(0.1f, 10.0f, 0.01f), 2.0f,
        "Decay Time (s)"));

    return { params.begin(), params.end() };
}

const juce::String StereoReverbAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool StereoReverbAudioProcessor::acceptsMidi() const { return false; }
bool StereoReverbAudioProcessor::producesMidi() const { return false; }
bool StereoReverbAudioProcessor::isMidiEffect() const { return false; }
double StereoReverbAudioProcessor::getTailLengthSeconds() const { return 10.0; }

int StereoReverbAudioProcessor::getNumPrograms() { return 1; }
int StereoReverbAudioProcessor::getCurrentProgram() { return 0; }
void StereoReverbAudioProcessor::setCurrentProgram (int) {}
const juce::String StereoReverbAudioProcessor::getProgramName (int) { return {}; }
void StereoReverbAudioProcessor::changeProgramName (int, const juce::String&) {}

void StereoReverbAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = static_cast<juce::uint32>(samplesPerBlock);
    spec.numChannels = 2;

    reverb.reset();
    reverb.prepare(spec);
}

void StereoReverbAudioProcessor::releaseResources() {}

#ifndef JucePlugin_PreferredChannelConfigurations
bool StereoReverbAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    // Only stereo in/out supported
    return layouts.getMainInputChannelSet()  == juce::AudioChannelSet::stereo()
        && layouts.getMainOutputChannelSet() == juce::AudioChannelSet::stereo();
}
#endif

void StereoReverbAudioProcessor::updateReverbParameters()
{
    juce::Reverb::Parameters params;
    params.roomSize = *roomSizeParam;
    // Map decay time (0.1 - 10s) to damping and wetLevel for a more musical response
    params.damping = juce::jlimit(0.0f, 1.0f, 1.0f - (*decayParam / 10.0f));
    params.wetLevel = 0.33f + 0.33f * (*roomSizeParam);
    params.dryLevel = 1.0f - params.wetLevel * 0.5f;
    params.width    = 1.0f;
    params.freezeMode = 0.0f;
    reverb.setParameters(params);
}

void StereoReverbAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;
    updateReverbParameters();

    juce::dsp::AudioBlock<float> block(buffer);
    reverb.process(juce::dsp::ProcessContextReplacing<float>(block));
}

bool StereoReverbAudioProcessor::hasEditor() const { return true; }
juce::AudioProcessorEditor* StereoReverbAudioProcessor::createEditor()
{
    return new StereoReverbAudioProcessorEditor (*this, apvts);
}

void StereoReverbAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void StereoReverbAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (apvts.state.getType()))
            apvts.replaceState (juce::ValueTree::fromXml (*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new StereoReverbAudioProcessor();
}
```

```cpp