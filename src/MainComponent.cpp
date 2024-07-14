#include "MainComponent.h"

MainComponent::MainComponent() :
    m_colour_picker (juce::ColourSelector::ColourSelectorOptions::showColourAtTop | juce::ColourSelector::ColourSelectorOptions::showColourspace), m_mod_button (MOD_BUTTON_TEXT), m_clear_button (CLEAR_BUTTON_TEXT) {

	for (int button = 0; button < NUM_BUTTONS; ++button) {
		m_buttons[button] = std::make_unique<ColourButton> (button + 1);
		addAndMakeVisible (*(m_buttons[button]));
		m_buttons[button]->setColour (m_colour_modder.getColourFor (button));

		m_buttons[button]->onClick = [&, button]() {
			startEditingPlayer (button);
			setSelectedButton (button);
		};
	}

	addAndMakeVisible (m_colour_picker);
	addAndMakeVisible (m_player_selector);
	addAndMakeVisible (m_mod_button);
	addAndMakeVisible (m_clear_button);

	for (int player = 1; player <= 8; ++player) {
		m_player_selector.addItem (std::to_string (player) + " (" + m_player_color_names[player] + ")", player);
	}

	m_player_selector.setSelectedId (1);

	m_mod_button.onClick = [&]() {
		m_colour_modder.modColour (m_player_selector.getSelectedId(), m_colour_picker.getCurrentColour());
	};

	m_clear_button.onClick = [&]() {
		m_colour_modder.removeMod();
	};

	m_colour_picker.setColour (juce::ColourSelector::ColourIds::backgroundColourId, juce::Colours::black);
	m_colour_picker.addChangeListener (this);

	setSelectedButton (0);

	setSize (600, 800);
}

MainComponent::~MainComponent() {
	m_colour_picker.removeChangeListener (this);
}

void MainComponent::changeListenerCallback (ChangeBroadcaster*) {
	m_buttons[m_current_player]->setColour (m_colour_picker.getCurrentColour());
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g) {
	g.fillAll (juce::Colours::black);
}

void MainComponent::resized() {
	auto bounds = getLocalBounds();

	m_colour_picker.setBounds (bounds.removeFromTop (juce::roundToInt (getHeight() * COLOR_PICKER_HEIGHT_PERCENT / 100.f)));

	m_clear_button.setBounds (bounds.removeFromBottom (juce::roundToInt ((bounds.getHeight() / 4.0f))));
	auto upper_bounds = bounds.removeFromBottom (juce::roundToInt (bounds.getHeight() / 3.0f));
	m_player_selector.setBounds (upper_bounds.removeFromLeft (juce::roundToInt (upper_bounds.getWidth() / 2.0f)));
	m_mod_button.setBounds (upper_bounds);

	auto counter = 0;
	auto width   = juce::roundToInt (bounds.getWidth() / (NUM_BUTTONS / 2));
	auto height  = juce::roundToInt (bounds.getHeight() / 2);
	for (int row = 0; row < NUM_BUTTONS / 2; ++row) {
		auto button_bounds = bounds.removeFromLeft (width);

		m_buttons[counter++]->setBounds (button_bounds.removeFromTop (height).reduced (1, 1));
		m_buttons[counter++]->setBounds (button_bounds.removeFromTop (height).reduced (1, 1));
	}
}

void MainComponent::startEditingPlayer (int p_player) {
	m_current_player = p_player;
	m_colour_picker.setCurrentColour (m_buttons[p_player]->getColour());
}

void MainComponent::setSelectedButton (int p_player) {
	for (auto& button : m_buttons)
		button->setSelectedFromNumber (p_player + 1); // +1 due to zero indexing
}