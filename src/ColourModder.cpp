#include "ColourModder.h"
#include <fstream>

float min3 (float p_a, float p_b, float p_c) {
	return std::min (std::min (p_a, p_b), p_c);
}

float max3 (float p_a, float p_b, float p_c) {
	return std::max (std::max (p_a, p_b), p_c);
}

HsvFloat rgb_to_hsv (const RgbInt& p_rgb) {
	float r = float (p_rgb.r) / 255.0f;
	float g = float (p_rgb.g) / 255.0f;
	float b = float (p_rgb.b) / 255.0f;

	float c_max = max3 (r, g, b);
	float c_min = min3 (r, g, b);
	float delta = c_max - c_min;

	HsvFloat ret;

	if (delta < 0.01) {
		ret.h = 0.f;
	} else {
		if (fabs (c_max - r) < 0.01) {
			ret.h = 60.f * (fmod ((g - b) / delta, 6.f));
		} else if (fabs (c_max - g) < 0.01) {
			ret.h = 60.f * ((b - r) / delta + 2);
		} else {
			ret.h = 60.f * ((r - g) / delta + 4);
		}
	}

	if (c_max < 0.01) {
		ret.s = 0.f;
	} else {
		ret.s = delta / c_max;
	}

	ret.v = c_max;

	return ret;
}

RgbInt hsv_to_rgb (const HsvFloat& p_hsv) {
	RgbInt ret;

	float c = p_hsv.v * p_hsv.s;
	float x = c * fabs (1 - fabs (fmod (p_hsv.h / 60.f, 2.f) - 1));
	float m = p_hsv.v - c;

	auto h = p_hsv.h;
	h      = h < 0 ? h + 360.f : h;

	float r, g, b;

	if (h < 60.f) {
		r = c;
		g = x;
		b = 0.f;
	} else if (h < 120.f) {
		r = x;
		g = c;
		b = 0.f;
	} else if (h < 180.f) {
		r = 0;
		g = c;
		b = x;
	} else if (h < 240.f) {
		r = 0.f;
		g = x;
		b = c;
	} else if (h < 300.f) {
		r = x;
		g = 0.f;
		b = c;
	} else {
		r = c;
		g = 0.f;
		b = x;
	}

	ret.r = juce::roundToInt ((r + m) * 255.0f);
	ret.g = juce::roundToInt ((g + m) * 255.0f);
	ret.b = juce::roundToInt ((b + m) * 255.0f);

	return ret;
}

bool is_color_string (const std::string& p_line) {
	if (p_line.find (" ") == std::string::npos) {
		return false;
	}
	return true;
}

struct RgbInt get_rgb (const std::string& p_line) {
	RgbInt ret;

	auto pos = p_line.find (" ");
	ret.r    = std::stoi (p_line.substr (0, pos));

	auto p_line_mod = p_line.substr (pos + 1, p_line.size() - pos - 1);
	pos             = p_line_mod.find (" ");
	ret.g           = std::stoi (p_line_mod.substr (0, pos));

	p_line_mod = p_line_mod.substr (pos + 1, p_line_mod.size() - pos - 1);
	ret.b      = std::stoi (p_line_mod);

	return ret;
}

bool is_valid_random_number_folder (const juce::File p_file) {
	//we're looking for a folder with a random name like "76561198087967090"
	auto local_name = p_file.getFileName().toStdString();
	if (local_name.size() < 10) {
		return false;
	}
	return local_name.find_first_not_of ("0123456789") == std::string::npos;
}

ColourModder::ColourModder() {
	//Find the local mods folder:
	m_local_mods_folder = juce::File (juce::File::getSpecialLocation (juce::File::SpecialLocationType::userHomeDirectory).getFullPathName());
	DBG (m_local_mods_folder.getFullPathName());
	if (!m_local_mods_folder.isDirectory()) {
		juce::AlertWindow::showMessageBox (
		    juce::AlertWindow::AlertIconType::WarningIcon,
		    "Path not found",
		    "The path " + m_local_mods_folder.getFullPathName() + " was not found. The tool will not work as intended!",
		    "Shit!");
		return;
	}

	m_local_mods_folder = m_local_mods_folder.getChildFile ("Games");
	DBG (m_local_mods_folder.getFullPathName());
	if (!m_local_mods_folder.isDirectory()) {
		juce::AlertWindow::showMessageBox (
		    juce::AlertWindow::AlertIconType::WarningIcon,
		    "Path not found",
		    "The path " + m_local_mods_folder.getFullPathName() + " was not found. The tool will not work as intended!",
		    "Shit!");
	}

	m_local_mods_folder = m_local_mods_folder.getChildFile ("Age of Empires 2 DE");
	DBG (m_local_mods_folder.getFullPathName());
	if (!m_local_mods_folder.isDirectory()) {
		juce::AlertWindow::showMessageBox (
		    juce::AlertWindow::AlertIconType::WarningIcon,
		    "Path not found",
		    "The path " + m_local_mods_folder.getFullPathName() + " was not found. The tool will not work as intended!",
		    "Shit!");
		return;
	}

	bool found_random_number_folder = false;
	auto child_dirs                 = m_local_mods_folder.findChildFiles (juce::File::TypesOfFileToFind::findDirectories, false);
	for (auto dir : child_dirs) {
		DBG (dir.getFullPathName());

		if (is_valid_random_number_folder (dir)) {
			DBG ("Found random number folder: " << dir.getFullPathName());
			found_random_number_folder = true;
			m_local_mods_folder        = dir;
			break;
		}
	}

	if (!found_random_number_folder) {
		juce::AlertWindow::showMessageBox (
		    juce::AlertWindow::AlertIconType::WarningIcon,
		    "Path not found",
		    "The random number path in " + m_local_mods_folder.getFullPathName() + " was not found. The tool will not work as intended!",
		    "Shit!");
		return;
	}

	m_local_mods_folder = m_local_mods_folder.getChildFile ("mods");
	DBG (m_local_mods_folder.getFullPathName());
	if (!m_local_mods_folder.isDirectory()) {
		juce::AlertWindow::showMessageBox (
		    juce::AlertWindow::AlertIconType::WarningIcon,
		    "Path not found",
		    "The path " + m_local_mods_folder.getFullPathName() + " was not found. The tool will not work as intended!",
		    "Shit!");
		return;
	}

	m_local_mods_folder = m_local_mods_folder.getChildFile ("local");
	DBG (m_local_mods_folder.getFullPathName());
	if (!m_local_mods_folder.isDirectory()) {
		juce::AlertWindow::showMessageBox (
		    juce::AlertWindow::AlertIconType::WarningIcon,
		    "Path not found",
		    "The path " + m_local_mods_folder.getFullPathName() + " was not found. The tool will not work as intended!",
		    "Shit!");
		return;
	}
}

void ColourModder::modColour (int p_id, juce::Colour p_col) {
	bool created_mod = false;
	if (!modFolderExists()) {
		created_mod = true;
		createModSkeleton();
	}

	auto palette_folder = m_local_mods_folder.getChildFile (MOD_FOLDER_NAME).getChildFile ("resources").getChildFile ("_common").getChildFile ("palettes");
	if (!palette_folder.isDirectory()) {
		juce::AlertWindow::showMessageBox (
		    juce::AlertWindow::AlertIconType::WarningIcon,
		    "Unable to find palette folder: " + palette_folder.getFullPathName().toStdString(),
		    "Shit!");
		return;
	}

	std::string file_name = "playercolor_" + juce::String (m_player_color_names[p_id]).toLowerCase().toStdString() + ".pal";
	DBG (file_name);

	//copy over red palette file as basis
	juce::MemoryInputStream colors_stream (BinaryData::playercolor_red_pal, BinaryData::playercolor_red_palSize, false);
	auto temp_red_palette_file = palette_folder.getChildFile ("red_temp.pal");
	juce::FileOutputStream output_file_stream (temp_red_palette_file);
	output_file_stream.setPosition (0);
	output_file_stream.write (colors_stream.getData(), colors_stream.getDataSize());
	output_file_stream.flush();

	//modify palette file
	std::ifstream ifs (temp_red_palette_file.getFullPathName().toStdString());
	std::ofstream ofs (palette_folder.getChildFile (file_name).getFullPathName().toStdString());
	std::string line;

	if (!ofs.is_open()) {
		DBG ("Error openening palette file " << palette_folder.getChildFile (file_name).getFullPathName());
	} else {
		DBG ("Opening file " << palette_folder.getChildFile (file_name).getFullPathName());
	}

	RgbInt selected_rgb;
	selected_rgb.r    = p_col.getRed();
	selected_rgb.g    = p_col.getGreen();
	selected_rgb.b    = p_col.getBlue();
	auto selected_hsv = rgb_to_hsv (selected_rgb);

	while (std::getline (ifs, line)) {
		if (is_color_string (line)) {
			auto rgb = get_rgb (line);
			//std::cout << rgb.r << ", " << rgb.g << ", " << rgb.b << std::endl;

			auto hsv = rgb_to_hsv (rgb);
			//DBG("hsv: " <<  hsv.h << ", " << hsv.s << ", " << hsv.v);

			//mod color
			hsv.h += selected_hsv.h;
			hsv.h = hsv.h < 0 ? hsv.h + 360 : hsv.h;
			hsv.h = hsv.h > 360 ? hsv.h - 360 : hsv.h;

			hsv.s *= selected_hsv.s;
			hsv.v *= selected_hsv.v;

			auto rgb_back = hsv_to_rgb (hsv);
			ofs << rgb_back.r << " " << rgb_back.g << " " << rgb_back.b << std::endl;
		} else {
			ofs << line << std::endl;
		}
	}
	ifs.close();
	ofs.close();

	temp_red_palette_file.deleteFile();

	//modify UI colors
	auto color_name_ui = juce::String (m_player_color_names[p_id]).toStdString();

	//teal is named aqua here for some reason
	if (color_name_ui == "Teal") {
		color_name_ui = "Aqua";
	}
	{ //todo remove
		//create backup of UIColors to read from
		auto ui_colors_original = m_local_mods_folder.getChildFile (MOD_FOLDER_NAME).getChildFile ("widgetui").getChildFile ("UIColors.json");
		auto ui_colors_tmp      = m_local_mods_folder.getChildFile (MOD_FOLDER_NAME).getChildFile ("widgetui").getChildFile ("UIColors_temp.json");
		ui_colors_original.copyFileTo (ui_colors_tmp);

		ifs = std::ifstream (ui_colors_tmp.getFullPathName().toStdString());
		ofs = std::ofstream (ui_colors_original.getFullPathName().toStdString());

		while (std::getline (ifs, line)) {
			if (line.find (color_name_ui) != std::string::npos) {
				DBG ("Found line containing " << color_name_ui);

				RgbInt selected_white;
				selected_white.r = juce::roundToInt ((255.0f + selected_rgb.r) / 2.0f);
				selected_white.r = selected_white.r > 255 ? 255 : selected_white.r;
				selected_white.g = juce::roundToInt ((255.0f + selected_rgb.g) / 2.0f);
				selected_white.g = selected_white.g > 255 ? 255 : selected_white.g;
				selected_white.b = juce::roundToInt ((255.0f + selected_rgb.b) / 2.0f);
				selected_white.b = selected_white.b > 255 ? 255 : selected_white.b;

				RgbInt selected_light;
				selected_light.r = juce::roundToInt (255.0f / 3.0f + (selected_rgb.r) * 2.0f / 3.0f);
				selected_light.r = (selected_light.r > 255 ? 255 : selected_light.r);
				selected_light.g = juce::roundToInt (255.0f / 3.0f + (selected_rgb.g) * 2.0f / 3.0f);
				selected_light.g = (selected_light.g > 255 ? 255 : selected_light.g);
				selected_light.b = juce::roundToInt (255.0f / 3.0f + (selected_rgb.b) * 2.0f / 3.0f);
				selected_light.b = (selected_light.b > 255 ? 255 : selected_light.b);

				RgbInt selected_dark;
				selected_dark.r = juce::roundToInt ((selected_rgb.r) * 2.0f / 3.0f);
				selected_dark.g = juce::roundToInt ((selected_rgb.g) * 2.0f / 3.0f);
				selected_dark.b = juce::roundToInt ((selected_rgb.b) * 2.0f / 3.0f);

				auto string_white = std::to_string (selected_white.r) + ", " + std::to_string (selected_white.g) + ", " + std::to_string (selected_white.b);
				auto string_light = std::to_string (selected_light.r) + ", " + std::to_string (selected_light.g) + ", " + std::to_string (selected_light.b);
				auto string_dark  = std::to_string (selected_dark.r) + ", " + std::to_string (selected_dark.g) + ", " + std::to_string (selected_dark.b);
				auto string_rgb   = std::to_string (selected_rgb.r) + ", " + std::to_string (selected_rgb.g) + ", " + std::to_string (selected_rgb.b);

				//write original line
				ofs << line << std::endl;
				ofs << "      \"Text\": [ " + string_white + ", 255 ]," << std::endl;
				ofs << "      \"TextOutline\": [ 0, 0, 0, 255 ]," << std::endl;
				ofs << "      \"Icons\": [ " + string_rgb + ", 255 ]," << std::endl;
				ofs << "      \"HealthBar\": [ " + string_white + ", 255 ]," << std::endl;
				ofs << "      \"TimelineDark\": [ " + string_dark + ", 255 ]," << std::endl;
				ofs << "      \"TimelineLight\": [ " + string_light + ", 255 ]," << std::endl;
				ofs << "      \"MiniMap\": [ " + string_rgb + ", 255 ]" << std::endl;

				//skip getline forward
				for (int skip = 0; skip < 7; ++skip) {
					std::getline (ifs, line);
					DBG ("Skipping line " + line);
				}
			} else {
				ofs << line << std::endl;
			}
		}
		ifs.close();
		ofs.close();
	} //todo remove

	//create backup of bina file to read from (contains healthbar colors)
	auto bina_original = m_local_mods_folder.getChildFile (MOD_FOLDER_NAME).getChildFile ("resources").getChildFile ("_common").getChildFile ("drs").getChildFile ("interface").getChildFile ("50500.bina");
	auto bina_tmp      = m_local_mods_folder.getChildFile (MOD_FOLDER_NAME).getChildFile ("resources").getChildFile ("_common").getChildFile ("drs").getChildFile ("interface").getChildFile ("50500_tmp.bina");
	bina_original.copyFileTo (bina_tmp);

	ifs = std::ifstream (bina_tmp.getFullPathName().toStdString());
	ofs = std::ofstream (bina_original.getFullPathName().toStdString());

	size_t line_counter = 1;
	while (std::getline (ifs, line)) {
		if (line_counter == m_healthbar_position_in_bina_file[p_id]) {
			ofs << selected_rgb.r << " " << selected_rgb.g << " " << selected_rgb.b << std::endl;
		} else {
			ofs << line << std::endl;
		}

		++line_counter;
	}
	ifs.close();
	ofs.close();

	juce::String message;
	if (created_mod) {
		message = "Successfully created local mod. The mod will show up in your in-game mod browser. You candisable it there anytime.\n\n Restart AoE2:DE for the mod to be picked up.";
	}

	juce::AlertWindow::showMessageBox (
	    juce::AlertWindow::AlertIconType::InfoIcon,
	    "Sucess!",
	    message);
}

bool ColourModder::modFolderExists() {
	auto mod_folder = m_local_mods_folder.getChildFile (MOD_FOLDER_NAME);
	return mod_folder.isDirectory();
}

void ColourModder::createModSkeleton() {
	auto palette_dir = m_local_mods_folder.getChildFile (MOD_FOLDER_NAME).getChildFile ("resources").getChildFile ("_common").getChildFile ("palettes");
	palette_dir.createDirectory();

	auto interface_dir = m_local_mods_folder.getChildFile (MOD_FOLDER_NAME).getChildFile ("resources").getChildFile ("_common").getChildFile ("drs").getChildFile ("interface");
	interface_dir.createDirectory();

	auto widget_dir = m_local_mods_folder.getChildFile (MOD_FOLDER_NAME).getChildFile ("widgetui");
	widget_dir.createDirectory();

	//copy over vanilla UI colors file
	juce::MemoryInputStream ui_colors_stream (BinaryData::UIColors_json, BinaryData::UIColors_jsonSize, false);
	auto output_file = widget_dir.getChildFile ("UIColors.json");
	juce::FileOutputStream output_file_stream (output_file);
	output_file_stream.setPosition (0),
	    output_file_stream.write (ui_colors_stream.getData(), ui_colors_stream.getDataSize());
	output_file_stream.flush();

	//copy over info.json
	juce::MemoryInputStream info_stream (BinaryData::info_json, BinaryData::info_jsonSize, false);
	auto output_file_info = m_local_mods_folder.getChildFile (MOD_FOLDER_NAME).getChildFile ("info.json");
	juce::FileOutputStream output_file_info_stream (output_file_info);
	output_file_info_stream.setPosition (0);
	output_file_info_stream.write (info_stream.getData(), info_stream.getDataSize());
	output_file_info_stream.flush();

	//copy over bina file (contains healthbar colors)
	juce::MemoryInputStream bina_stream (BinaryData::_50500_bina, BinaryData::_50500_binaSize, false);
	auto output_file_bina = interface_dir.getChildFile ("50500.bina");
	juce::FileOutputStream output_file_bina_stream (output_file_bina);
	output_file_bina_stream.setPosition (0);
	output_file_bina_stream.write (bina_stream.getData(), bina_stream.getDataSize());
	output_file_bina_stream.flush();

	//copy over thumbnail
	juce::MemoryInputStream thumbnail_stream (BinaryData::thumbnail_jpg, BinaryData::thumbnail_jpgSize, false);
	auto output_file_thumbnail = m_local_mods_folder.getChildFile (MOD_FOLDER_NAME).getChildFile ("thumbnail.jpg");
	juce::FileOutputStream output_file_thumbnail_stream (output_file_thumbnail);
	output_file_thumbnail_stream.setPosition (0);
	output_file_thumbnail_stream.write (thumbnail_stream.getData(), thumbnail_stream.getDataSize());
	output_file_thumbnail_stream.flush();
}

void ColourModder::removeMod() {
	if (juce::AlertWindow::showOkCancelBox (juce::AlertWindow::WarningIcon,
	                                        "Delete Modded Colors",
	                                        "Are you sure you want to delete all the previously modded colors and return to the original AoE2:DE color scheme?",
	                                        {},
	                                        {},
	                                        {})) {
		juce::File file_to_delete = m_local_mods_folder.getChildFile (MOD_FOLDER_NAME);
		if (file_to_delete.deleteRecursively()) {
			DBG ("Directory was deleted!");
		} else {
			DBG ("file was NOT deleted!");
		}
	}
}

juce::Colour ColourModder::getColourFor (int p_index) const {
	return juce::Colours::red;
}
