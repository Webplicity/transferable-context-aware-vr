# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.17

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
CMAKE_COMMAND = /home/charles/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/202.7660.37/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/charles/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/202.7660.37/bin/cmake/linux/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/charles/Documents/UA/Context-aware-VR/openvr-master/samples

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/charles/Documents/UA/Context-aware-VR/openvr-master/samples/cmake-build-debug

# Utility rule file for hellovr_vulkan_autogen.

# Include the progress variables for this target.
include hellovr_vulkan/CMakeFiles/hellovr_vulkan_autogen.dir/progress.make

hellovr_vulkan/CMakeFiles/hellovr_vulkan_autogen:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/charles/Documents/UA/Context-aware-VR/openvr-master/samples/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Automatic MOC and UIC for target hellovr_vulkan"
	cd /home/charles/Documents/UA/Context-aware-VR/openvr-master/samples/cmake-build-debug/hellovr_vulkan && /home/charles/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/202.7660.37/bin/cmake/linux/bin/cmake -E cmake_autogen /home/charles/Documents/UA/Context-aware-VR/openvr-master/samples/cmake-build-debug/hellovr_vulkan/CMakeFiles/hellovr_vulkan_autogen.dir/AutogenInfo.json Debug

hellovr_vulkan_autogen: hellovr_vulkan/CMakeFiles/hellovr_vulkan_autogen
hellovr_vulkan_autogen: hellovr_vulkan/CMakeFiles/hellovr_vulkan_autogen.dir/build.make

.PHONY : hellovr_vulkan_autogen

# Rule to build all files generated by this target.
hellovr_vulkan/CMakeFiles/hellovr_vulkan_autogen.dir/build: hellovr_vulkan_autogen

.PHONY : hellovr_vulkan/CMakeFiles/hellovr_vulkan_autogen.dir/build

hellovr_vulkan/CMakeFiles/hellovr_vulkan_autogen.dir/clean:
	cd /home/charles/Documents/UA/Context-aware-VR/openvr-master/samples/cmake-build-debug/hellovr_vulkan && $(CMAKE_COMMAND) -P CMakeFiles/hellovr_vulkan_autogen.dir/cmake_clean.cmake
.PHONY : hellovr_vulkan/CMakeFiles/hellovr_vulkan_autogen.dir/clean

hellovr_vulkan/CMakeFiles/hellovr_vulkan_autogen.dir/depend:
	cd /home/charles/Documents/UA/Context-aware-VR/openvr-master/samples/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/charles/Documents/UA/Context-aware-VR/openvr-master/samples /home/charles/Documents/UA/Context-aware-VR/openvr-master/samples/hellovr_vulkan /home/charles/Documents/UA/Context-aware-VR/openvr-master/samples/cmake-build-debug /home/charles/Documents/UA/Context-aware-VR/openvr-master/samples/cmake-build-debug/hellovr_vulkan /home/charles/Documents/UA/Context-aware-VR/openvr-master/samples/cmake-build-debug/hellovr_vulkan/CMakeFiles/hellovr_vulkan_autogen.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : hellovr_vulkan/CMakeFiles/hellovr_vulkan_autogen.dir/depend

