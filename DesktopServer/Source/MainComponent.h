/*
  ==============================================================================

    MainComponent.h
    Created: 21 Mar 2021 9:16:20pm
    Author:  Andrew Gentile

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "IPCServer.h"
//==============================================================================

class MainComponent  : public juce::Component
{
private:
    std::unique_ptr<juce::File> file_{nullptr};
    juce::AudioFormatManager format_manager_;
    std::unique_ptr<juce::AudioFormatReader> reader_{nullptr};
    std::vector<juce::AudioBuffer<float>> queue_;
    
    // TODO: Set audio buffer length dynmically
    int buffer_len_{512};
    int num_channels_{0};
    
    IPCServer server_;
    
public:
    MainComponent(const char* file_path)
    {
        LoadAudioFile(file_path);
        format_manager_.registerBasicFormats();
        ProcessAudioFile();
        InitializeServer();
    }

    ~MainComponent() override
    {
    }
    
    void LoadAudioFile(const char* file_path)
    {
        file_.reset(new juce::File(file_path));
        if(file_ == nullptr) {
            juce::Logger::getCurrentLogger()->writeToLog("Error: Unable to load file" + juce::String(file_path));
            return;
        }
        
        juce::Logger::getCurrentLogger()->writeToLog("Loaded audio file from " + juce::String(file_path));
    }
    
    void ProcessAudioFile()
    {
        reader_.reset(format_manager_.createReaderFor(*file_.get()));
        if(reader_ == nullptr) {
            juce::Logger::getCurrentLogger()->writeToLog("Error: Unable to create AudioFormatReader for file");
            return;
        }
        
        juce::Logger::getCurrentLogger()->writeToLog("Reading audio file into queue...");
        num_channels_ = reader_.get()->numChannels;
        queue_.reserve(ceil((float)reader_.get()->lengthInSamples / (float)buffer_len_));
        int cursor{0};
        
        while(cursor < reader_.get()->lengthInSamples)
        {
            juce::AudioBuffer<float> buffer(num_channels_, buffer_len_);

            reader_.get()->read(&buffer, 0, buffer_len_, cursor, true, true);
            queue_.push_back(buffer);
            cursor += buffer_len_;
        }
        juce::Logger::getCurrentLogger()->writeToLog("Done!");
        
    }
    
    void ProcessQueue()
    {
        juce::Logger::getCurrentLogger()->writeToLog("Process Audio Queue");
        for (auto it = queue_.begin(); it < queue_.end(); it++)
        {
            server_.SendMessage(*it);
        }
        
        juce::Logger::getCurrentLogger()->writeToLog("Done!");
    }
    
    void InitializeServer()
    {
        juce::Logger::getCurrentLogger()->writeToLog("Starting IPCServer");
        if (!server_.BeginWaitingForSocket()) { return; }
        server_.SetCallback(std::bind(&MainComponent::ProcessQueue, this));
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
