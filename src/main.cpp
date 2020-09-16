#include "Platform/Platform.hpp"
#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include <iostream>
#include <array>

#include "demos.hpp"

/** Demo 2 Data **/
struct d2 {
	static Point p;
	static Rect r;
	static std::array<int, 4> arr1;
	static std::vector<int> arr2;
	static std::vector<std::string> comboVec;
	static std::vector<std::string> listVec;
	static int comboIndex;
	static int listIndex;
	static std::vector<char> textInputBuffer;
};

Point d2::p;
Rect  d2::r;
std::array<int, 4> d2::arr1 = { 0 };
std::vector<int> d2::arr2(4, 0);
std::vector<std::string> d2::comboVec = { "Camera Animation", "Circle Tween", "Easing Functions" };
int d2::comboIndex = 0;
std::vector<std::string> d2::listVec = { "Camera Animation", "Circle Tween", "Easing Functions" };
int d2::listIndex = 0;
std::vector<char> d2::textInputBuffer = {'t', 'e', 's', 't'};

/* --------------------------------------------------------------------------------
  Callback example 1: Change all input to A
  -------------------------------------------------------------------------------- */

// 1. first you define a free function
int callback(ImGuiTextEditCallbackData* data) {
	data->EventChar = 'A'; // makes every character fed to InputText an 'A'
	return 0;
}

std::array<char, 100> textArr = {' '};

const char replacementChar = 'Z';

void callbackExample() {

	// 2a. Then pass it to InputText
	ImGui::InputText("Text", textArr.data(), textArr.size(),
		ImGuiInputTextFlags_CallbackCharFilter, callback); // specify we're using a callbackCharFilter

	// 2b. You can also use stateless lambdas which can be used just as free functions
	ImGui::InputText("Text2", textArr.data(), textArr.size(),
		ImGuiInputTextFlags_CallbackCharFilter,
		[](ImGuiTextEditCallbackData* data) {
			data->EventChar = 'A';
			return 0;
		}
	);

	// 3. Suppose we want to use a char variable instead of a hard-code replacement
	//    We can store the pointer to a char in ImGuiEditCallbackData::UserData
	ImGui::InputText("Text3", textArr.data(), textArr.size(),
		ImGuiInputTextFlags_CallbackCharFilter,
		[](ImGuiTextEditCallbackData* data) {
			data->EventChar = *static_cast<const char *>(data->UserData);
			return 0;
		},
		const_cast<char*>(&replacementChar)); // cast away constness (store in data->UserData)
}

/* --------------------------------------------------------------------------------------
  Callback example 2: Defining a wrapper function that lets us pass lambdas with state
  --------------------------------------------------------------------------------------- */

// Now we can pass a lambda with state like this:
void callbackExample02(const char replacementCh) {

	if (ImGui::InputTextCool("Text4", textArr.data(), textArr.size(),
		ImGuiInputTextFlags_CallbackCharFilter,
		[&replacementCh](ImGuiTextEditCallbackData* data) {
			data->EventChar = replacementCh;
			std::cout << "Replaced with: " << replacementCh << "\n";
			return 0;
		})) {
			/* Do stuff when text changes */
	}
}

/* --------------------------------------------------------------------------------------
  Callback example 3: Generalising the approach
  (requires boost function_traits / template programming)
  // TODO: https://eliasdaler.github.io/using-imgui-with-sfml-pt2/
  --------------------------------------------------------------------------------------- */

/** End Demo 2 Data **/

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

void runDemo2(sf::RenderWindow&);
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

	window.setTitle(demo1.getWindowTitle());
    window.resetGLStates(); // call it if you only draw ImGui. Otherwise not needed.

	sf::Uint8 current_demo = 1;

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
			case 2:  runDemo2(window); break;
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

void runDemo2(sf::RenderWindow& window) {

	ImGui::Begin("Sample Window");

	// Using "##" to define unique labels but with same display string
	if (ImGui::Button("Same Label")) {
		std::cout << "Button 1 pushed" << std::endl;
	}
	if (ImGui::Button("Same Label##Second")) {
		std::cout << "Button 2 pushed" << std::endl;
	}

	// Using an array to define input boxes
	std::array<int, 4> arr = { 0 };

	for (int i = 0; i < arr.size(); ++i) {
		ImGui::PushID(i);
		ImGui::InputInt("##", &arr[i]);
		ImGui::PopID();
	}
	ImGui::End();

	// Create another window
	ImGui::Begin("Sample Window #2");

	if (ImGui::Button("Button")) {
		std::cout << "button clicked" << std::endl;
	}
	ImGui::End();

	// Append to existing window
	ImGui::Begin("Sample Window");

	if (ImGui::Button("Appended Button")) {
		std::cout << "appended button clicked" << std::endl;
	}

	// Using structs with POD - interpreted as array of floats
	if(ImGui::InputFloat2("Point", &d2::p.x)) {
		std::cout << "point changed" << std::endl;
	}

	// Using structs with POD - interpreted as array of floats
	if(ImGui::InputFloat4("Rect", &d2::r.x)) {
		std::cout << "rect changed" << std::endl;
	}

	if (ImGui::InputRect("Rect Custom", &d2::r)) {
		std::cout << "rect changed #2" << std::endl;
	}

	// call std::array::data() and std::vector::data() to pass the raw
	// pointer to the internal array to ImGui
	//ImGui::PushItemWidth(128.f);
	ImGui::InputInt4("STL Array", d2::arr1.data());
	ImGui::InputInt4("STL Vector", d2::arr2.data());

	// Use redefined callback to pass std::vector to ComboBox and ListBox
	ImGui::BeginGroup();
	ImGui::PushItemWidth(140.f);
	if (ImGui::Combo("Sample Combo", &d2::comboIndex, d2::comboVec)) {
		std::cout << "> combo change: " << d2::comboVec[d2::comboIndex] << "\n"; }
	ImGui::SameLine();

	ImGui::PushItemWidth(140.f);
	if (ImGui::ListBox("Sample List", &d2::listIndex, d2::listVec)) {
		std::cout << "> list change: " << d2::listVec[d2::listIndex] << "\n"; }
	ImGui::SameLine();
	ImGui::EndGroup();

	// InputText and std::string
	// Can pass vector<char> or array<char> to InputText and initialise string to vector.at()
	ImGui::InputText("Sample Input", d2::textInputBuffer.data(), d2::textInputBuffer.size() + 1);

	// Callback example
	callbackExample();

	// Callback example #2
	callbackExample02('R');

	ImGui::End();
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