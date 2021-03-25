/*
  ==============================================================================

    This file contains the basic startup code for the Desktop Server application.

  ==============================================================================
*/

#include <math.h>
#include <JuceHeader.h>
#include "MainComponent.h"
//==============================================================================

int main (int argc, char* argv[])
{
    // Load an audio file from disk
    // TODO: Make file path an argument
    MainComponent main_component("/Users/andrew.gentile/Desktop/audio.wav");
    
    // TODO: Implement a GUI to keep the MainComponent running
    juce::Logger::getCurrentLogger()->writeToLog("\nPress any key to exit...\n");
    std::cin.get();
    
    return 0;
}
