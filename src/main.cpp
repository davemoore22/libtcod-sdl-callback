// Copyright 2019 Dave Moore
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
// documentation files (the "Software"), to deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the
// Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include "main.hpp"
#include "renderer.hpp"

auto CSTR(const std::string& string_to_convert) -> const char*
{
	return !string_to_convert.empty() ? (char*)string_to_convert.c_str() : (char*)"";
}

auto main(int argc, char* args[]) -> int
{
	// Setup Screen
	TCODConsole::initRoot(203, 80, "Libtcod SDL Callback Demo", false, TCOD_RENDERER_SDL);
	TCODSystem::setFps(10);
	std::string version = {"Powered by Libtcod " + std::to_string(TCOD_MAJOR_VERSION) + "." +
		std::to_string(TCOD_MINOR_VERSION) + "." + std::to_string(TCOD_PATCHLEVEL)};

	// Get Image (source image - https://upload.wikimedia.org/wikipedia/commons/5/58/Sunset_2007-1.jpg)
	const std::filesystem::path sunset_image_path = {std::filesystem::current_path() / "sunset.png"};

	// Load image into libtcod and scale for screen
	const auto sunset_image_libtcod = std::make_unique<TCODImage>(CSTR(sunset_image_path.string()));
    sunset_image_libtcod->scale(200, 142);

    // Load pre-scaled image into SDL and convert to an value with Alpha Transparency
	SDL_Surface* sunset_image_SDL = IMG_Load(CSTR(sunset_image_path.string()));
	if (sunset_image_SDL == nullptr)
		std::cout << "SDL Error: " << IMG_GetError() << std::endl;
	SDL_Surface *sunset_image_SDL_alpha = SDL_ConvertSurfaceFormat(sunset_image_SDL, SDL_PIXELFORMAT_RGBA8888, 0);
	if (sunset_image_SDL_alpha == nullptr)
		std::cout << "SDL Error: " << IMG_GetError() << std::endl;

	// Clear libtcod console
	TCODConsole::root->setDefaultBackground(TCODColor::black);
	TCODConsole::root->clear();

	// Display text
	TCODConsole::root->setDefaultForeground(TCODColor::yellow);
	TCODConsole::root->printf(2, 74, "Libtcod SDL Callback Example (C) 2019 Dave Moore");
	TCODConsole::root->setDefaultForeground(TCODColor::orange);
	TCODConsole::root->printf(2, 76, "davemoore22@gmail.com");
	TCODConsole::root->setDefaultForeground(TCODColor::red);
	TCODConsole::root->printf(2, 78, "Code released under MIT License");
	TCODConsole::root->setDefaultForeground(TCODColor::cyan);
	TCODConsole::root->printf(102, 74, "Left: libtcod.blit2x");
	TCODConsole::root->printf(152, 74, "Right: SDL_BlitScaled");
	TCODConsole::root->setDefaultForeground(TCODColor::white);
	TCODConsole::root->printf(102, 76, "Press ESCAPE to quit");
	TCODConsole::root->setDefaultForeground(TCODColor::silver);
	TCODConsole::root->print(102, 78, version);

	// And underneath the image text to demonstrate transparency
	TCODConsole::root->setDefaultForeground(TCODColor::white);
	TCODConsole::root->printf(150, 2, "Surface Manipulation");
	TCODConsole::root->printf(148, 3, "Transparent SDL Blitting");

	// Enable SDL Renderer
	TCODSystem::registerSDLRenderer(new Renderer(sunset_image_SDL_alpha));
	TCOD_key_t key_pressed = {};
	while (true) {

		// Get keypress and abort if ESCAPE
		TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &key_pressed, nullptr);
		if (key_pressed.vk == TCODK_ESCAPE)
			break;

		// Display Image using libtcod on one half of the screen (the SDL renderer will display the same image on the
		// other half)
		sunset_image_libtcod->blit2x(TCODConsole::root, 1, 1);


		// Refresh display
		TCODConsole::root->flush();
	}

	// Disable SDL Renderer
	TCODSystem::registerSDLRenderer(NULL);

	// And quit gracefully
	SDL_FreeSurface(sunset_image_SDL_alpha);
	SDL_FreeSurface(sunset_image_SDL);
	TCOD_quit();
}
