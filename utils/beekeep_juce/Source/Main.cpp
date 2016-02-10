/*
  ==============================================================================

    This file was auto-generated by the Introjucer!

    It contains the basic startup code for a Juce application.

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "GraphEditorComponent.h"
#include "files.h"
#include "net.h"
#include "print_funcs.h"
#include "preset.h"
#include "scene.h"

//==============================================================================
class beekeepApplication  : public JUCEApplication
{
public:
    //==============================================================================
    beekeepApplication() {}

    const String getApplicationName() override       { return ProjectInfo::projectName; }
    const String getApplicationVersion() override    { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override       { return true; }

    //==============================================================================
    void initialise (const String& commandLine) override
    {
        presets_init();
        
        print_dbg("\r\n net_init... ");
        net_init();
        
        print_dbg("\r\n scene_init...");
        scene_init();
        
        print_dbg("\r\n files_init...");
        files_init();
        
        mainWindow = new MainWindow (getApplicationName());
    }

    void shutdown() override
    {
       mainWindow = nullptr; // (deletes our window)
    }

    //==============================================================================
    void systemRequestedQuit() override
    {
        quit();
    }

    void anotherInstanceStarted (const String& commandLine) override
    {}

    //==============================================================================
    /*
        This class implements the desktop window that contains an instance of
        our MainContentComponent class.
    */
    class MainWindow    : public DocumentWindow
    {
    public:
        MainWindow (String name)  : DocumentWindow (name,
                                                    Colours::lightgrey,
                                                    DocumentWindow::allButtons)
        {
            setUsingNativeTitleBar (true);
//            setContentOwned (new ...(), true);
            port_.setSize(800, 600);
            setContentOwned(&port_, true);
            port_.setViewedComponent(&graph_ed_);
            setResizable(true, true);
            
            centreWithSize (getWidth(), getHeight());
            setVisible (true);
        }

        void closeButtonPressed() override
        {
            JUCEApplication::getInstance()->systemRequestedQuit();
        }

    private:
        Viewport port_;
        GraphEditorComponent graph_ed_;
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
    };

private:
    ScopedPointer<MainWindow> mainWindow;
};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (beekeepApplication)
