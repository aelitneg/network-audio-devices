/*
  ==============================================================================

    IPCClient.h
    Created: 2 Apr 2021 10:28:53pm
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
    juce::CriticalSection &object_lock_;
    
public:
    IPCClient(std::vector<juce::MemoryBlock> &queue, juce::CriticalSection &object_lock) :
        juce::InterprocessConnection(false), queue_(queue), object_lock_(object_lock)
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
        object_lock_.enter();
        queue_.push_back(msg); // Uses copy constructor
        object_lock_.exit();
    }
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IPCClient)
};
