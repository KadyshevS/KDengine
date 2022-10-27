#pragma once
#include "KDWin.h"
#include "KDException.h"

class Window
{
public:
	class Exception : public KDException
	{
		HRESULT hr;
	public:
		Exception( int line, const char* file, HRESULT hr ) noexcept;

		const char* what() const noexcept override;
		virtual const char* GetType() const noexcept override;
		static std::string TranslateErrorCode(HRESULT hr) noexcept;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
	};

private:
//	Window params
	int width;
	int height;
	HWND hWnd;

//	Register & cleanup window class
	class WindowClass
	{
		WindowClass() noexcept;
		WindowClass( const WindowClass& ) = delete;

		WindowClass& operator = (const WindowClass&) = delete;

		static constexpr const char* wndClassName = "KD Engine Direct3D Window";
		static WindowClass wndClass;
		HINSTANCE hInst;

		~WindowClass();

	public:
		static const char* GetName() noexcept;
		static HINSTANCE GetInst() noexcept;
	};

	static LRESULT CALLBACK HandleMsgSetup( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam ) noexcept;
	static LRESULT CALLBACK HandleMsgThunk( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam ) noexcept;
	LRESULT HandleMsg( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam ) noexcept;

public:
	Window( const int Width, const int Height, const char* Name );
	Window( const Window& ) = delete;
	Window& operator = ( const Window& ) = delete;

	~Window();
};

#define KDWND_EXCEPT( hr ) Window::Exception( __LINE__, __FILE__, hr )
#define KDWND_LAST_EXCEPT() Window::Exception( __LINE__, __FILE__, GetLastError() )