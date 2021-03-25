/*
  ==============================================================================

    This file contains the basic startup code for a JUCE application.

  ==============================================================================
*/

#include <JuceHeader.h>

//==============================================================================
class MainComponent : public juce::AudioAppComponent
{
private:
    int kNumInputChannels{0};
    int kNumOutputChannels{2};
    
    juce::AudioFormatManager format_manager_;
    const char* kFilePath_{"/Users/andrew.gentile/Desktop/audio.wav"};
    
    std::unique_ptr<juce::AudioFormatReader> reader_{nullptr};
    juce::int64 cursor_{0};
    
    std::unique_ptr<juce::AudioBuffer<float>> read_buffer_{nullptr};
    std::unique_ptr<juce::AudioBuffer<float>> write_buffer_{nullptr};
    std::unique_ptr<juce::MemoryBlock> memory_block_{nullptr};
    
public:
    MainComponent()
    {
        setAudioChannels(kNumInputChannels, kNumOutputChannels);
        
        format_manager_.registerBasicFormats();
        
        juce::File file (kFilePath_);
        
        std::unique_ptr<juce::AudioFormatReader> reader (format_manager_.createReaderFor(file));
        reader_.reset(reader.release());
    }
    
    ~MainComponent() override
    {
        shutdownAudio();
    }
    
    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override
    {
        // Initialize read buffer
        std::unique_ptr<juce::AudioBuffer<float>> read_buffer(new juce::AudioBuffer<float>());
        if (read_buffer != nullptr) {
            read_buffer->setSize(kNumOutputChannels, samplesPerBlockExpected);
            read_buffer_.reset(read_buffer.release());
        }
        
        // Initialize write buffer
        std::unique_ptr<juce::AudioBuffer<float>> write_buffer(new juce::AudioBuffer<float>());
        if (write_buffer != nullptr)
        {
            write_buffer->setSize(kNumOutputChannels, samplesPerBlockExpected);
            write_buffer_.reset(write_buffer.release());
        }
        
        // Initialize memory block
        std::unique_ptr<juce::MemoryBlock> memory_block(new juce::MemoryBlock());
        if (memory_block != nullptr)
        {
            int num_channels = read_buffer_.get()->getNumChannels();
            int num_samples = read_buffer_.get()->getNumSamples();
            
            memory_block.get()->setSize(num_channels * num_samples * sizeof(float));
            
            memory_block_.reset(memory_block.release());
        }
    }
    
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override
    {
        // Play silence if no AudioFormatReader is available
        if (reader_ == nullptr)
        {
            bufferToFill.clearActiveBufferRegion();
            return;
        }
        
        // Read samples from audio file into read buffer
        reader_.get()->read(read_buffer_.get(), 0, bufferToFill.numSamples, cursor_, true, true);
        
        // Read samples from buffer into memory block
        for (int ch = 0; ch < read_buffer_.get()->getNumChannels(); ch++)
        {
            memory_block_.get()->copyFrom(
                                          read_buffer_.get()->getArrayOfWritePointers()[ch],
                                          read_buffer_.get()->getNumSamples() * sizeof(float) * ch,
                                          read_buffer_.get()->getNumSamples() * sizeof(float));
        }
        
        // Read sample from memory block into write buffer
        float *memory_block = reinterpret_cast<float*>(memory_block_.get()->getData());
        for (int ch = 0; ch < write_buffer_.get()->getNumChannels(); ch++)
        {
            write_buffer_.get()->copyFrom(ch, 0, memory_block, write_buffer_.get()->getNumSamples());
            memory_block += write_buffer_.get()->getNumSamples();
        }
        
        // Read from buffer into hardware playback buffer
        for (int ch = 0; ch < bufferToFill.buffer->getNumChannels(); ch++)
        {
            bufferToFill.buffer->copyFrom(
                                          ch,
                                          0,
                                          write_buffer_.get()->getArrayOfReadPointers()[ch],
                                          bufferToFill.buffer->getNumSamples());
        }
        
        cursor_ += bufferToFill.numSamples;
    }
    
    void releaseResources() override
    {
        // Nothing to do here.
    }
};

//==============================================================================
int main (int argc, char* argv[])
{
    MainComponent main_component;
    
    std::cout << "Press any key to exit" << std::endl;
    
    char exit;
    std::cin >> exit;
    
    return 0;
}
