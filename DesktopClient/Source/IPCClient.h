/*
  ==============================================================================

    IPCClient.h
    Created: 9 Mar 2021 9:14:52pm
    Author:  Andrew Gentile

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
//==============================================================================

class IPCClient  : public juce::InterprocessConnection
{
private:
    juce::String hostname_{"192.168.215.51"};
    int port_number_{3000};
    int timeout_{15 * 1000};
    std::vector<juce::MemoryBlock> &queue_;
    
public:
    IPCClient(std::vector<juce::MemoryBlock> &queue) :
        juce::InterprocessConnection(false), queue_(queue)
    {
    }
    
    ~IPCClient()
    {
        disconnect();
    }
    
    bool ConnectToSocket()
    {
        return connectToSocket(hostname_, port_number_, timeout_);
    }
    
    void connectionMade() override
    {
        juce::Logger::getCurrentLogger()->writeToLog("IPCClient connection established");
    }
    
    void connectionLost() override
    {
        juce::Logger::getCurrentLogger()->writeToLog("IPCClient connection lost");
    }
    
    void messageReceived(const juce::MemoryBlock &msg) override
    {
        queue_.push_back(msg);
    }
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IPCClient)
};
