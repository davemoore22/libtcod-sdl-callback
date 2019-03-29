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

#include "renderer.hpp"

// Default Constructor
Renderer::Renderer(SDL_Surface* image_to_display): _image_to_display(image_to_display)
{
}

// Default Destructor
Renderer::~Renderer()
{
}

// Render Method
auto Renderer::render(void* SDL_surface) -> void
{
	// Work out where to display the image and how much to resize it to take up the other half of the screen
	int font_width = {};
	int font_height = {};
	TCODSystem::getCharSize(&font_width, &font_height);
	SDL_Rect image_destination = {102 * font_width, 1 * font_height, 100 * font_width, 71 * font_height};

	// Since custom rendering is done last, set a transparent region to show libtcod text underneath
	SDL_Rect hole = {152 * font_width, 2 * font_height, 100 * font_width, 10 * font_height};
	set_transparent_area(_image_to_display, hole);

	// Get access to the surface
	SDL_Surface* screen = (SDL_Surface*) SDL_surface;
	SDL_SetSurfaceBlendMode(screen, SDL_BLENDMODE_BLEND);
	SDL_BlitScaled(_image_to_display, nullptr, screen, &image_destination);

	// And set the Dirty Area to cover what we've just done
	TCODConsole::root->setDirty(102, 1, 100, 71);
}

// Set a transparent region
auto Renderer::set_transparent_area(SDL_Surface *surface, SDL_Rect transparent_rect) -> void
{
	// Get access to pixels of surface
	SDL_LockSurface(_image_to_display);
	SDL_Color rgba;
	for (int x = transparent_rect.x; x < transparent_rect.x + transparent_rect.w; x++) {
		for (int y = transparent_rect.y; y < transparent_rect.y + transparent_rect.h; y++) {

			// Get the pixel colour, adjust the transparency and update
			Uint32 source_pixel = get_pixel(_image_to_display, x, y);
			SDL_GetRGBA(source_pixel, _image_to_display->format, &rgba.r, &rgba.g, &rgba.b, &rgba.a);
			Uint32 transparent_pixel = SDL_MapRGBA(_image_to_display->format, rgba.r, rgba.g, rgba.b, 0x00);
			set_pixel(_image_to_display, x, y, transparent_pixel);
		}
	}
	SDL_UnlockSurface(_image_to_display);
}

// Code from https://stackoverflow.com/questions/53033971/how-to-get-the-color-of-a-specific-pixel-from-sdl-surface
auto Renderer::get_pixel(SDL_Surface *surface, int x, int y) -> Uint32
{
    int bpp = surface->format->BytesPerPixel;
    Uint8 *p = reinterpret_cast<Uint8*>(surface->pixels) + y * surface->pitch + x * bpp;

	switch (bpp) {
	case 1:
		return *p;
		break;
	case 2:
		return *reinterpret_cast<Uint16*>(p);
		break;
	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
			return p[0] << 16 | p[1] << 8 | p[2];
		else
			return p[0] | p[1] << 8 | p[2] << 16;
		break;
	case 4:
		return *reinterpret_cast<Uint32*>(p);
		break;
	default:
		return 0;
	}
}

// Source from pygame/src/getset.h (https://bitbucket.org/gindar/pgext)
auto Renderer::set_pixel(SDL_Surface* surface, int x, int y, Uint32 color) -> void
{
    SDL_PixelFormat *format = surface->format;
    Uint8 *pixels = reinterpret_cast<Uint8*>(surface->pixels);
    Uint8 *byte_buf;

    if(format->BytesPerPixel == 3) {
        byte_buf = reinterpret_cast<Uint8*>((pixels + y * surface->pitch) + x * 3);
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
        *(byte_buf + 2 - (format->Rshift >> 3)) = static_cast<Uint8>(color >> 16);
        *(byte_buf + 2 - (format->Gshift >> 3)) = static_cast<Uint8>(color >> 8);
        *(byte_buf + 2 - (format->Bshift >> 3)) = static_cast<Uint8>(color);
#else
        *(byte_buf + (format->Rshift >> 3)) = (Uint8) (color >> 16);
        *(byte_buf + (format->Gshift >> 3)) = (Uint8) (color >> 8);
        *(byte_buf + (format->Bshift >> 3)) = (Uint8) color;
#endif
    } else if (format->BytesPerPixel == 4)
        *(reinterpret_cast<Uint32*>((pixels + y * surface->pitch)) + x) = color;
}


