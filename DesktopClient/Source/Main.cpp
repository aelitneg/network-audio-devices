/*
  ==============================================================================

    This file is the entry point for the Desktop Client application.

  ==============================================================================
*/

#include <JuceHeader.h>

#include "MainComponent.h"
//==============================================================================

int main (int argc, char* argv[])
{
    MainComponent main_component;
    
    // TODO: Implement a GUI to keep the MainComponent running
    juce::Logger::getCurrentLogger()->writeToLog("Press any key to play");
    std::cin.get();
    main_component.Play();
    
    juce::Logger::getCurrentLogger()->writeToLog("Press any key to exit");
    std::cin.get();
    main_component.Stop();
    
    return 0;
}
