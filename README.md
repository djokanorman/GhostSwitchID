
<img width="1024" height="1024" alt="Copilot_20251111_202628" src="https://github.com/user-attachments/assets/abb12ee7-3018-4d12-bddd-8a636ead4fd1" />

# GhostSwitchID
GhostSwitchID is a lightweight Windows tool that sends MIDI CC messages via LoopMIDI when pressing Ctrl+Shift+F1–F12. It runs in the system tray, starts with Windows, and enables automatic preset switching in Sonarworks SoundID Reference or any MIDI-compatible software. No installation or coding required.

# 🎛️ GhostSwitchID

GhostSwitchID is a lightweight Windows application that sends MIDI CC messages when you press **Ctrl + Shift + F1–F12**.  
It runs silently in the system tray, starts automatically with Windows, and is designed for **preset switching in SoundID Reference** or any MIDI-compatible software.

---

## ✨ Features

- ⌨️ **Hotkey support**: Ctrl + Shift + F1–F12 triggers MIDI CC messages (CC#32 to CC#43)
- 🎹 **LoopMIDI integration**: Sends MIDI to a virtual port named `GhostSwitchID`
- 🧠 **Fully automatic**: No manual setup or configuration required
- 🖼️ **Custom icon**: Embedded `.ico` file for professional appearance
- 🪟 **System tray**: Runs silently with tray icon and exit option
- 🚀 **Auto-start**: Adds itself to Windows startup registry
- 📦 **Portable**: Works from any folder, no installation needed

---

## 🛠️ Requirements

- ✅ [LoopMIDI](https://www.tobias-erichsen.de/software/loopmidi.html) must be installed  
- ✅ A virtual MIDI port named `GhostSwitchID` must be created in LoopMIDI

---

## 📥 Installation

1. Download the latest `GhostSwitchID.exe` from the [Releases](https://github.com/yourusername/GhostSwitchID/releases) page  
2. Install LoopMIDI and create a port named `GhostSwitchID`  
3. Run `GhostSwitchID.exe` — it will appear in the system tray  
4. Press **Ctrl + Shift + F1–F12** to send MIDI CC messages  
5. Use Stream Deck or any macro tool to trigger these hotkeys

---

## 🎛️ MIDI Mapping (SoundID Reference)

To make GhostSwitchID control your presets in SoundID Reference:

1. Open SoundID Reference  
2. Click the **three dots (⋯)** next to any preset  
3. Choose **“Map MIDI”**  
4. Press the desired hotkey (e.g. `Ctrl + Shift + F3`)  
5. Save the mapping — now that preset will activate when you press that key

Repeat for each preset you want to control.

---

## 🎯 MIDI CC Mapping Table

| Hotkey              | MIDI CC Sent |
|---------------------|--------------|
| Ctrl + Shift + F1   | CC#32  
| Ctrl + Shift + F2   | CC#33  
| Ctrl + Shift + F3   | CC#34  
| Ctrl + Shift + F4   | CC#35  
| Ctrl + Shift + F5   | CC#36  
| Ctrl + Shift + F6   | CC#37  
| Ctrl + Shift + F7   | CC#38  
| Ctrl + Shift + F8   | CC#39  
| Ctrl + Shift + F9   | CC#40  
| Ctrl + Shift + F10  | CC#41  
| Ctrl + Shift + F11  | CC#42  
| Ctrl + Shift + F12  | CC#43  

---

## 📦 Portability

GhostSwitchID does **not depend on any fixed folder or external files**.  
You can place the `.exe` anywhere — desktop, USB drive, custom folder — and it will work.  
The icon is embedded, and startup registry uses the current path automatically.

---

## 👨‍💻 Developer

GhostSwitchID is built by **Okan Orman**, a technical visionary focused on automation and seamless user experience in music production.

---

## 📬 Questions or Feedback?

Feel free to open an issue on GitHub or reach out via the Discussions tab.  
This tool was built to solve a real-world problem — and now it’s yours to use, improve, and share.


