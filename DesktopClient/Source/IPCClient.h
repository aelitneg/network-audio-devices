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
    std::vector<juce::MemoryBlock> &queue_;
    juce::CriticalSection &objectLock_;
    
public:
    IPCClient(std::vector<juce::MemoryBlock> &queue, juce::CriticalSection &objectLock) :
        juce::InterprocessConnection(false), queue_(queue), objectLock_(objectLock)
    {
    }
    
    ~IPCClient()
    {
        disconnect();
    }
    
    bool ConnectToSocket(juce::String &hostname, int port_number)
    {
        int timeout{15 * 1000};
        return connectToSocket(hostname, port_number, timeout);
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
        const juce::ScopedLock scopedLock (objectLock_);
        queue_.push_back(msg); // Uses copy constructor
    }
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IPCClient)
};
