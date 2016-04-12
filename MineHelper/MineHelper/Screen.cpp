#include "stdafx.h"
#include "Screen.h"

using namespace std;

HWND hwndTarget = 0;

shared_ptr<RawBitmap> Screen::GetScreenBitmap() {
  if (!hwndTarget) {
    ::EnumWindows(
        [](HWND hwnd, LPARAM lParam) -> BOOL {
          LPTSTR lpString = (LPTSTR)VirtualAlloc(
              (LPVOID)NULL, (DWORD)(2000 + 2), MEM_COMMIT, PAGE_READWRITE);
          ::GetWindowTextW(hwnd, lpString, 1000);
          wstring sWindowText = wstring((wchar_t *)(lpString));
          const wregex re_chrome(_T("(.i.e.ie...........a..i.e....u..i..a.e..O..i.e..i.e..ee.e.)"));
          wsmatch match;
          if (regex_search(sWindowText, match, re_chrome) && match.size() > 1) {
            hwndTarget = hwnd;
            return false; // stop iteration
          } else
            return true; // continue
        },
        0L);
  }

  shared_ptr<RawBitmap> result(new RawBitmap());
  if (!hwndTarget)
    goto done;

  HDC hdcWindow;
  HDC hdcMemDC = NULL;
  HBITMAP hbmWindow = NULL;

  hdcWindow = GetDC(hwndTarget);
  hdcMemDC = CreateCompatibleDC(hdcWindow);
  if (!hdcMemDC)
    goto done;

  RECT rcClient;
  GetClientRect(hwndTarget, &rcClient);
  if (rcClient.right == rcClient.left)
    goto done;

  result->Clear(rcClient.right - rcClient.left, rcClient.bottom - rcClient.top);

  hbmWindow = CreateCompatibleBitmap(hdcWindow, result->Width, result->Height);
  if (!hbmWindow)
    goto done;

  HGDIOBJ hOld = SelectObject(hdcMemDC, hbmWindow);
  if (!BitBlt(hdcMemDC, 0, 0, result->Width, result->Height, hdcWindow, 0, 0,
              SRCCOPY))
    goto done;

  BITMAPINFOHEADER bi;
  bi.biBitCount = 32;
  bi.biClrImportant = 0;
  bi.biClrUsed = 0;
  bi.biCompression = BI_RGB;
  bi.biHeight = result->Height;
  bi.biPlanes = 1;
  bi.biSize = sizeof(BITMAPINFOHEADER);
  bi.biSizeImage = 0;
  bi.biWidth = result->Width;
  bi.biXPelsPerMeter = 0;
  bi.biYPelsPerMeter = 0;

  GetDIBits(hdcWindow, hbmWindow, 0, result->Height, result->bits.get(),
            (BITMAPINFO *)&bi, DIB_RGB_COLORS);

  SelectObject(hdcMemDC, hOld);

done:
  DeleteObject(hbmWindow);
  DeleteObject(hdcMemDC);
  ReleaseDC(hwndTarget, hdcWindow);

  return result;
}
