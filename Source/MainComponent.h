#pragma once

#include <JuceHeader.h>
#include <map>

#define COLOR_PICKER_HEIGHT_PERCENT 80
#define MOD_BUTTON_TEXT "<-- Mod this player color!"
#define CLEAR_BUTTON_TEXT "Remove all modded colors"
#define MOD_FOLDER_NAME "Custom Colors"

class MainComponent  : public juce::Component
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    bool mod_folder_exists();
    void create_mod_skeleton();
    void remove_mod();
    void mod_color();
    
private:
    juce::ColourSelector m_color_picker;
    juce::ComboBox m_player_selector;
    juce::TextButton m_mod_button;
    juce::TextButton m_clear_button;

    juce::File m_local_mods_folder;

    std::map<int, std::string> m_player_color_names =     {{1, "Blue"}, {2, "Red"}, {3, "Green"}, {4, "Yellow"}, {5, "Teal"}, {6, "Purple"}, {7, "Grey"}, {8, "Orange"}};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
