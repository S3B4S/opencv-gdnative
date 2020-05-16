# Defuse the bomb
* Game engine: Godot
* Object recognition: OpenCV

# Getting up and running
This section will cover cloning this repository, as well as the submodules, and how to get up and running with the project.

## Prerequisites
* Godot 3.2.1
* SCons 3.1.2
* OpenCV 4.3.0
* Windows: Visual Studio

### Godot
You need to have Godot installed, do so [here](https://godotengine.org/download/).
Verify that you can use this through the cli:
```
$ godot --version
3.2.1.stable.mono.official
```
If you do not see above output, there are several things you can do depending on your OS.

#### Windows
Either you fix this by adding the path to your Godot `.exe` to the PATH environment variable correctly so that you can use `godot` anywhere.
Or you take the easy route, which is directly calling it as follows:
```
$ "<path-to-godot-dir>/<godot-exe-name>.exe" --version
```
So for example:
```
$ "D:\Program Files (x86)\Godot\Godot_v3.2.1-stable_mono_win64.exe" --version
3.2.1.stable.mono.official
```
Which is fine to do for now, as we will only need to use this `.exe` through the cli once for the current setup.

#### MacOS
When you install Godot, you can acess the binary file that you can run in the cli with the following path:
```
/Applications/<Godot>.app/Contents/MacOS/Godot
```
The name of the `.app` depends on whether you installed the Mono or the non Mono version.

Just like for Windows, there's an easy route you can take by directly calling the path as follows
```
$ /Applications/<Godot>.app/Contents/MacOS/Godot --version
```
So for example (I have the Mono version):
```
$ /Applications/Godot_mono.app/Contents/MacOS/Godot --version
0: /Applications/Godot_mono.app/Contents/MacOS/Godot
1: --version
Current path: /Users/kevin/Development/contextproject
3.2.1.stable.mono.official
```

Or you make it so that you can use the `godot` command by adding the following:
```
$ ln -s /Applications/<Godot>.app/Contents/MacOS/Godot /usr/local/bin/godot
```
For more information on this, see [here](https://godotengine.org/qa/22104/how-to-run-a-project-in-godot-from-command-on-mac).

### Scons
Go to their [site](https://scons.org/pages/download.html) and download the 3.1.2 version of the "SCons Packages" (so not "scons-local Packages" or "scons-src Packages").
Extract the package and follow the instructions in the `README.md`.

Verify that it's working with the following command
```
$ scons --version
SCons by Steven Knight et al.:
        script: v3.1.2.bee7caf9defd6e108fc2998a2520ddb36a967691, 2019-12-17 02:07:09, by bdeegan on octodog
        engine: v3.1.2.bee7caf9defd6e108fc2998a2520ddb36a967691, 2019-12-17 02:07:09, by bdeegan on octodog
        engine path: ['C:\\Users\\kevin\\Development\\scons-3.1.2\\script\\..\\engine\\SCons']
Copyright (c) 2001 - 2019 The SCons Foundation
```

Do not proceed until this works.

### OpenCV
#### Windows
@TODO when we have something working on windows (I do not want to declare a dependency already). ~ @ksavanderwerff

#### MacOS
Installing OpenCV 4 for MacOS requires Xcode and homebrew.

1. **Install Xcode**  
Install Xcode from the App Store. Open the App Store and search for Xcode and then click the Get button

2. **Install Homebrew**  
Open the terminal and execute the following code:  
``` /usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"```

3. **Install OpenCV**   
Open the terminal and execute the following code:  
```brew install opencv```

If these the steps were not sufficient, additional installation steps can be found [here](https://medium.com/@jaskaranvirdi/setting-up-opencv-and-c-development-environment-in-xcode-b6027728003).

### Visual Studio
On Windows, you will need to install Visual Studio so that you have access to `cl.exe`, which you need when building C++.
Check out this [part](https://docs.godotengine.org/en/stable/development/compiling/compiling_for_windows.html#installing-visual-studio-caveats) of Godots documentation on VS.

## Cloning
If you've already cloned the project before, skip this step.
Since this branch includes a submodule, you need to clone recursively so that submodules get cloned as well:
```
$ git clone --recursive git@gitlab.ewi.tudelft.nl:TI2806/2019-2020/cg-01/main-repository.git
```

## Submodules
If you've already cloned the project, but have yet to get the submodules:
```
$ git submodule update --init --recursive
```

## Building bindings
We need to generate the Godot C++ bindings so that we can make use of these when compiling our C++ scripts.
That's why the submodule `godot-cpp` is included, they contain the source files to generate the bindings.

Before that we do need to generate an `api.json` (why you ask? Beats me).

Once you're in the root of the project in your terminal, call the following:
```
$ <godot> --gdnative-generate-json-api api.json
```
With `<godot>` being either the command `godot`, or a path directly to the executable file as described in the prerequisites.
The resulting `api.json` file should be placed in the root directory of the project.

Now we'll actually generate the bindings.

To speed up compilation, add `-j<N>` to the `scons` command, where `<N>` is the number of CPU threads you have on your system. The example below uses 4 threads.

Replace `<platform>` with `windows`, `osx` or `linux` depending on your OS.

Note: Add `bits=64` to the `scons` command if you're on Windows.
```
$ cd godot-cpp
$ scons platform=<platform> generate_bindings=yes use_custom_api_file=yes custom_api_file=../api.json -j4
```
This step might take a little bit, when this has succeeded, you should have libraries stored in `godot-cpp/bin/`

Previous steps should be a "do once and forget" situation. The step that follows is what you need to do each time you make changes to the plugin so that you can import the output of that into Godot.

## Building the GDNative plugin
This section operates under the assumption that you have set your `OPENCV_DIR` correctly so that `scons` can find the OpenCV libraries on your computer. See `SConstruct` for the paths.
Go back to the root of the project in your terminal.
Now run the following, where `<platform>` is `windows`, `osx` or `linux`.
```
scons platform=<platform>
```
This should build some libraries into `Godot/bin/<platform>/`.
These libraries are the result of building the GDNative plugin and you should be able to succefully use them in Godot now.
The current Godot project already ataches the built script to a node, so you can simply start the Godot project to test out if it's working.
If it's not working, you'll get errors. If you have no errors, it's working. :sunglasses:

## More information
* [Godot's tutorial on GDNative C++](https://docs.godotengine.org/en/stable/tutorials/plugins/gdnative/gdnative-cpp-example.html).

**Developer Team:**
- Kevin van der Werff, Producer & Communication 
- Onno Gieling, Lead Playtesting
- Jonas Duifs, Interaction Designer 
- Pepijn Klop, Lead Designer
- Gees Brouwers, Lead Artist & SFX Designer 
- Jeroen Janssen, Lead Programmer 