# Context Project CG-1

## Name: Defuse the bomb
**Game Engine: Godot**
**Object recognition: OpenCV**

## Cloning the repository
This section will cover cloning this repository, as well as the submodules, and how to get up and running with the project.

### Prerequisites
* Godot 3.2.1
* SCons 3.1.2
* OpenCV 4.3.0
* Windows: Visual Studio

#### Godot
You need to have a Godot `.exe`, (this is the same `.exe` that you use to build the game).
Verify that you can use this through the cli:
```
$ godot --version
3.2.1.stable.mono.official
```
If you do not see above output, it likely means that your path isn't set up correctly (or the `.exe` has a different name), there are two things you can do now;
Either you fix this by setting the path correctly so that you can use `godot` anywhere.
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

#### Scons
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
@TODO 

### Visual Studio
On Windows, you will need to install Visual Studio so that you have access to `cl.exe`, which you need when building C++.
Check out this [part](https://docs.godotengine.org/en/stable/development/compiling/compiling_for_windows.html#installing-visual-studio-caveats) of Godots documentation on VS.

### Cloning
Since this branch includes a submodule, you need to clone recursively so that submodules get cloned as well:
```
$ git clone --recursive git@gitlab.ewi.tudelft.nl:TI2806/2019-2020/cg-01/main-repository.git
```
If you've already cloned the project, but now need to switch to this branch and get the submodule:
```
$ git checkout --track origin/gdnative
$ git submodule update --init
```

### Building bindings
We need to generate the Godot C++ bindings so that we can make use of these when compiling our C++ scripts.
That's why the submodule `godot-cpp` is included, they contain the source files to generate the bindings.

First we do need to generate an `api.json`. (Why you ask? Beats me.)
Once you're in the root of the project in your terminal, call the following:
```
$ <godot> --gdnative-generate-json-api api.json
```
With `<godot>` being either the command `godot`, or a path directly to the `.exe` file as described in the prerequisites.
The resulting `api.json` file should be placed in the root directory of the project.

Now we'll actually generate the bindings.
To speed up compilation, add `-j<N>` to the `scons` command, where `N` is the number of CPU threads you have on your system. The example below uses 4 threads.
Replace `<platform>` with `windows`, `osx` or `linux` depending on your OS.
Note: Add `bits=64` to the `scons` command if you're on Windows.
```
$ cd godot-cpp
$ scons platform=<platform> generate_bindings=yes use_custom_api_file=yes custom_api_file=../api.json -j4
```
This step might take a little bit, when this has succeeded, you should have libraries stored in `godot-cpp/bin/`

Previous steps should be a "do once and forget" situation. The step that follows is what you need to do each time you make changes to the plugin so that you can import the output of that into Godot.

### Building the GDNative plugin
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

### More information
* [Godot's tutorial on GDNative C++](https://docs.godotengine.org/en/stable/tutorials/plugins/gdnative/gdnative-cpp-example.html).

**Developer Team:**
- Kevin van der Werff, Producer & Communication 
- Onno Gieling, Lead Playtesting
- Jonas Duifs, Interaction Designer 
- Pepijn Klop, Lead Designer
- Gees Brouwers, Lead Artist & SFX Designer 
- Jeroen Janssen, Lead Programmer 