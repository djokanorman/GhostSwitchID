#include <windows.h>
#include <shellapi.h>
#include <vector>
#include <string>
#include "RtMidi.h"

#define WM_TRAYICON (WM_USER + 1)
#define ID_TRAY_EXIT 1001
#define ID_TRAY_ENABLE 1002
#define ID_TRAY_DISABLE 1003
#define ID_TRAY_STARTUP 1004

HINSTANCE hInst;
NOTIFYICONDATA nid = {};
bool midiEnabled = true;
RtMidiOut* midiout = nullptr;

std::wstring GetIconPath() {
    wchar_t exePath[MAX_PATH];
    GetModuleFileName(NULL, exePath, MAX_PATH);
    std::wstring exeDir(exePath);
    size_t pos = exeDir.find_last_of(L"\\/");
    if (pos != std::wstring::npos) {
        exeDir = exeDir.substr(0, pos);
    }
    return exeDir + L"\\GhostSwitchID.ico";
}

void SendMidi(unsigned char cc) noexcept {
    if (!midiEnabled || !midiout) return;
    std::vector<unsigned char> message = { 0xB0, cc, 127 };
    midiout->sendMessage(&message);
}

void ToggleStartup(bool enable) {
    HKEY hKey;
    const wchar_t* runPath = L"Software\\Microsoft\\Windows\\CurrentVersion\\Run";
    const wchar_t* appName = L"GhostSwitchID";
    wchar_t exePath[MAX_PATH];
    GetModuleFileName(NULL, exePath, MAX_PATH);

    std::wstring quotedPath = L"\"" + std::wstring(exePath) + L"\"";
    DWORD dataSize = static_cast<DWORD>(sizeof(wchar_t) * (quotedPath.length() + 1));

    if (RegOpenKeyEx(HKEY_CURRENT_USER, runPath, 0, KEY_WRITE, &hKey) == ERROR_SUCCESS) {
        if (enable) {
            RegSetValueEx(hKey, appName, 0, REG_SZ, (BYTE*)quotedPath.c_str(), dataSize);
        }
        else {
            RegDeleteValue(hKey, appName);
        }
        RegCloseKey(hKey);
    }
}

bool IsStartupEnabled() {
    HKEY hKey;
    bool enabled = false;
    if (RegOpenKeyEx(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        wchar_t value[512];
        DWORD size = sizeof(value);
        if (RegQueryValueEx(hKey, L"GhostSwitchID", 0, NULL, (LPBYTE)value, &size) == ERROR_SUCCESS) {
            enabled = true;
        }
        RegCloseKey(hKey);
    }
    return enabled;
}

void ShowTrayMenu(HWND hwnd) {
    POINT pt;
    GetCursorPos(&pt);
    HMENU menu = CreatePopupMenu();

    InsertMenu(menu, -1, MF_BYPOSITION | (midiEnabled ? MF_CHECKED : MF_UNCHECKED), ID_TRAY_ENABLE, L"Enable MIDI");
    InsertMenu(menu, -1, MF_BYPOSITION | (!midiEnabled ? MF_CHECKED : MF_UNCHECKED), ID_TRAY_DISABLE, L"Disable MIDI");
    InsertMenu(menu, -1, MF_BYPOSITION | (IsStartupEnabled() ? MF_CHECKED : MF_UNCHECKED), ID_TRAY_STARTUP, L"Start with Windows");
    InsertMenu(menu, -1, MF_BYPOSITION, ID_TRAY_EXIT, L"Exit");

    SetForegroundWindow(hwnd);
    TrackPopupMenu(menu, TPM_BOTTOMALIGN | TPM_LEFTALIGN, pt.x, pt.y, 0, hwnd, NULL);
    DestroyMenu(menu);
}

int FindGhostSwitchPort(RtMidiOut* midiout) {
    unsigned int nPorts = midiout->getPortCount();
    for (unsigned int i = 0; i < nPorts; ++i) {
        std::string name = midiout->getPortName(i);
        if (name.find("GhostSwitchID") != std::string::npos) return i;
    }
    return -1;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE:
        nid.cbSize = sizeof(NOTIFYICONDATA);
        nid.hWnd = hwnd;
        nid.uID = 1;
        nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
        nid.uCallbackMessage = WM_TRAYICON;
        nid.hIcon = (HICON)LoadImage(NULL, GetIconPath().c_str(), IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE);
        wcscpy_s(nid.szTip, L"GhostSwitchID");
        Shell_NotifyIcon(NIM_ADD, &nid);

        for (int i = 0; i < 12; ++i) {
            RegisterHotKey(hwnd, i + 1, MOD_CONTROL | MOD_SHIFT, VK_F1 + i);
        }

        try {
            midiout = new RtMidiOut(RtMidi::WINDOWS_MM);
            int portIndex = FindGhostSwitchPort(midiout);
            if (portIndex >= 0) {
                midiout->openPort(portIndex);
            }
            else {
                MessageBox(hwnd, L"GhostSwitchID MIDI portu bulunamadı.\nLoopMIDI açık mı?", L"Hata", MB_ICONERROR);
            }
        }
        catch (...) {
            MessageBox(hwnd, L"MIDI bağlantısı kurulamadı.", L"Hata", MB_ICONERROR);
        }

        if (!IsStartupEnabled()) {
            ToggleStartup(true);
        }
        break;

    case WM_HOTKEY:
        if (wParam >= 1 && wParam <= 12) {
            SendMidi(0x20 + (wParam - 1)); // CC#32 → F1, CC#33 → F2, ...
        }
        break;

    case WM_TRAYICON:
        if (lParam == WM_RBUTTONUP) ShowTrayMenu(hwnd);
        break;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case ID_TRAY_ENABLE: midiEnabled = true; break;
        case ID_TRAY_DISABLE: midiEnabled = false; break;
        case ID_TRAY_STARTUP: ToggleStartup(!IsStartupEnabled()); break;
        case ID_TRAY_EXIT:
            Shell_NotifyIcon(NIM_DELETE, &nid);
            PostQuitMessage(0);
            break;
        }
        break;

    case WM_DESTROY:
        Shell_NotifyIcon(NIM_DELETE, &nid);
        if (midiout) delete midiout;
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    hInst = hInstance;
    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"GhostSwitchIDClass";
    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(0, wc.lpszClassName, L"GhostSwitchID", 0, 0, 0, 0, 0, NULL, NULL, hInstance, NULL);
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
