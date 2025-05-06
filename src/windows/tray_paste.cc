#include "keyboard_emulator.h"
#include <iostream>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam,
                            LPARAM lParam) {
  switch (uMsg) {
    case WM_HOTKEY:
      if (wParam == HOTKEY_ID) {
        paste_clipboard_text(KEY_PRESS_DELAY_MS);
      }
      break;
    case WM_DESTROY:
      PostQuitMessage(0);
      return 0;
    default:
      return DefWindowProc(hwnd, uMsg, wParam, lParam);
  }
  return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow) {
  const wchar_t CLASS_NAME[] = L"HiddenWindowClass";

  WNDCLASS wc = {0};
  wc.lpfnWndProc = WindowProc;
  wc.hInstance = hInstance;
  wc.lpszClassName = CLASS_NAME;

  if (!RegisterClass(&wc)) return 0;

  HWND hwnd =
      CreateWindowEx(0, CLASS_NAME, L"Tray Paste", WS_OVERLAPPEDWINDOW,
                     CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
                     nullptr,  // Parent window
                     nullptr, hInstance, nullptr);

  if (!hwnd) return 0;

  ShowWindow(hwnd, SW_HIDE);

  if (!RegisterHotKey(hwnd, HOTKEY_ID, MOD_CONTROL | MOD_ALT, static_cast<UINT>('V'))) {
    std::wcerr << L"Failed to register hotkey: " << GetLastError() << L"\r\n";
    return 0;
  }

  MSG msg;
  while (GetMessage(&msg, nullptr, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  return 0;
}
