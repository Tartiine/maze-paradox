# best-pf-game

# 🎮 Platformer Game

Welcome to our game project, crafted with C++ and SFML.

## 🚀 Getting Started

These instructions will get you a copy of the project up and running on your local machine.

### 🛠️ Building

- **On Windows ![VSCode](https://img.shields.io/badge/IDE-VSCode-blue.svg)**


#### Prerequisites
- Ensure you have [Visual Studio Code](https://code.visualstudio.com/) installed.
- Download and install [SFML](https://www.sfml-dev.org/download/sfml/2.6.1/). Specifically, you need the version 2.6.1 and the build for GCC 13.1.0 MinGW. This version ensures compatibility with the project setup.
- Make sure you have a C++ compiler that supports C++17 or higher. For compatibility with SFML 2.6.1 built with GCC 13.1.0, use the same or compatible GCC version.

#### Setting Up Environment Variables
1. **SFML_DIR Environment Variable:**
   - After installing SFML, you need to set the `SFML_DIR` environment variable to the path where SFML is installed. This is required for the build system to locate SFML's libraries and headers.
   - You can set this variable system-wide in Windows:
     - Open the Start Search, type in `Environment Variables`, and select "Edit the system environment variables".
     - In the System Properties window, click on the "Environment Variables" button.
     - In the Environment Variables window, click "New" under the "System variables" section.
     - Set the variable name to `SFML_DIR` and the variable value to the path of your SFML installation (e.g., `C:\SFML` if you extracted SFML to C:\SFML).
   - Alternatively, you can set this variable temporarily in a command prompt with the following command (you will need to do this every time in a new command prompt window):
     ```bash
     set SFML_DIR=C:\Path\To\SFML
     ```

#### Compiling the Project with VSCode
1. **Configure VSCode:**
   - Open Visual Studio Code.
   - Open the project folder in VSCode.
   - Ensure you have the C++ extension (such as Microsoft’s C++ extension) installed in VSCode to support C++ development.

2. **Build and Run:**
   - Use the shortcut `Ctrl+Shift+B` to run the build task. This will compile and run the project and link SFML automatically.

## 🛠️ Built With

- [SFML](https://www.sfml-dev.org/) - The Simple and Fast Multimedia Library
- [C++](https://en.cppreference.com/w/) - Programming language


## 🖼️ Screenshots

## 📖 Authors

- **Tartine** - *Student* - [Tartiine](https://github.com/tartiine)
- **Transformot** - *Student* - [Transformot](https://github.com/transformot)

## 🎉 Acknowledgments

- Hat tip to anyone whose code was used
- Special thanks to [Maxime Devanne](https://github.com/maxwell1503) for his guidance and support.
