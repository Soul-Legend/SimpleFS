MAKENAME = SimpleFS

MODULES = disk fs file_ops
GUI_MODULES = button popup_one_b popup_two_b scrollable_text inode_list disk_selection_screen gui_controller system_controller disk_content_screen inode_content_screen

SRC_DIR = src
INCL_DIR = include
BUILD_DIR = build
BIN_DIR = bin

CXX = g++
CXXFLAGS = -std=c++20 -O0 -g -Wall -Wextra -Wpedantic -Werror
LDFLAGS = -lm -lsfml-graphics -lsfml-window -lsfml-system
