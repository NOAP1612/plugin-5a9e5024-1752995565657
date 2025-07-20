CMakeLists.txt

cmake_minimum_required(VERSION 3.15)

project(StereoReverbPlugin VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)

# JUCE location (edit as needed)
set(JUCE_DIR "$ENV{JUCE_DIR}" CACHE PATH "Path to JUCE")

add_subdirectory(${JUCE_DIR} JUCE)

juce_add_plugin(StereoReverb
    COMPANY_NAME "YourCompany"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE
    PLUGIN_MANUFACTURER_CODE Juce
    PLUGIN_CODE SRvb
    FORMATS VST3 AU
    PRODUCT_NAME "StereoReverb"
)

target_sources(StereoReverb
    PRIVATE
        PluginProcessor.cpp
        PluginProcessor.h
        PluginEditor.cpp
        PluginEditor.h
)

target_link_libraries(StereoReverb
    PRIVATE
        juce::juce_audio_utils
)
```