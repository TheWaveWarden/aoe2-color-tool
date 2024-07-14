#pragma once

#include <JuceHeader.h>

class ColourButton : public juce::Component {
public:
	ColourButton (int p_player_text_number);

	void setColour (juce::Colour p_col);
	juce::Colour getColour() const;

	void setSelectedFromNumber (int p_number);

	void paint (juce::Graphics& g) override;
	void mouseMove (const juce::MouseEvent& p_event) override;
	void mouseExit (const juce::MouseEvent& p_event) override;
	void mouseDown (const juce::MouseEvent& p_event) override;

	std::function<void()> onClick = nullptr;

private:
	juce::Colour m_colour = juce::Colours::pink;
	bool m_highlight      = false;
	bool m_selected       = false;
	const int m_number;
};