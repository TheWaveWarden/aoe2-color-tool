#include "MainComponent.h"

MainComponent::MainComponent() :
    m_colour_picker (juce::ColourSelector::ColourSelectorOptions::showColourAtTop | juce::ColourSelector::ColourSelectorOptions::showColourspace | juce::ColourSelector::ColourSelectorOptions::editableColour),
    m_mod_button (MOD_BUTTON_TEXT),
    m_clear_button (CLEAR_BUTTON_TEXT),
    m_health_bar_page (m_colour_modder) {

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
	addAndMakeVisible (m_mod_button);
	addAndMakeVisible (m_clear_button);
	addChildComponent (m_health_bar_page);

	m_colour_modder.onModHealthBarColours = [&]() { m_health_bar_page.setVisible (true); };

	m_mod_button.onClick = [&]() {
		m_colour_modder.createSpriteMod ({m_buttons[0]->getColour(),
		                                  m_buttons[1]->getColour(),
		                                  m_buttons[2]->getColour(),
		                                  m_buttons[3]->getColour(),
		                                  m_buttons[4]->getColour(),
		                                  m_buttons[5]->getColour(),
		                                  m_buttons[6]->getColour(),
		                                  m_buttons[7]->getColour()},
		                                 {m_buttons[8]->getColour(),
		                                  m_buttons[9]->getColour(),
		                                  m_buttons[10]->getColour(),
		                                  m_buttons[11]->getColour(),
		                                  m_buttons[12]->getColour(),
		                                  m_buttons[13]->getColour(),
		                                  m_buttons[14]->getColour(),
		                                  m_buttons[15]->getColour()});
	};

	m_clear_button.onClick = [&]() {
		m_colour_modder.removeMod();
		for (int button = 0; button < NUM_BUTTONS; ++button)
			m_buttons[button]->setColour (m_colour_modder.getColourFor (button));
		setSelectedButton (0);
		startEditingPlayer (0);
	};

	m_colour_picker.setColour (juce::ColourSelector::ColourIds::backgroundColourId, juce::Colours::black);
	m_colour_picker.addChangeListener (this);

	setSelectedButton (0);
	startEditingPlayer (0);

	setSize (600, 700);
}

MainComponent::~MainComponent() {
	m_colour_picker.removeChangeListener (this);
}

void MainComponent::changeListenerCallback (ChangeBroadcaster*) {
	m_buttons[m_current_player]->setColour (m_colour_picker.getCurrentColour());
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g) {
	g.fillAll (BACKGROUND_COLOUR);

	g.setColour (juce::Colours::white.withAlpha (0.3f));
	g.setFont (15);
	g.drawText (juce::String ("Version ") + juce::String (ProjectInfo::versionString), getLocalBounds().reduced (3, 3), juce::Justification::bottomRight);
}

void MainComponent::resized() {
	auto bounds = getLocalBounds();
	m_health_bar_page.setBounds (bounds);

	m_colour_picker.setBounds (bounds.removeFromTop (juce::roundToInt (getHeight() * COLOR_PICKER_HEIGHT_PERCENT / 100.f)));

	auto button_bounds = bounds.removeFromBottom (juce::roundToInt (bounds.getHeight() / 3.0f));
	m_mod_button.setBounds (button_bounds.removeFromLeft (button_bounds.getWidth() / 2));
	m_clear_button.setBounds (button_bounds);
	const auto button_height = juce::roundToInt (getHeight() * 0.05f);
	const auto button_width  = juce::roundToInt (getWidth() * 0.3);
	m_clear_button.setBounds (m_clear_button.getBounds().withSizeKeepingCentre (button_width, button_height));
	m_mod_button.setBounds (m_mod_button.getBounds().withSizeKeepingCentre (button_width, button_height));

	positionColourButtons (bounds);
}

void MainComponent::positionColourButtons (juce::Rectangle<int> p_bounds) {
	auto counter = 0;
	auto width   = juce::roundToInt (p_bounds.getWidth() / (NUM_PLAYERS / 2));
	auto height  = juce::roundToInt (p_bounds.getHeight() / 2);
	for (int row = 0; row < NUM_PLAYERS / 2; ++row) {
		auto button_bounds = p_bounds.removeFromLeft (width).reduced (2, 2);

		auto local_bounds = button_bounds.removeFromTop (height).reduced (1, 1);
		m_buttons[counter]->setBounds (local_bounds.removeFromLeft (width / 2));
		m_buttons[counter + NUM_PLAYERS]->setBounds (local_bounds.removeFromLeft (width / 2));
		++counter;
		local_bounds = button_bounds.removeFromTop (height).reduced (1, 1);
		m_buttons[counter]->setBounds (local_bounds.removeFromLeft (width / 2));
		m_buttons[counter + NUM_PLAYERS]->setBounds (local_bounds.removeFromLeft (width / 2));
		++counter;
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