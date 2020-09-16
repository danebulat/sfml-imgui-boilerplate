#include "Platform/Platform.hpp"
#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include <iostream>
#include <array>

#include "demos.hpp"

/** Demo 3 **/
struct d3 {
	static bool show_window;
	static char buf[100];
	static float f;
	static float f2[2];
	static bool my_tool_active;
	static float my_color[4];
};

bool d3::show_window = true;
char d3::buf[100] = {'\0'};
float d3::f = 0.f;
float d3::f2[2] = {.0f};
bool d3::my_tool_active = true;
float d3::my_color[4] = {0.f};
/** End Demo 3 **/

void runDemo3(sf::RenderWindow&);

int main()
{
	util::Platform platform;

	sf::RenderWindow window(sf::VideoMode(800, 600), "");
    window.setVerticalSyncEnabled(true);

	// Initialize ImGui. Internal font atlas is created
	// (see ImGui-SFML README on how to use other fonts)
    ImGui::SFML::Init(window);

	sf::Clock deltaClock;

	Demo1 demo1;
	Demo2 demo2;

	window.setTitle(demo1.getWindowTitle());
    window.resetGLStates(); // call it if you only draw ImGui. Otherwise not needed.

	sf::Uint8 current_demo = 2;

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

		switch (current_demo) {
			case 1:  demo1.run(window); break;
			case 2:  demo2.run(window); break;
			case 3:  runDemo3(window); break;
			default: demo1.run(window); break;
		}

        window.clear(demo1.getBackgroundColor()); // fill background with color

		// render ImGui windows
        ImGui::SFML::Render(window);

        window.display();
    }

	// clean up ImGui, such as deleting the internal font atlas
    ImGui::SFML::Shutdown();

	return 0;
}

void runDemo3(sf::RenderWindow& window) {

	// Hello, World Sample window
	if(d3::show_window)
    {
		if (!ImGui::Begin("Sample Window", &d3::show_window)) {
			ImGui::End();
		}
		else {
			ImGui::Text("Hello, world %d", 123);
			if (ImGui::Button("Save")) {
				std::cout << "Call MySaveFunction()\n";
			}
			ImGui::InputText("string", d3::buf, IM_ARRAYSIZE(d3::buf));
			ImGui::SliderFloat("float", &d3::f, 0.f, 1.f);
			ImGui::SliderFloat2("float2", d3::f2, 0.f, 1.f);

			ImGui::End();
		}
	}

	// Create a window called "My First Tool" with a menu bar
	ImGui::Begin("My First Tool", &d3::my_tool_active, ImGuiWindowFlags_MenuBar);
	if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Open..", "Ctrl+O")) {  std::cout << "> Open()\n"; }
			if (ImGui::MenuItem("Save", "Ctrl+S")) { std::cout << "> Save()\n"; }
			if (ImGui::MenuItem("Close", "Ctrl+W")) { std::cout << "> Close()\n"; }
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	// Button to show other sample window
	if(ImGui::Button("Toggle Sample Window")) {
		d3::show_window = !d3::show_window;
	}

	// Edit a color (stored as ~4 floats)
	ImGui::ColorEdit4("Color", d3::my_color);

	// Plot some values
	const float my_values[] = { .2f, .1f, 1.f, .5f, .9f, 2.2f };
	ImGui::PlotLines("Frame Times", my_values, IM_ARRAYSIZE(my_values));

	// Display contents in a scrolling region
	ImGui::TextColored(ImVec4(1,1,0,1), "Important Stuff");
	ImGui::BeginChild("Scrolling");
	for (int n = 0; n < 50; ++n) {
		ImGui::Text("%04d: Some text", n);
	}
	ImGui::EndChild();
	ImGui::End();
}