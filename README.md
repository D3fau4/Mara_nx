# Mara_nx

Mara_nx is a game patching application for Nintendo Switch. This application was build with Borealis by natinusala to simplify the patching process for Spanish translations available on TraduSquare.

## Requirements for Compilation

Make sure you meet the following requirements to compile Mara_nx:

- DevkitPro.
- CMake 3.22 or higher.

## Build

Follow the steps below to build Mara_nx:

1. Clone this repository to your system:
```
git clone https://github.com/D3fau4/Mara_nx.git
```

2. Navigate to the cloned repository directory:
```
cd Mara_nx
```

3. Create a build directory:
```
mkdir build
```

4. Generate build files with CMake:
```
cmake -G "Unix Makefiles" --toolchain=DevkitA64Libnx.cmake -S . -B build
```


5. Build the project:
```
cmake --build .
```

6. Once the compilation is finished, an executable file for Mara_nx.nro will be generated in `build/`

<!--## Usage

Follow these steps to use Mara_nx:

1. Open the program from the homebrew application selector on your Nintendo Switch.

2. On the main screen of Mara_nx, select the "Install" button.

3. The program will automatically search for a compatible game with the patch on your Nintendo Switch. If a compatible game is found, a notification will appear indicating that it has been successfully installed in the Mara launcher.

4. On the next execution of the game installed in the Mara launcher, Mara_nx will automatically launch before the game.

5. Follow the instructions provided by Mara_nx to complete the patching and installation process for the Spanish translation.

6. Once the patching process is complete, you can enjoy the patched game in Spanish.

Note: Be sure to follow all instructions provided by the application for safe and proper usage.
--->

## Credits

- [libnx](https://github.com/switchbrew/libnx) - Tools and resources for Nintendo Switch homebrew development.
- [Borealis](https://github.com/natinusala/borealis) - Developed by natinusala.
