/*
  ==============================================================================

    IPCServer.h
    Created: 13 Apr 2021 7:24:58am
    Author:  Andrew Gentile

  ==============================================================================
*/

#pragma once

#include <functional>
#include <JuceHeader.h>
#include "IPCClientConnection.h"
//==============================================================================

class IPCServer  : public juce::InterprocessConnectionServer
{
private:
    int port_{3000};
    juce::String bind_address_{""}; // Default to all addresses
    std::unique_ptr<IPCClientConnection> client_connection_{nullptr};
    std::function<void()> callback_{nullptr};
    
public:
    IPCServer()
    {
    }
    
    IPCServer(int port, juce::String &bind_address) : port_(port), bind_address_(bind_address)
    {
    }
    
    ~IPCServer()
    {
    }
    
    bool BeginWaitingForSocket()
    {
        if (!beginWaitingForSocket(port_, bind_address_))
        {
            juce::String error_message(
                                       "Error: IPCServer unable to bind to port " +
                                       juce::String(port_) + " at " + bind_address_);
            juce::Logger::getCurrentLogger()->writeToLog(error_message);
            return false;
        }
        
        juce::String log_message("IPCServer listening on port " + juce::String(port_));
        juce::Logger::getCurrentLogger()->writeToLog(log_message);
        
        return true;
    }
    
    juce::InterprocessConnection *createConnectionObject() override
    {
        juce::Logger::getCurrentLogger()->writeToLog("Client connecting to IPCServer");
        
        std::unique_ptr<IPCClientConnection> client_connection(new IPCClientConnection());
        
        client_connection_.reset(client_connection.release());
        
        return client_connection_.get();
    }
    
    bool ClientConnected()
    {
        if (client_connection_.get() != nullptr)
        {
            return client_connection_.get()->isConnected();
        }
        
        return false;
    }
    
    void SendMessage(juce::AudioBuffer<float> &audio_buffer)
    {
        juce::MemoryBlock memory_block;
        memory_block.setSize(audio_buffer.getNumChannels() * audio_buffer.getNumSamples() * sizeof(float));
        
        for (int ch = 0; ch < audio_buffer.getNumChannels(); ch++)
        {
            memory_block.copyFrom(audio_buffer.getArrayOfWritePointers()[ch],
                                  audio_buffer.getNumSamples() * sizeof(float) * ch,
                                  audio_buffer.getNumSamples() * sizeof(float));
        }
        
        client_connection_->sendMessage(memory_block);
    }
    
private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IPCServer)
};
