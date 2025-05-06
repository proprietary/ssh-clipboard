#include "ssh_clipboard/

#define HOTKEY_ID 1       // ID for our registered hotkey.
#define PASTE_HOTKEY 'V'  // The key to trigger (here: Ctrl + Alt + V).
#define KEY_PRESS_DELAY_MS \
  50  // Delay (in milliseconds) between each character.

void send_unicode_char(wchar_t ch) {
  INPUT input[2] = {0};

  input[0].type = INPUT_KEYBOARD;
  input[0].ki.wScan = ch;
  input[0].ki.dwFlags = KEYEVENTF_UNICODE;

  input[1].type = INPUT_KEYBOARD;
  input[1].ki.wScan = ch;
  input[1].ki.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_KEYUP;

  // Send the two events.
  SendInput(2, input, sizeof(INPUT));
}

// This function reads the clipboard text (as Unicode) and types it out letter
// by letter.
void paste_clipboard_text(DWORD delay) {
  // Open the clipboard (NULL means current task).
  if (OpenClipboard(nullptr)) {
    // Get clipboard data in the CF_UNICODETEXT format.
    HANDLE hData = GetClipboardData(CF_UNICODETEXT);
    if (hData) {
      // Lock the global memory handle to get a pointer to the data.
      wchar_t* pszText = static_cast<wchar_t*>(GlobalLock(hData));
      if (pszText) {
        // For each character in the Unicode string until the null terminator...
        for (size_t i = 0; pszText[i] != L'\0'; i++) {
          SendUnicodeChar(
              pszText[i]);  // Emulate the key press for this character.
          Sleep(delay);     // Wait for the configurable delay.
        }
        GlobalUnlock(hData);
      }
    }
    CloseClipboard();
  }
}
