/*
  ==============================================================================

    This file contains the basic startup code for the Desktop Server application.

  ==============================================================================
*/

#include <exception>
#include <JuceHeader.h>
#include "MainComponent.h"
//==============================================================================

int main (int argc, char* argv[])
{
    juce::Logger *log = juce::Logger::getCurrentLogger();
    
    try
    {
        // Load an audio file from disk
        if (argc < 2) {
            log->writeToLog("usage: DesktopServer path");
            return -1;
        }
        
        MainComponent main_component(argv[1]);
        
        // TODO: Implement a GUI to keep the MainComponent running
        log->writeToLog("\nPress any key to exit...\n");
        std::cin.get();
        
    }
    catch (std::exception &e)
    {
        log->writeToLog("Unhandled Exception: " + juce::String(e.what()));
        return -1;
    }
    
    return 0;
}
