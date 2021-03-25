/*
  ==============================================================================

    MainComponent.h
    Created: 8 Mar 2021 9:23:37pm
    Author:  Andrew Gentile

  ==============================================================================
*/

#pragma once

#include <functional>
#include <JuceHeader.h>
#include "IPCClient.h"
//==============================================================================

class MainComponent  : public juce::AudioAppComponent
{
private:
    int kNumOutputChannels{2};
    std::unique_ptr<IPCClient> ipc_client_{nullptr};
    std::vector<juce::MemoryBlock> queue_;
    
    // TODO: These should be encoded in a message header
    int kNumChannels{2};
    int kBufferLen{512};
    
    bool isPlaying{false};
    
public:
    MainComponent()
    {
        setAudioChannels(0, kNumOutputChannels);
        Connect();
    }
    
    ~MainComponent() override
    {
        shutdownAudio();
    }
    
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override
    {
    }
    
    void getNextAudioBlock(const juce::AudioSourceChannelInfo &bufferToFill) override
    {
        if (!isPlaying || queue_.size() < 1)
        {
            bufferToFill.clearActiveBufferRegion();
            return;
        }
        
        const float *data = reinterpret_cast<const float*>(queue_.front().getData());
        for (int ch = 0; ch < bufferToFill.buffer->getNumChannels(); ch++)
        {
            bufferToFill.buffer->copyFrom(ch, 0, data, bufferToFill.numSamples);
            data += bufferToFill.numSamples;
        }
        
        queue_.erase(queue_.begin());
    }
    
    void releaseResources() override
    {
    }
    
    void Connect()
    {
        std::unique_ptr<IPCClient> ipc_client(new IPCClient(queue_));
        if(ipc_client == nullptr || !ipc_client.get()->ConnectToSocket())
        {
            juce::Logger::getCurrentLogger()->writeToLog("ConnectToSocket failed");
            return;
        }
        
        juce::Logger::getCurrentLogger()->writeToLog("ConnectToSocket success");
        ipc_client_.reset(ipc_client.release());
    }
    
    void Play()
    {
        isPlaying = true;
    }
    
    void Stop()
    {
        isPlaying = false;
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
