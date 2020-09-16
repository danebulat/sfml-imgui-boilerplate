#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics.hpp>

#include <iostream>
#include <vector>
#include <array>

struct Rect {
	float x;
	float y;
	float w;
	float h;
};

struct Point {
	float x;
	float y;
};

namespace ImGui
{

/*----------------------------------------------------------------------
 Function to create float inputs for a Rect
 ----------------------------------------------------------------------*/
bool InputRect(const char* label,
			Rect* rect,
			int decimal_precision = -1,
			ImGuiInputTextFlags extra_flags = 0) {

	ImGui::PushID(label);	// id for group
	ImGui::BeginGroup();	// start group

	bool value_changed = false;
	std::array<float*, 4> arr = { &rect->x, &rect->y, &rect->w, &rect->h };

	for (auto& elem : arr) {
		ImGui::PushID(elem);			// id for InputFloat
		ImGui::PushItemWidth(64.f);		// width for InputFloat
		value_changed |= ImGui::InputFloat("##arr", elem, 0, 0,
			decimal_precision, extra_flags);
		ImGui::PopID();					// pop id for InputFloat
		ImGui::SameLine();				// same line as previous element
	}

	//ImGui::SameLine(); /* makes no difference */
	ImGui::TextUnformatted(label);

	ImGui::EndGroup();
	ImGui::PopID();

	return value_changed;
}

/*----------------------------------------------------------------------
 Static lambda and overridden functions to enable STL arrays and vectors
 of strings to populate an ImGui::ListBox and ImGui::Combo
 ----------------------------------------------------------------------*/

/** Lambda that sets the out text for the current item in ListBox or Combo
 *  based on data from a vector<string> object.
 *
 * Takes an array of data and index value.
 * Function is required to set `out_text` and return true or false.
 */
static auto vector_getter = [](void* vec, int idx, const char** out_text) {

	// cast `void* vec` parameter back to vector<string*>* and re-reference
	auto& vector = *static_cast<std::vector<std::string>*>(vec);

	// return false if idx is out of bounds
	if (idx < 0 || idx > static_cast<int>(vector.size())) { return false; }

	// set `out_text*` buffer to const char* stored in string at idx in vector
	*out_text = vector.at(idx).c_str();

	return true;
};

/** Overridden Combo function to accept std::vector<std::string>&
 */
bool Combo(const char* label,                   // widget label
		   int* current_item,                   // start index of item to draw
		   std::vector<std::string>& values) {  // data vector

	if (values.empty()) { return false; }		// return false if vector is empty

	// Draw ImGui::Combo widget
	return Combo(label, 						// widget label
				 current_item, 					// start index of item to draw
				 vector_getter,					// getter function bool(*items_getter)(void* data, int idx, const char* out_text)
				 static_cast<void*>(&values),	// &vector array cast to void*
				 values.size());				// number of items to draw to combo
}

/** Overridden ListBox function to accept std::vector<std::string>&
 */
bool ListBox(const char* label,						// widget label
			 int* current_item,					    // start index of item to draw
			 std::vector<std::string>& values) {	// data vector

	if (values.empty()) { return false; }

	return ListBox(label,						// widget label
				   current_item,				// start index of item to draw
				   vector_getter,				// getter function bool(*items_getter)(void* data, int idx, const char* out_text)
				   static_cast<void*>(&values),	// &vector cast to void*
				   values.size());			    // number of items to draw to list box
}

/*----------------------------------------------------------------------
  Wrapper function that lets us pass a lambda with state to and
  InputText callback function
  ----------------------------------------------------------------------*/
template <typename F>
bool InputTextCool(const char* label,			// widget label
			   char* buf,						// InputText buffer
			   size_t buf_size,					// size of buffer
			   ImGuiInputTextFlags flags = 0,	// InputText flags
			   F callback = nullptr,			// callback when data changes
			   void* user_data = nullptr) {		// user data passed to callback

	// stateless callback to pass to ImGui::InputText
	auto free_callback = [](ImGuiTextEditCallbackData* data) {
		auto& f = *static_cast<F*>(data->UserData);
		return f(data);		// call lambda with state
	};

	return ImGui::InputText(label,				// widget label
							buf,				// InputText buffer
							buf_size,			// size of buffer
							flags,				// InputText flags
							free_callback,		// stateless callback
							&callback);			// state callback (called in free_callback)
}

}// namespace ImGui

/*----------------------------------------------------------------------
  Demo1 class
  ----------------------------------------------------------------------*/
class Demo1 {
private:
	char      m_windowTitle[255];
	sf::Color m_bg_color;
	int       m_integer;
	bool      m_open;

	float     m_pos[2] = { 10.f, 20.f };
	float     m_color[3] = { 0.f, 0.f, 0.f };

public:
	Demo1()
		: m_windowTitle("Hello, ImGui")
		, m_bg_color(sf::Color::Black)
		, m_integer(32)
		, m_open(true)
	{}

	sf::Color getBackgroundColor() const {
		return m_bg_color;
	}

	const char* getWindowTitle() const {
		return m_windowTitle;
	}

	void run(sf::RenderWindow& window) {
		/** Begin window **/
		ImGui::Begin("Sample window");

		// Background color edit
		if (ImGui::ColorEdit3("Background color", m_color)) {

			// Output changed color to console
			std::cout << "color[r, g, b] = " << m_color[0] << ' ' << m_color[1] << ' ' << m_color[2] << std::endl;

			// this code gets called if color value changes, so
			// the background color is upgraded automatically
			m_bg_color.r = static_cast<sf::Uint8>(m_color[0] * 255.f);
			m_bg_color.g = static_cast<sf::Uint8>(m_color[1] * 255.f);
			m_bg_color.b = static_cast<sf::Uint8>(m_color[2] * 255.f);
		}

		// Creates a text edit with "Window title" label
		// Pass in windowTitle[] char array as data source for text edit
		if (ImGui::InputText("Window title", m_windowTitle, 255)) {
			std::cout << "new windowTitle[] = " << m_windowTitle << std::endl;
		}

		// Create a button with label "Update window title"
		if (ImGui::Button("Update window title")) {
			// this code gets if user clicks on the button
			// you could have written if(ImGui::InputText(...))
			// but this shows how buttons work
			window.setTitle(m_windowTitle);
		}

		if (ImGui::InputInt("Integer value", &m_integer)) {
			std::cout << "new integer: " << m_integer << std::endl;
		}

		if (ImGui::InputFloat2("position", m_pos)) {
			std::cout << "new pos: " << m_pos[0] << ',' << m_pos[1] << std::endl;
		}

		if (ImGui::ArrowButton("Left Arrow Button", ImGuiDir_Left)) {
			std::cout << "left arrow clicked\n";
		}

		if (ImGui::ArrowButton("Up Arrow Button", ImGuiDir_Up)) {
			std::cout << "up arrow clicked\n";
		}

		ImGui::End();
		/** End window **/

		ImGui::ShowAboutWindow(&m_open);
		ImGui::ShowDemoWindow(&m_open);
		ImGui::ShowMetricsWindow(&m_open);
	}
};