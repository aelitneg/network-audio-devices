/*
  ==============================================================================

    IPCClientConnection.h
    Created: 13 Apr 2021 7:24:29am
    Author:  Andrew Gentile

  ==============================================================================
*/

#pragma once

#include <functional>
#include <JuceHeader.h>
//==============================================================================

class IPCClientConnection  : public juce::InterprocessConnection
{
public:
    IPCClientConnection() : juce::InterprocessConnection(false)
    {
    }
    
    ~IPCClientConnection()
    {
    }
    
    void connectionMade() override
    {
        juce::Logger::getCurrentLogger()->writeToLog("IPCClientConnection established");
    }
    
    void connectionLost() override
    {
        juce::Logger::getCurrentLogger()->writeToLog("IPCClientConnection lost");
    }
    
    void messageReceived(const juce::MemoryBlock &msg) override
    {
    }
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IPCClientConnection)
};
