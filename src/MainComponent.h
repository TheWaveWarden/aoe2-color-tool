#pragma once

#include "ColourButton.h"
#include "ColourModder.h"
#include <JuceHeader.h>
#include "HealthBarPage.h"
#include <map>

#define COLOR_PICKER_HEIGHT_PERCENT 70
#define MOD_BUTTON_TEXT "Create Color Mod"
#define CLEAR_BUTTON_TEXT "Remove Existing Color Mod"
#define EDIT_OUTLINE_BUTTON_TEXT "Edit Outlines Separately"

class MainComponent : public juce::Component, public juce::ChangeListener {
public:
	//==============================================================================
	MainComponent();
	~MainComponent() override;

	//==============================================================================
	void paint (juce::Graphics&) override;
	void resized() override;

	void startEditingPlayer (int p_player);
	void changeListenerCallback (ChangeBroadcaster*) override;
	void setSelectedButton (int p_player);

private:
	static constexpr auto NUM_PLAYERS = 8;
	static constexpr auto NUM_BUTTONS = 16;

	void positionColourButtons (juce::Rectangle<int> p_bounds);

	std::array<std::unique_ptr<ColourButton>, NUM_BUTTONS> m_buttons;

	std::map<int, std::string> m_player_color_names = {{1, "Blue"}, {2, "Red"}, {3, "Green"}, {4, "Yellow"}, {5, "Teal"}, {6, "Purple"}, {7, "Grey"}, {8, "Orange"}};

	juce::ColourSelector m_colour_picker;
	juce::TextButton m_mod_button;
	juce::TextButton m_clear_button;
    HealthBarPage m_health_bar_page;

	ColourModder m_colour_modder;

	int m_current_player = 0;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
