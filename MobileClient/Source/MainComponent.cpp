#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    setSize (800, 600);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (0, kNumOutputChannels); });
    }
    else
    {
        setAudioChannels (0, kNumOutputChannels);
    }
    
    addAndMakeVisible(&hostname_label_);
    hostname_label_.setText(juce::String("Hostname"), juce::NotificationType::dontSendNotification);
    
    addAndMakeVisible(&hostname_text_editor_);
    hostname_text_editor_.setName("hostname");
    hostname_text_editor_.onTextChange = [this] { hostname_ = hostname_text_editor_.getText(); };
    
    addAndMakeVisible(&port_label_);
    port_label_.setText(juce::String("Port"), juce::NotificationType::dontSendNotification);
    
    addAndMakeVisible(&port_text_editor_);
    port_text_editor_.setName("port");
    port_text_editor_.onTextChange = [this] { port_ = port_text_editor_.getText(); };
    
    addAndMakeVisible(&connect_button_);
    connect_button_.setButtonText("Connect");
    connect_button_.onClick = [this] { ConnectButtonClicked(); };
}

MainComponent::~MainComponent()
{
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    juce::ScopedLock scoped_lock(object_lock_);
    
    if (queue_.size() < 1)
    {
        bufferToFill.clearActiveBufferRegion();
        return;
    }
    
    const float *data = reinterpret_cast<const float*>(queue_.front().getData());
    for (int ch = 0; ch < bufferToFill.buffer->getNumChannels(); ch++)
    {
        bufferToFill.buffer->copyFrom(ch, 0, data, bufferToFill.numSamples);
        data += bufferToFill.numSamples;
    }
    
    queue_.erase(queue_.begin());
}

void MainComponent::releaseResources()
{
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
    hostname_label_.setBounds(25, getHeight() / 4, getWidth() - 50, 50);
    hostname_text_editor_.setBounds(25, getHeight() / 4 + 50, getWidth() - 50, 50);
    
    port_label_.setBounds(25, getHeight() / 4 + 100, getWidth() - 50, 50);
    port_text_editor_.setBounds(25, getHeight() / 4 + 150, getWidth() - 50, 50);
    
    connect_button_.setBounds (25, getHeight() / 4 + 250, getWidth() - 50, 50);
}

void MainComponent::ConnectButtonClicked()
{
    if (ipc_client_ == nullptr)
    {
        // Connect
        std::unique_ptr<IPCClient> ipc_client(new IPCClient(queue_, object_lock_));
        if(ipc_client == nullptr || !ipc_client.get()->ConnectToSocket(hostname_, port_.getIntValue()))
        {
            juce::Logger::getCurrentLogger()->writeToLog("ConnectToSocket failed");
            connect_button_.setButtonText("Connect");
            return;
        }
        
        juce::Logger::getCurrentLogger()->writeToLog("ConnectToSocket success");
        ipc_client_.reset(ipc_client.release());
        connect_button_.setButtonText("Disconnect");
    }
    else
    {
        juce::ScopedLock scoped_lock(object_lock_);
        queue_.clear();
        
        ipc_client_.reset(nullptr);
        connect_button_.setButtonText("Connect");
    }
}
