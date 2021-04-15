#pragma once

#include <JuceHeader.h>
#include "IPCClient.h"
//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;
    
    void PortTextEditorChanged();
    void ConnectButtonClicked();
    
private:
    //==============================================================================
    int kNumOutputChannels{2};
    std::unique_ptr<IPCClient> ipc_client_{nullptr};
    std::vector<juce::MemoryBlock> queue_;
    
    juce::CriticalSection object_lock_;
    
    //==============================================================================
    juce::String hostname_{""};
    juce::String port_;
    
    //==============================================================================
    juce::Label hostname_label_;
    juce::TextEditor hostname_text_editor_;
    
    juce::Label port_label_;
    juce::TextEditor port_text_editor_;
    
    juce::TextButton connect_button_;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
