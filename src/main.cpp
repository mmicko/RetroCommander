/*
 * Copyright 2015 Miodrag Milanovic. All rights reserved.
 * License: http://www.opensource.org/licenses/BSD-3-Clause
 */

#include "common.h"
#include <bgfx.h>
#include <bx/uint32_t.h>

int _main_(int /*_argc*/, char** /*_argv*/)
{
	uint32_t width = 1280;
	uint32_t height = 720;
	uint32_t debug = BGFX_DEBUG_TEXT;
	uint32_t reset = BGFX_RESET_VSYNC;

	bgfx::init();
	bgfx::reset(width, height, reset);

	// Enable debug text.
	bgfx::setDebug(debug);

	// Set view 0 clear state.
	bgfx::setViewClear(0
		, BGFX_CLEAR_COLOR|BGFX_CLEAR_DEPTH
		, 0x303030ff
		, 1.0f
		, 0
		);

	while (!entry::processEvents(width, height, debug, reset) )
	{
		bgfx::setViewRect(0, 0, 0, width, height);

		bgfx::submit(0);

		bgfx::frame();
	}

	// Shutdown bgfx.
	bgfx::shutdown();

	return 0;
}
