# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.14

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
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
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/wangshaoyong/CLionProjects/CSA

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/wangshaoyong/CLionProjects/CSA/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/CSA.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/CSA.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/CSA.dir/flags.make

CMakeFiles/CSA.dir/main.cpp.o: CMakeFiles/CSA.dir/flags.make
CMakeFiles/CSA.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/wangshaoyong/CLionProjects/CSA/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/CSA.dir/main.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/CSA.dir/main.cpp.o -c /Users/wangshaoyong/CLionProjects/CSA/main.cpp

CMakeFiles/CSA.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/CSA.dir/main.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/wangshaoyong/CLionProjects/CSA/main.cpp > CMakeFiles/CSA.dir/main.cpp.i

CMakeFiles/CSA.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/CSA.dir/main.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/wangshaoyong/CLionProjects/CSA/main.cpp -o CMakeFiles/CSA.dir/main.cpp.s

# Object files for target CSA
CSA_OBJECTS = \
"CMakeFiles/CSA.dir/main.cpp.o"

# External object files for target CSA
CSA_EXTERNAL_OBJECTS =

CSA: CMakeFiles/CSA.dir/main.cpp.o
CSA: CMakeFiles/CSA.dir/build.make
CSA: CMakeFiles/CSA.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/wangshaoyong/CLionProjects/CSA/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable CSA"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/CSA.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/CSA.dir/build: CSA

.PHONY : CMakeFiles/CSA.dir/build

CMakeFiles/CSA.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/CSA.dir/cmake_clean.cmake
.PHONY : CMakeFiles/CSA.dir/clean

CMakeFiles/CSA.dir/depend:
	cd /Users/wangshaoyong/CLionProjects/CSA/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/wangshaoyong/CLionProjects/CSA /Users/wangshaoyong/CLionProjects/CSA /Users/wangshaoyong/CLionProjects/CSA/cmake-build-debug /Users/wangshaoyong/CLionProjects/CSA/cmake-build-debug /Users/wangshaoyong/CLionProjects/CSA/cmake-build-debug/CMakeFiles/CSA.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/CSA.dir/depend

