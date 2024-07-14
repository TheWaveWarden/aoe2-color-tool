#include "MainComponent.h"

MainComponent::MainComponent() :
    m_color_picker (juce::ColourSelector::ColourSelectorOptions::showColourAtTop | juce::ColourSelector::ColourSelectorOptions::showColourspace), m_mod_button (MOD_BUTTON_TEXT), m_clear_button (CLEAR_BUTTON_TEXT) {
	setSize (600, 600);

	addAndMakeVisible (m_color_picker);
	addAndMakeVisible (m_player_selector);
	addAndMakeVisible (m_mod_button);
	addAndMakeVisible (m_clear_button);

	for (int player = 1; player <= 8; ++player) {
		m_player_selector.addItem (std::to_string (player) + " (" + m_player_color_names[player] + ")", player);
	}

	m_player_selector.setSelectedId (1);

	m_mod_button.onClick = [&]() {
		m_colour_modder.modColour (m_player_selector.getSelectedId(), m_color_picker.getCurrentColour());
	};

	m_clear_button.onClick = [&]() {
		m_colour_modder.removeMod();
	};
}

MainComponent::~MainComponent() {
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g) {
	g.fillAll (juce::Colours::black);
}

void MainComponent::resized() {
	auto bounds = getLocalBounds();

	m_color_picker.setBounds (bounds.removeFromTop (juce::roundToInt (getHeight() * COLOR_PICKER_HEIGHT_PERCENT / 100.f)));

	auto upper_bounds = bounds.removeFromTop (juce::roundToInt (bounds.getHeight() / 2.0f));
	m_player_selector.setBounds (upper_bounds.removeFromLeft (juce::roundToInt (upper_bounds.getWidth() / 2.0f)));
	m_mod_button.setBounds (upper_bounds);

	m_clear_button.setBounds (bounds);
}
