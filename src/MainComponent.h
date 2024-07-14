#pragma once

#include "ColourModder.h"
#include <JuceHeader.h>
#include <map>

#define COLOR_PICKER_HEIGHT_PERCENT 80
#define MOD_BUTTON_TEXT "<-- Mod this player color!"
#define CLEAR_BUTTON_TEXT "Remove all modded colors"

class MainComponent : public juce::Component {
public:
	//==============================================================================
	MainComponent();
	~MainComponent() override;

	//==============================================================================
	void paint (juce::Graphics&) override;
	void resized() override;

private:
	std::map<int, std::string> m_player_color_names = {{1, "Blue"}, {2, "Red"}, {3, "Green"}, {4, "Yellow"}, {5, "Teal"}, {6, "Purple"}, {7, "Grey"}, {8, "Orange"}};

	juce::ColourSelector m_color_picker;
	juce::ComboBox m_player_selector;
	juce::TextButton m_mod_button;
	juce::TextButton m_clear_button;

	ColourModder m_colour_modder;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
