#pragma once

#include <JuceHeader.h>

#define MOD_FOLDER_NAME "Custom Colors"

struct RgbInt {
	int r;
	int g;
	int b;
};

struct HsvFloat {
	float h;
	float s;
	float v;
};

class ColourModder {
public:
	ColourModder();

	bool modFolderExists();
	void createModSkeleton();
	void removeMod();
	void modColour (int p_id, juce::Colour p_col);

	void createSpriteMod (std::array<juce::Colour, 8> p_team_colours, std::array<juce::Colour, 8> p_outline_colours);

	juce::Colour getColourFor (int p_index) const;

    juce::File getBinaFileLocation() const;

    std::function<void()> onModHealthBarColours;

private:
	juce::File m_local_mods_folder;
	std::map<int, std::string> m_player_color_names = {{1, "Blue"}, {2, "Red"}, {3, "Green"}, {4, "Yellow"}, {5, "Teal"}, {6, "Purple"}, {7, "Grey"}, {8, "Orange"}};

	//defines in what lines the colors for healthbar in the bina file are stored
	std::map<int, int> m_healthbar_position_in_bina_file = {{1, 24}, {2, 40}, {3, 245}, {4, 247}, {5, 255}, {6, 256}, {7, 17}, {8, 89}};
};