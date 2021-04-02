/*
  ==============================================================================

    This file is the entry point for the Desktop Client application.

  ==============================================================================
*/
#include <exception>
#include <string>
#include <JuceHeader.h>
#include "MainComponent.h"
//==============================================================================


int main (int argc, char* argv[])
{
    juce::Logger *log = juce::Logger::getCurrentLogger();
    
    try
    {
        MainComponent main_component;
        
        // Parse arguments
        if (argc < 2)
        {
            log->writeToLog("usage: DesktopClient hostname:port");
            return 64;
        }
        
        std::string args(argv[1]);
        std::size_t pos = args.find(':');
        if (pos == std::string::npos)
        {
            log->writeToLog("usage: DesktopClient hostname:port");
            return 64;
        }
        
        juce::String hostname = args.substr(0, pos);
        int port = std::stoi(args.substr(pos + 1));
        
        // Connect client to server
        main_component.Connect(hostname, port);
        
        // TODO: Implement a GUI to keep the MainComponent running
        log->writeToLog("Press any key to exit");
        std::cin.get();        
    }
    catch (std::exception &e)
    {
        log->writeToLog("Unhandled Exception: " + juce::String(e.what()));
        return -1;
    }
    
    
    
    
    return 0;
}
