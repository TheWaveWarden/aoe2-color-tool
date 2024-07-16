cmake_minimum_required(VERSION 3.22)

project(AoE2ColorTool VERSION 1.2.0)

add_subdirectory(JUCE)                    # If you've put JUCE in a subdirectory called JUCE
juce_add_gui_app(AoE2ColorTool
    PRODUCT_NAME "AoE2ColorTool")     # The name of the final executable, which can differ from the target name
juce_generate_juce_header(AoE2ColorTool)

target_sources(AoE2ColorTool
    PRIVATE
        src/Main.cpp
        src/MainComponent.cpp
        src/ColourModder.cpp
        src/ColourButton.cpp
        src/HealthBarPage.cpp)

target_compile_definitions(AoE2ColorTool
    PRIVATE
        JUCE_MODAL_LOOPS_PERMITTED=1
        JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_gui_app` call
        JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_gui_app` call
        JUCE_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:AoE2ColorTool,JUCE_PRODUCT_NAME>"
        JUCE_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:AoE2ColorTool,JUCE_VERSION>")

include_directories(json/include)

juce_add_binary_data(GuiAppData
    SOURCES 
        Assets/50500.bina
        Assets/UIColors.json
        Assets/info.json
        Assets/playercolor_red.pal
        Assets/thumbnail.jpg
        Assets/spritecolors.json
)

target_link_libraries(AoE2ColorTool
    PRIVATE
        GuiAppData
        juce::juce_gui_extra
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)
