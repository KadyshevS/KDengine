#include "Window.h"
#include "WndExcept.h"
#include "resource.h"
#include <sstream>
#include "imgui_impl_win32.h"

//	Window Class
Window::WindowClass Window::WindowClass::wndClass;
Window::WindowClass::WindowClass() noexcept
	:
	hInst( GetModuleHandle(nullptr) )
{
	WNDCLASSEXA wc = { 0 };
	wc.cbSize = sizeof( wc );
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = HandleMsgSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInst;
	wc.hIcon = static_cast<HICON>( LoadImage( GetInst(), MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 64, 64, 0 ) );
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = wndClassName;
	wc.hIconSm = static_cast<HICON>( LoadImage( hInst, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, 0 ) );
	
	RegisterClassEx(&wc);
}

const char* Window::WindowClass::GetName() noexcept
{
	return wndClassName;
}
HINSTANCE Window::WindowClass::GetInst() noexcept
{
	return wndClass.hInst;
}

Window::WindowClass::~WindowClass()
{
	UnregisterClass( wndClassName, GetInst() );
}

//	Window
Window::Window(const int Width, const int Height, const char* Name)
	:
	width(Width),
	height(Height)
{
//	Calculate Window size
	RECT wr;
	wr.left = 100;
	wr.right = Width + wr.left;
	wr.top = 100;
	wr.bottom = Height + wr.top;

//	Create window & get hWnd
	if ( AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE) == 0 )
	{
		KDWND_LAST_EXCEPT();
	}

	hWnd = CreateWindow(
		WindowClass::GetName(), Name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, wr.right - wr.left, wr.bottom - wr.top,
		nullptr, nullptr, WindowClass::GetInst(), this
	);

	if ( hWnd == nullptr )
	{
		KDWND_LAST_EXCEPT();
	}

//	Show window
	ShowWindow( hWnd, SW_SHOWDEFAULT );

//	Init Imgui
	ImGui_ImplWin32_Init( hWnd );

//	Make Graphics ptr
	gfx = std::make_unique<Graphics>( hWnd, width, height );

//	Register Mouse raw input device
	RAWINPUTDEVICE rid;
	rid.usUsagePage = 0x01; // mouse page
	rid.usUsage = 0x02; // mouse usage
	rid.dwFlags = 0;
	rid.hwndTarget = nullptr;
	if (RegisterRawInputDevices(&rid, 1, sizeof(rid)) == FALSE)
	{
		throw KDWND_LAST_EXCEPT();
	}
}

Window::~Window()
{
	ImGui_ImplWin32_Shutdown();
	DestroyWindow(hWnd);
}

void Window::ConfineCursor() noexcept
{
	RECT rect;
	GetClientRect( hWnd, &rect );
	MapWindowPoints( hWnd, nullptr, reinterpret_cast<POINT*>(&rect), 2 );
	ClipCursor( &rect );
}
void Window::FreeCursor() noexcept
{
	ClipCursor( nullptr );
}
void Window::EnableCursor() noexcept
{
	cursorEnabled = true;
	ShowCursor();
	EnableImguiMouse();
	FreeCursor();
}
void Window::DisableCursor() noexcept
{
	cursorEnabled = false;
	HideCursor();
	DisableImguiMouse();
	ConfineCursor();
}
bool Window::CursorEnabled() const noexcept
{
	return cursorEnabled;
}

void Window::EnableImguiMouse() noexcept
{
	ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
}
void Window::DisableImguiMouse() noexcept
{
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
}
void Window::HideCursor() noexcept
{
	while( ::ShowCursor( FALSE ) >= 0 );
}
void Window::ShowCursor() noexcept
{
	while( ::ShowCursor( TRUE ) < 0 );
}

LRESULT WINAPI Window::HandleMsgSetup(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	if (msg == WM_NCCREATE)
	{
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));

		return pWnd->HandleMsg(hwnd, msg, wParam, lParam);
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}
LRESULT WINAPI Window::HandleMsgThunk(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

	return pWnd->HandleMsg(hwnd, msg, wParam, lParam);
}
LRESULT Window::HandleMsg(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept
{
	if ( ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam) )
	{
		return true;
	}
	const auto imio = ImGui::GetIO();

	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	case WM_KILLFOCUS:
		kbd.ClearState();
		break;
	case WM_ACTIVATE:
		if (!cursorEnabled)
		{
			if (wParam & WA_ACTIVE)
			{
				ConfineCursor();
				HideCursor();
			}
			else
			{
				FreeCursor();
				ShowCursor();
			}
		}
		break;

		//	Keyboard Input
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		if (imio.WantCaptureKeyboard)
		{
			break;
		}
		if (!(lParam & 0x40000000) || kbd.AutorepeatIsEnabled())
		{
			kbd.OnKeyPressed(static_cast<unsigned char>(wParam));
		}
		break;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		kbd.OnKeyReleased(static_cast<unsigned char>(wParam));
		break;
	case WM_CHAR:
		if (imio.WantCaptureKeyboard)
		{
			break;
		}
		kbd.OnChar(static_cast<unsigned char>(wParam));
		break;

		//	Mouse Input
	case WM_MOUSEMOVE:
	{
		if (imio.WantCaptureKeyboard)
		{
			break;
		}
		const POINTS pt = MAKEPOINTS(lParam);
		if (pt.x >= 0 && pt.x < width && pt.y >= 0 && pt.y < height)
		{
			mouse.OnMouseMove(pt.x, pt.y);
			if (!mouse.IsInWindow())
			{
				SetCapture(hWnd);
				mouse.OnMouseEnter();
			}
		}
		else
		{
			if (wParam & (MK_LBUTTON | MK_RBUTTON))
			{
				mouse.OnMouseMove(pt.x, pt.y);
			}
			else
			{
				ReleaseCapture();
				mouse.OnMouseLeave();
			}
		}
		break;
	}
	case WM_LBUTTONDOWN:
	{
		SetForegroundWindow(hWnd);
		if (!cursorEnabled)
		{
			ConfineCursor();
			HideCursor();
		}
		if (imio.WantCaptureKeyboard)
		{
			break;
		}
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnLeftPressed(pt.x, pt.y);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		if (imio.WantCaptureKeyboard)
		{
			break;
		}
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnRightPressed(pt.x, pt.y);
		break;
	}
	case WM_LBUTTONUP:
	{
		if (imio.WantCaptureKeyboard)
		{
			break;
		}
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnLeftReleased(pt.x, pt.y);
		// release mouse if outside of window
		if (pt.x < 0 || pt.x >= width || pt.y < 0 || pt.y >= height)
		{
			ReleaseCapture();
			mouse.OnMouseLeave();
		}
		break;
	}
	case WM_RBUTTONUP:
	{
		if (imio.WantCaptureKeyboard)
		{
			break;
		}
		const POINTS pt = MAKEPOINTS(lParam);
		mouse.OnRightReleased(pt.x, pt.y);
		// release mouse if outside of window
		if (pt.x < 0 || pt.x >= width || pt.y < 0 || pt.y >= height)
		{
			ReleaseCapture();
			mouse.OnMouseLeave();
		}
		break;
	}
	case WM_MOUSEWHEEL:
	{
		if (imio.WantCaptureKeyboard)
		{
			break;
		}
		const POINTS pt = MAKEPOINTS(lParam);
		const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
		mouse.OnWheelDelta(pt.x, pt.y, delta);
		break;
	}
	//	Mouse Raw Messages
	case WM_INPUT:
	{
		if( !mouse.RawEnabled() )
		{
			break;
		}
		UINT size;
		// first get the size of the input data
		if (GetRawInputData(
			reinterpret_cast<HRAWINPUT>(lParam),
			RID_INPUT,
			nullptr,
			&size,
			sizeof(RAWINPUTHEADER)) == -1)
		{
			// bail msg processing if error
			break;
		}
		rawBuffer.resize(size);
		// read in the input data
		if (GetRawInputData(
			reinterpret_cast<HRAWINPUT>(lParam),
			RID_INPUT,
			rawBuffer.data(),
			&size,
			sizeof(RAWINPUTHEADER)) != size)
		{
			break;
		}
		auto& ri = reinterpret_cast<const RAWINPUT&>(*rawBuffer.data());
		if (ri.header.dwType == RIM_TYPEMOUSE &&
			(ri.data.mouse.lLastX != 0 || ri.data.mouse.lLastY != 0))
		{
			mouse.OnRawDelta(ri.data.mouse.lLastX, ri.data.mouse.lLastY);
		}
		break;
	}
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

void Window::SetTitle(const char* title)
{
	if ( SetWindowText(hWnd, title) == 0 )
	{
		throw KDWND_LAST_EXCEPT();
	}
}
void Window::SetTitle(const std::string& title)
{
	if ( SetWindowText(hWnd, title.c_str()) == 0 )
	{
		throw KDWND_LAST_EXCEPT();
	}
}

Graphics& Window::Gfx()
{
	if ( !gfx )
	{
		throw KDWND_NOGFX_EXCEPT();
	}
	return *gfx;
}
std::optional<int> Window::ProcessMessages()
{
	MSG msg;

	while ( PeekMessage( &msg, nullptr, 0, 0, PM_REMOVE ) )
	{
		if ( msg.message == WM_QUIT )
		{
			return (int)msg.wParam;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return {};
}

//	Window Exception
Window::Exception::Exception(int line, const char* file, HRESULT hr) noexcept
	:
	KDException( line, file ),
	hr( hr )
{}

const char* Window::Exception::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] " << GetErrorCode() << std::endl
		<< "[Descrition] " << GetErrorDescription() << std::endl
		<< GetOriginString();

	whatBuffer = oss.str();
	return whatBuffer.c_str();
}
const char* Window::Exception::GetType() const noexcept
{
	return "Window Exception";
}
std::string Window::Exception::TranslateErrorCode( HRESULT hr ) noexcept
{
	char* pMsgBuf = nullptr;
	DWORD nMsgLen = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, hr, MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
		reinterpret_cast<LPSTR>(&pMsgBuf), 0, nullptr
	);

	if ( nMsgLen == 0 )
	{
		return "Unidentified Error Code";
	}
	std::string errorString = pMsgBuf;
	LocalFree( pMsgBuf );

	return errorString;
}
HRESULT Window::Exception::GetErrorCode() const noexcept
{
	return hr;
}
std::string Window::Exception::GetErrorDescription() const noexcept
{
	return TranslateErrorCode( hr );
}

const char* Window::NoGfxException::GetType() const noexcept
{
	return "";
}
