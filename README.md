# network-audio-devices

A series of prototypes related to streaming audio over a network.

## Environment Setup

These instructions will describe how to setup a development environment to build, run, and develop the projects in this repository.

### JUCE

These projects are built using the [JUCE](https://juce.com/) audio framekwork. These steps will setup a JUCE deveelopment environment on your computer. If you already have JUCE a JUCE development environment setup, you can skip these steps.

1. [Download JUCE](https://juce.com/get-juce/download) for your platform
1. Extract the file zip file to a known location on your computer
1. Open the _Projucer.app_ in the `/JUCE` directory
1. Open the _Global Paths_ menu and verify the paths for
   - Path to JUCE
   - JUCE Modules

### Build

1. Open one of the `.jucer` files in this repository
1. Click the _Save and Open in IDE_ button at the top
1. Once in the IDE, build, run, and debug as normal

## Prototypes

This section provides a brief summary of the prototypes

### Sandbox

**Note:** Sandbox is used for testing basic concepts in C++ and JUCE.

Currently, the sandbox contains an example of the basic memory flow of the DesktopServer / DesktopClient prototypes. It reads an audio file from disk, converts it to a AudioBuffer, then to a MemoryBlock, and finally back to a buffer which is passed to te OS for playback.

### DesktopServer

Transmit audio in real time over a network connection. Loads an audio file from disk into a queue of AudioBuffers. Starts an InterprocessConnectionServer listening for client connections. When a client connects, it sends the buffers over the connection as MemoryBlocks.

### DesktopClient

Receive audio in real time over a network connection. Using an InterprocessConnection to a DesktopServer instance, receives audio data as a MemoryBlock. The data from the MemoryBlock is copied into an AudioBuffer provided by the OS for playback by the system audio hardware.
