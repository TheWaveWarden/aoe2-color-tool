#include "ColourButton.h"

ColourButton::ColourButton (int p_player_text_number) :
    m_number (p_player_text_number) {
}

void ColourButton::setColour (juce::Colour p_col) {
	m_colour = p_col;
	repaint();
}

juce::Colour ColourButton::getColour() const {
	return m_colour;
}

void ColourButton::paint (juce::Graphics& g) {
	static constexpr auto stroke = 1.0f;
	static constexpr auto corner = 3.0f;

	// background
	g.setColour (m_colour);
	g.fillRoundedRectangle (getLocalBounds().toFloat().reduced (stroke / 2.0f), corner);

	// border
	if (m_selected)
		g.setColour (juce::Colours::white);
	else
		g.setColour (m_highlight ? juce::Colours::yellow : juce::Colours::black);

	g.drawRoundedRectangle (getLocalBounds().toFloat().reduced (stroke / 2.0f), corner, stroke);

	// text
	g.setColour (m_colour.getPerceivedBrightness() > 0.5f ? juce::Colours::black : juce::Colours::white);
	g.setFont (getHeight() * 0.2f);
	juce::String text = (m_number > 8 ? (juce::String ("Outline ") + juce::String (m_number - 8)) : (juce::String ("Player ") + juce::String (m_number)));
	g.drawText (text, getLocalBounds().reduced (5, 5), juce::Justification::topLeft);
}

void ColourButton::mouseMove (const juce::MouseEvent&) {
	m_highlight = true;
	repaint();
}
void ColourButton::mouseExit (const juce::MouseEvent&) {
	m_highlight = false;
	repaint();
}

void ColourButton::mouseDown (const juce::MouseEvent&) {
	if (onClick) {
		onClick();
	} else {
		jassertfalse;
	}
}

void ColourButton::setSelectedFromNumber (int p_number) {
	m_selected = (p_number == m_number);
	repaint();
}