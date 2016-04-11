#include "stdafx.h"
#include "Screen.h"

HWND hwndTarget = 0;

Screen::Screen() {}

Screen::~Screen() {}

void Screen::GetScreenBitmap() {
  if (!hwndTarget) {
    ::EnumWindows(
        [](HWND hwnd, LPARAM lParam) -> BOOL {
          LPTSTR lpString = (LPTSTR)VirtualAlloc(
              (LPVOID)NULL, (DWORD)(1000 + 1), MEM_COMMIT, PAGE_READWRITE);
          DWORD dwProcessId;
          ::GetWindowTextW(hwnd, lpString, 1000);
          std::wstring sWindowText = std::wstring((wchar_t *)(lpString));
          std::wcout << sWindowText << L"\r\n";
          const std::wregex re_chrome(_T("(Minefield, HTML5 Massively ")
                                      _T("Multiplayer Online Minesweeper)"));
          std::wsmatch match;
          if (std::regex_search(sWindowText, match, re_chrome) &&
              match.size() > 1) {
            hwndTarget = hwnd;
            return false; // stop iteration
          } else
            return true; // continue
        },
        0L);
  }

  CaptureAnImage(hwndTarget);
}

int Screen::CaptureAnImage(HWND hWnd) {
  HDC hdcWindow;
  HDC hdcMemDC = NULL;
  HBITMAP hbmScreen = NULL;
  BITMAP bmpScreen;

  // Retrieve the handle to a display device context for the client
  // area of the window.
  hdcWindow = GetDC(hWnd);

  // Create a compatible DC which is used in a BitBlt from the window DC
  hdcMemDC = CreateCompatibleDC(hdcWindow);

  if (!hdcMemDC) {
    MessageBox(hWnd, L"CreateCompatibleDC has failed", L"Failed", MB_OK);
    goto done;
  }

  // Get the client area for size calculation
  RECT rcClient;
  GetClientRect(hWnd, &rcClient);

  // Create a compatible bitmap from the Window DC
  hbmScreen = CreateCompatibleBitmap(hdcWindow, rcClient.right - rcClient.left,
                                     rcClient.bottom - rcClient.top);

  if (!hbmScreen) {
    MessageBox(hWnd, L"CreateCompatibleBitmap Failed", L"Failed", MB_OK);
    goto done;
  }

  // Select the compatible bitmap into the compatible memory DC.
  SelectObject(hdcMemDC, hbmScreen);

  // Bit block transfer into our compatible memory DC.
  if (!BitBlt(hdcMemDC, 0, 0, rcClient.right - rcClient.left,
              rcClient.bottom - rcClient.top, hdcWindow, 0, 0, SRCCOPY)) {
    MessageBox(hWnd, L"BitBlt has failed", L"Failed", MB_OK);
    goto done;
  }

  // Get the BITMAP from the HBITMAP
  GetObject(hbmScreen, sizeof(BITMAP), &bmpScreen);

  BITMAPFILEHEADER bmfHeader;
  BITMAPINFOHEADER bi;

  bmpScreen.GetPixel

  bi.biSize = sizeof(BITMAPINFOHEADER);
  bi.biWidth = bmpScreen.bmWidth;
  bi.biHeight = bmpScreen.bmHeight;
  bi.biPlanes = 1;
  bi.biBitCount = 32;
  bi.biCompression = BI_RGB;
  bi.biSizeImage = 0;
  bi.biXPelsPerMeter = 0;
  bi.biYPelsPerMeter = 0;
  bi.biClrUsed = 0;
  bi.biClrImportant = 0;

  DWORD dwBmpSize =
      ((bmpScreen.bmWidth * bi.biBitCount + 31) / 32) * 4 * bmpScreen.bmHeight;

  // Starting with 32-bit Windows, GlobalAlloc and LocalAlloc are implemented
  // as wrapper functions that
  // call HeapAlloc using a handle to the process's default heap. Therefore,
  // GlobalAlloc and LocalAlloc
  // have greater overhead than HeapAlloc.
  HANDLE hDIB = GlobalAlloc(GHND, dwBmpSize);
  char *lpbitmap = (char *)GlobalLock(hDIB);

  // Gets the "bits" from the bitmap and copies them into a buffer
  // which is pointed to by lpbitmap.
  GetDIBits(hdcWindow, hbmScreen, 0, (UINT)bmpScreen.bmHeight, lpbitmap,
            (BITMAPINFO *)&bi, DIB_RGB_COLORS);

  // A file is created, this is where we will save the screen capture.
  HANDLE hFile = CreateFile(L"captureqwsx.bmp", GENERIC_WRITE, 0, NULL,
                            CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

  // Add the size of the headers to the size of the bitmap to get the total
  // file size
  DWORD dwSizeofDIB =
      dwBmpSize + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

  // Offset to where the actual bitmap bits start.
  bmfHeader.bfOffBits =
      (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER);

  // Size of the file
  bmfHeader.bfSize = dwSizeofDIB;

  // bfType must always be BM for Bitmaps
  bmfHeader.bfType = 0x4D42; // BM

  DWORD dwBytesWritten = 0;
  WriteFile(hFile, (LPSTR)&bmfHeader, sizeof(BITMAPFILEHEADER), &dwBytesWritten,
            NULL);
  WriteFile(hFile, (LPSTR)&bi, sizeof(BITMAPINFOHEADER), &dwBytesWritten, NULL);
  WriteFile(hFile, (LPSTR)lpbitmap, dwBmpSize, &dwBytesWritten, NULL);

  // Unlock and Free the DIB from the heap
  GlobalUnlock(hDIB);
  GlobalFree(hDIB);

  // Close the handle for the file that was created
  CloseHandle(hFile);

// Clean up
done:
  DeleteObject(hbmScreen);
  DeleteObject(hdcMemDC);
  ReleaseDC(hWnd, hdcWindow);

  return 0;
}