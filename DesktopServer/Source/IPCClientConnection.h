/*
  ==============================================================================

    IPCClientConnection.h
    Created: 17 Mar 2021 10:05:27pm
    Author:  Andrew Gentile

  ==============================================================================
*/

#pragma once

#include <functional>
#include <JuceHeader.h>
//==============================================================================

class IPCClientConnection  : public juce::InterprocessConnection
{
private:
    std::function<void()> connection_made_callback_{nullptr};
    
public:
    IPCClientConnection() : juce::InterprocessConnection(false)
    {
    }
    
    ~IPCClientConnection()
    {
    }
    
    void SetConnectionMadeCallback(std::function<void()> callback)
    {
        connection_made_callback_ = callback;
    }
    
    void connectionMade() override
    {
        juce::Logger::getCurrentLogger()->writeToLog("IPCClientConnection established");
        if (connection_made_callback_ != nullptr)
        {
            connection_made_callback_();
        }
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
