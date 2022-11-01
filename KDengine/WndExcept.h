#pragma once

#define KDWND_EXCEPT( hr ) Window::Exception( __LINE__, __FILE__, hr )
#define KDWND_LAST_EXCEPT() Window::Exception( __LINE__, __FILE__, GetLastError() )
#define KDWND_NOGFX_EXCEPT() Window::NoGfxException( __LINE__, __FILE__ )
