#include "HealthBarPage.h"

HealthBarPage::HealthBarPage (ColourModder& p_modder) :
    m_modder (p_modder),
    m_open_folder_button ("Open Folder"),
    m_cancel_button ("Back") {

	addAndMakeVisible (m_open_folder_button);
	addAndMakeVisible (m_cancel_button);

	m_cancel_button.onClick = [&]() {
		setVisible (false);
	};

    m_open_folder_button.onClick = [&]() {
		auto path = m_modder.getBinaFileLocation();
        juce::URL(path.getFullPathName()).launchInDefaultBrowser();
	};
}

void HealthBarPage::paint (juce::Graphics& g) {
	g.fillAll (BACKGROUND_COLOUR);

	const auto font_height = proportionOfHeight (0.04f);
	g.setFont (font_height * 0.7f);

	auto bounds = getLocalBounds();
	bounds.removeFromTop (proportionOfHeight (0.15f));

	g.setColour (juce::Colours::white);
	g.drawText ("Unfortunately, it is not possible to mod the healthbar", bounds.removeFromTop (font_height), juce::Justification::centred);
	g.drawText ("colors with the in-game modding system. You can, however,", bounds.removeFromTop (font_height), juce::Justification::centred);
	g.drawText ("mod them by replacing one of the files in the game directory.", bounds.removeFromTop (font_height), juce::Justification::centred);

	bounds.removeFromTop (proportionOfHeight (0.10f));
	g.drawText ("To do so, open the folder below and copy the file \"50500.bina\". Next,", bounds.removeFromTop (font_height), juce::Justification::centred);
	g.drawText ("find the installation folder for AoE2:DE, then find the following path:", bounds.removeFromTop (font_height), juce::Justification::centred);

	bounds.removeFromTop (proportionOfHeight (0.05f));
	g.drawText ("AoE2DE/resources/_common/drs/interface", bounds.removeFromTop (font_height), juce::Justification::centred);
	bounds.removeFromTop (proportionOfHeight (0.05f));

	g.drawText ("In this folder, replace the \"50500.bina\" with the one you just copied.", bounds.removeFromTop (font_height), juce::Justification::centred);
	bounds.removeFromTop (proportionOfHeight (0.10f));
	g.drawText ("To undo this, replace it with the \"50500_original.bina\" file instead.", bounds.removeFromTop (font_height), juce::Justification::centred);

	//AoE2DE\resources\_common\drs\interface
}

void HealthBarPage::resized() {
	auto bounds = getLocalBounds();
	bounds      = bounds.removeFromBottom (proportionOfHeight (0.15f));
	bounds      = bounds.removeFromTop (proportionOfHeight (0.08f));

	const auto inlay_x = proportionOfWidth (0.1f);
	m_cancel_button.setBounds (bounds.removeFromLeft (getWidth() / 2).reduced (inlay_x, 0));
	m_open_folder_button.setBounds (bounds.removeFromLeft (getWidth() / 2).reduced (inlay_x, 0));
}
