#include "Platform/Platform.hpp"
#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include <iostream>

/** Demo 1 Data **/
struct d1 {
	static char windowTitle[255];
	static sf::Color bgColor;
	static int integer;
	static float pos[];
	static bool open;
    static float color[3];
};

char d1::windowTitle[255] = "Hello, ImGui";
sf::Color d1::bgColor = sf::Color::Black;
int d1::integer = 32;
float d1::pos[] = {10.f, 20.f};
bool d1::open = true;
float d1::color[3] = {0.f, 0.f, 0.f};
/** End Demo 1 Data **/

void runDemo1(sf::RenderWindow&);

int main()
{
	util::Platform platform;

	sf::RenderWindow window(sf::VideoMode(800, 600), "");
    window.setVerticalSyncEnabled(true);

	// Initialize ImGui. Internal font atlas is created
	// (see ImGui-SFML README on how to use other fonts)
    ImGui::SFML::Init(window);

	window.setTitle(d1::windowTitle);
    window.resetGLStates(); // call it if you only draw ImGui. Otherwise not needed.

	sf::Clock deltaClock;

	while (window.isOpen()) {
        sf::Event event;

		while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

		runDemo1(window);

        window.clear(d1::bgColor); // fill background with color
        ImGui::SFML::Render(window);
        window.display();
    }

	// Clean up ImGui, such as deleting the internal font atlas
    ImGui::SFML::Shutdown();

	return 0;
}

void runDemo1(sf::RenderWindow& window) {

	/** Begin window **/
	ImGui::Begin("Sample window");

	// Background color edit
	if (ImGui::ColorEdit3("Background color", d1::color)) {

		// Output changed color to console
		std::cout << "color[r, g, b] = " << d1::color[0] << ' ' << d1::color[1] << ' ' << d1::color[2] << std::endl;

		// this code gets called if color value changes, so
		// the background color is upgraded automatically!
		d1::bgColor.r = static_cast<sf::Uint8>(d1::color[0] * 255.f);
		d1::bgColor.g = static_cast<sf::Uint8>(d1::color[1] * 255.f);
		d1::bgColor.b = static_cast<sf::Uint8>(d1::color[2] * 255.f);
	}

	// Creates a text edit with "Window title" label
	// Pass in windowTitle[] char array as data source for text edit
	if (ImGui::InputText("Window title", d1::windowTitle, 255)) {
		std::cout << "new windowTitle[] = " << d1::windowTitle << std::endl;
	}

	// Create a button with label "Update window title"
	if (ImGui::Button("Update window title")) {
		// this code gets if user clicks on the button
		// you could have written if(ImGui::InputText(...))
		// but this shows how buttons work
		window.setTitle(d1::windowTitle);
	}

	if (ImGui::InputInt("Integer value", &d1::integer)) {
		std::cout << "new integer: " << d1::integer << std::endl;
	}

	if (ImGui::InputFloat2("position", d1::pos)) {
		std::cout << "new pos: " << d1::pos[0] << ',' << d1::pos[1] << std::endl;
	}

	if (ImGui::ArrowButton("Left Arrow Button", ImGuiDir_Left)) {
		std::cout << "left arrow clicked\n";
	}

	if (ImGui::ArrowButton("Up Arrow Button", ImGuiDir_Up)) {
		std::cout << "up arrow clicked\n";
	}

	ImGui::End();

	ImGui::ShowAboutWindow(&d1::open);

	ImGui::ShowDemoWindow(&d1::open);

	ImGui::ShowMetricsWindow(&d1::open);
	/** End window **/
}