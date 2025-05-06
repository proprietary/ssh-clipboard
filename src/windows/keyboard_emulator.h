#ifndef _INCLUDE_SSH_CLIPBOARD_WINDOWS_KEYBOARD_EMULATOR_H
#define _INCLUDE_SSH_CLIPBOARD_WINDOWS_KEYBOARD_EMULATOR_H
#include <tchar.h>
#include <windows.h>

void send_unicode_char(wchar_t ch);
void paste_clipboard_text(DWORD delay);

#endif  // _INCLUDE_SSH_CLIPBOARD_WINDOWS_KEYBOARD_EMULATOR_H
