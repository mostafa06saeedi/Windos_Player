# 🎵 Windos Player

A feature-rich **Qt6 desktop music player** built in C++ — complete with user authentication, an animated equalizer, and **real-time playback sync between two clients over TCP**. Originally built as a Computer Networks course project.

---

## ✨ Features

### 🔐 User Accounts
- Sign up, log in, and password recovery flow
- Passwords are hashed before storage (never kept in plain text)
- Lightweight custom encryption layer for storing sensitive user data

### 🎧 Music Playback
- Full playback controls (play, pause, skip, seek) via `QMediaPlayer`
- Custom **clickable slider** for intuitive seeking
- Animated **real-time equalizer widget** that visualizes the beat

### 🌐 Network Sync (the fun part)
- `SimpleSync` opens a TCP server/client connection between two instances
- One instance can broadcast **play/stop commands and playback position** to another — so two people can listen in sync, on two different machines
- Built directly on `QTcpServer` / `QTcpSocket`, no external dependencies

### 🖼️ UI
- Clean `QMainWindow`-based interface designed in Qt Designer (`.ui` files)
- Themed login window with a custom welcome intro screen

---

## 📂 Project Structure

```
.
├── include/          # Header files (.h)
├── src/              # Source files (.cpp), including main.cpp
├── ui/               # Qt Designer UI files (.ui)
├── CMakeLists.txt    # Build configuration (Qt6 + CMake)
└── .gitignore
```

---

## 🧩 Tech Stack

| Component | Purpose |
|---|---|
| **Qt6 Widgets** | UI framework |
| **Qt6 Multimedia** | Audio playback engine |
| **Qt6 Core (Network)** | TCP-based playback synchronization |
| **QCryptographicHash** | Password hashing |
| **CMake** | Build system |

---

## 🚀 Getting Started

### Prerequisites
- Qt 6.x (Core, Widgets, Multimedia modules)
- CMake ≥ 3.16
- A C++17-compatible compiler

### Build
```bash
mkdir build && cd build
cmake ..
cmake --build .
./play
```

> ⚠️ **Heads up before you build:**
> - `UserManager` currently stores account data at a hardcoded local path (`D:/nima/play/users.txt`), left over from the original developer's machine. You'll want to point this to a path that exists on your system before running it — otherwise sign-up/login won't persist.
> - `SimpleSync::logToFile` writes a `debug_log.txt` to the working directory at runtime; this is gitignored and safe to ignore.

---

## 🕹️ How It Works

1. **Launch** → a short animated welcome screen appears
2. **Log in or sign up** → credentials are hashed and validated
3. **Land in the main player** → browse and play your local music library, watch the equalizer react
4. **(Optional) Sync with a friend** → one instance starts a `SimpleSync` server, the other connects as a client, and playback state streams between them over TCP

---

## 🗂️ File Overview

| File | Responsibility |
|---|---|
| `mainwindow.*` | Main player window and UI logic |
| `musicplayer.*` | Core playback engine wrapper around `QMediaPlayer` |
| `login.*` | Login/sign-up window and flow |
| `user.*` | User data model |
| `usermanager.*` | Account storage, validation, and lookup |
| `encryption.*` | Password hashing and simple data encryption |
| `equalizerwidget.*` | Animated equalizer visualization |
| `clickableslider.*` | Custom seek slider widget |
| `simple_sync.*` | TCP-based playback synchronization between clients |

---

## 👨‍💻 Contributors
- **Mostafa** — Lead developer and sole contributor. Responsible for all stages of design, development, testing, and execution of this project.
