---

# Music Player Application

## Features

* User authentication: sign up and login
* Playback of MP3 and WAV files
* Playlist management: create, add songs, load playlists, queue songs
* Playback controls: play/pause, previous/next track, volume control, progress slider with seek
* Playback modes: repeat single track, repeat playlist, shuffle
* Synchronization between devices: sender and receiver modes via TCP/IP
* Visual equalizer widget
* Security: password hashing, password recovery with security question

## Project Structure

* **ClickableSlider.h**: Custom clickable slider widget
* **encryption.\***: Functions for hashing and encrypting data
* **equalizerwidget.\***: Visual equalizer widget
* **login.\***: User login, signup, and password recovery logic
* **mainwindow.\***: Main application window and controls
* **musicplayer.\***: Core music playback logic and playlist management
* **simple\_sync.\***: TCP/IP synchronization functionality
* **user.\***: User data model
* **usermanager.\***: User management (load/save, validation)
* **ui\_login.ui, ui\_mainwindow\.ui**: Qt Designer UI files

## Requirements

* Qt 6.x
* C++ compiler (MSVC, MinGW, GCC, Clang)

## Build and Run

1. Clone the repository and enter the project directory
2. Open the project in Qt Creator and build it
   Or run `qmake` then `make` (or `nmake`) from the command line
3. Run the generated executable (found in debug or release folder)

## Notes

* Music files should be placed in `D:/UserTrack`
* Icons paths are hardcoded; either place icons accordingly or update paths in the code
* User data is saved in `D:/nima/play/users.txt`
* Playlists are saved as `playlists_<username>.json` in the same folder

## Synchronization

* Sender mode: press "S" to act as server and send playback commands
* Receiver mode: press "R" to connect as client and receive commands
* Default IP: `127.0.0.1`, port: `12345`

## Important

* Consider changing absolute paths to relative paths for portability
* Validation errors during login/signup are shown to users
* For support or to report issues, please use the GitHub Issues page

---
# Windos_Player
