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

#pragma once

#include "main.hpp"

// SDL Callback Renderer
class Renderer: public ITCODSDLRenderer
{
public:

	// Default Constructor
	Renderer(SDL_Surface* image_to_display);

	// Default Destructor
	~Renderer();

private:

	// Render Method
	auto render(void *SDL_surface) -> void;

	// Pixel Manipulation
	auto get_pixel(SDL_Surface *surface, int x, int y) -> Uint32;
	auto set_pixel(SDL_Surface* surface, int x, int y, Uint32 color) -> void;

	// Set a transparent region
	auto set_transparent_area(SDL_Surface *surface, SDL_Rect transparent_rect) -> void;

	// Somewhere to store the image to display
	SDL_Surface* _image_to_display;
};
