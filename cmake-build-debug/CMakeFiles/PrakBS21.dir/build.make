# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.21

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/danieloberle/Desktop/BS/Praktikum

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/danieloberle/Desktop/BS/Praktikum/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/PrakBS21.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/PrakBS21.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/PrakBS21.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/PrakBS21.dir/flags.make

CMakeFiles/PrakBS21.dir/main.c.o: CMakeFiles/PrakBS21.dir/flags.make
CMakeFiles/PrakBS21.dir/main.c.o: ../main.c
CMakeFiles/PrakBS21.dir/main.c.o: CMakeFiles/PrakBS21.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/danieloberle/Desktop/BS/Praktikum/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/PrakBS21.dir/main.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/PrakBS21.dir/main.c.o -MF CMakeFiles/PrakBS21.dir/main.c.o.d -o CMakeFiles/PrakBS21.dir/main.c.o -c /Users/danieloberle/Desktop/BS/Praktikum/main.c

CMakeFiles/PrakBS21.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/PrakBS21.dir/main.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/danieloberle/Desktop/BS/Praktikum/main.c > CMakeFiles/PrakBS21.dir/main.c.i

CMakeFiles/PrakBS21.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/PrakBS21.dir/main.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/danieloberle/Desktop/BS/Praktikum/main.c -o CMakeFiles/PrakBS21.dir/main.c.s

# Object files for target PrakBS21
PrakBS21_OBJECTS = \
"CMakeFiles/PrakBS21.dir/main.c.o"

# External object files for target PrakBS21
PrakBS21_EXTERNAL_OBJECTS =

PrakBS21: CMakeFiles/PrakBS21.dir/main.c.o
PrakBS21: CMakeFiles/PrakBS21.dir/build.make
PrakBS21: CMakeFiles/PrakBS21.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/danieloberle/Desktop/BS/Praktikum/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable PrakBS21"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/PrakBS21.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/PrakBS21.dir/build: PrakBS21
.PHONY : CMakeFiles/PrakBS21.dir/build

CMakeFiles/PrakBS21.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/PrakBS21.dir/cmake_clean.cmake
.PHONY : CMakeFiles/PrakBS21.dir/clean

CMakeFiles/PrakBS21.dir/depend:
	cd /Users/danieloberle/Desktop/BS/Praktikum/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/danieloberle/Desktop/BS/Praktikum /Users/danieloberle/Desktop/BS/Praktikum /Users/danieloberle/Desktop/BS/Praktikum/cmake-build-debug /Users/danieloberle/Desktop/BS/Praktikum/cmake-build-debug /Users/danieloberle/Desktop/BS/Praktikum/cmake-build-debug/CMakeFiles/PrakBS21.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/PrakBS21.dir/depend
