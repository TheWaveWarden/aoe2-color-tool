#pragma once

#include "ColourModder.h"
#include "JuceHeader.h"

//#define BACKGROUND_COLOUR (juce::Colours::black.interpolatedWith (juce::Colours::white, 0.2f))
//#define BACKGROUND_COLOUR (juce::Colours::darkslateblue)
#define BACKGROUND_COLOUR (juce::Colour(0xFF1D232A))


class HealthBarPage : public juce::Component {
public:
	HealthBarPage (ColourModder& p_mdoder);

	void paint (juce::Graphics& g) override;
	void resized() override;

private:
	ColourModder& m_modder;

	juce::TextButton m_open_folder_button;
	juce::TextButton m_cancel_button;
};