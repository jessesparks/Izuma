# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.27

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /home/jesse/bin/CLion/clion-2023.3.2/bin/cmake/linux/x64/bin/cmake

# The command to remove a file.
RM = /home/jesse/bin/CLion/clion-2023.3.2/bin/cmake/linux/x64/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/jesse/dev/Izuma

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jesse/dev/Izuma/cmake-build-debug

# Include any dependencies generated for this target.
include IzumaGame/CMakeFiles/IzumaGame.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include IzumaGame/CMakeFiles/IzumaGame.dir/compiler_depend.make

# Include the progress variables for this target.
include IzumaGame/CMakeFiles/IzumaGame.dir/progress.make

# Include the compile flags for this target's objects.
include IzumaGame/CMakeFiles/IzumaGame.dir/flags.make

IzumaGame/CMakeFiles/IzumaGame.dir/src/IzumaGame.cpp.o: IzumaGame/CMakeFiles/IzumaGame.dir/flags.make
IzumaGame/CMakeFiles/IzumaGame.dir/src/IzumaGame.cpp.o: /home/jesse/dev/Izuma/IzumaGame/src/IzumaGame.cpp
IzumaGame/CMakeFiles/IzumaGame.dir/src/IzumaGame.cpp.o: IzumaGame/CMakeFiles/IzumaGame.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/jesse/dev/Izuma/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object IzumaGame/CMakeFiles/IzumaGame.dir/src/IzumaGame.cpp.o"
	cd /home/jesse/dev/Izuma/cmake-build-debug/IzumaGame && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT IzumaGame/CMakeFiles/IzumaGame.dir/src/IzumaGame.cpp.o -MF CMakeFiles/IzumaGame.dir/src/IzumaGame.cpp.o.d -o CMakeFiles/IzumaGame.dir/src/IzumaGame.cpp.o -c /home/jesse/dev/Izuma/IzumaGame/src/IzumaGame.cpp

IzumaGame/CMakeFiles/IzumaGame.dir/src/IzumaGame.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/IzumaGame.dir/src/IzumaGame.cpp.i"
	cd /home/jesse/dev/Izuma/cmake-build-debug/IzumaGame && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jesse/dev/Izuma/IzumaGame/src/IzumaGame.cpp > CMakeFiles/IzumaGame.dir/src/IzumaGame.cpp.i

IzumaGame/CMakeFiles/IzumaGame.dir/src/IzumaGame.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/IzumaGame.dir/src/IzumaGame.cpp.s"
	cd /home/jesse/dev/Izuma/cmake-build-debug/IzumaGame && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jesse/dev/Izuma/IzumaGame/src/IzumaGame.cpp -o CMakeFiles/IzumaGame.dir/src/IzumaGame.cpp.s

# Object files for target IzumaGame
IzumaGame_OBJECTS = \
"CMakeFiles/IzumaGame.dir/src/IzumaGame.cpp.o"

# External object files for target IzumaGame
IzumaGame_EXTERNAL_OBJECTS =

IzumaGame/IzumaGame: IzumaGame/CMakeFiles/IzumaGame.dir/src/IzumaGame.cpp.o
IzumaGame/IzumaGame: IzumaGame/CMakeFiles/IzumaGame.dir/build.make
IzumaGame/IzumaGame: IzumaEngine/libIzumaEngine.a
IzumaGame/IzumaGame: IzumaGame/CMakeFiles/IzumaGame.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/jesse/dev/Izuma/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable IzumaGame"
	cd /home/jesse/dev/Izuma/cmake-build-debug/IzumaGame && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/IzumaGame.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
IzumaGame/CMakeFiles/IzumaGame.dir/build: IzumaGame/IzumaGame
.PHONY : IzumaGame/CMakeFiles/IzumaGame.dir/build

IzumaGame/CMakeFiles/IzumaGame.dir/clean:
	cd /home/jesse/dev/Izuma/cmake-build-debug/IzumaGame && $(CMAKE_COMMAND) -P CMakeFiles/IzumaGame.dir/cmake_clean.cmake
.PHONY : IzumaGame/CMakeFiles/IzumaGame.dir/clean

IzumaGame/CMakeFiles/IzumaGame.dir/depend:
	cd /home/jesse/dev/Izuma/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jesse/dev/Izuma /home/jesse/dev/Izuma/IzumaGame /home/jesse/dev/Izuma/cmake-build-debug /home/jesse/dev/Izuma/cmake-build-debug/IzumaGame /home/jesse/dev/Izuma/cmake-build-debug/IzumaGame/CMakeFiles/IzumaGame.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : IzumaGame/CMakeFiles/IzumaGame.dir/depend
