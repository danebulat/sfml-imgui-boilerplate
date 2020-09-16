#include "Platform/Platform.hpp"
#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include <iostream>
#include <array>

#include "imgui_utils.hpp"
#include "imgui_demos.hpp"

int main()
{
	util::Platform platform;

	sf::RenderWindow window(sf::VideoMode(800, 600), "");
    window.setVerticalSyncEnabled(true);

	// Initialize ImGui. Internal font atlas is created
	// (see ImGui-SFML README on how to use other fonts)
    ImGui::SFML::Init(window);

	sf::Clock deltaClock;

	DemoManager demoManager;
	demoManager.initialise(window);

	// Call it if you only draw ImGui, otherwise not needed
    window.resetGLStates();

	while (window.isOpen()) {
        sf::Event event;

		while (window.pollEvent(event)) {

			// process ImGui events
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

		// update ImGui
        ImGui::SFML::Update(window, deltaClock.restart());

		demoManager.update(window);

		if (demoManager.get_current_demo() != 1)
        	window.clear();

		// render ImGui windows
        ImGui::SFML::Render(window);

        window.display();
    }

	// clean up ImGui, such as deleting the internal font atlas
    ImGui::SFML::Shutdown();

	return 0;
}
