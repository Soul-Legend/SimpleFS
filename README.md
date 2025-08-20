# SimpleFS: A Unix-Like File System Simulator with a GUI

This project is a C++ implementation of SimpleFS, a lightweight file system that emulates the inode layer of traditional Unix-based systems. It operates on a simulated disk (a single host file), providing a safe environment to explore low-level file system concepts like data structures, block management, and metadata handling.

The system features a dual-interface design, offering both an interactive command-line shell for scripting and testing, and a full-featured graphical user interface (GUI) built with SFML for visual interaction and management.

## Features

*   **Inode-Based Architecture:** Manages file metadata using a classic inode structure, featuring direct pointers and a single indirect pointer for block allocation.
*   **Simulated Disk Management:** All operations are performed on a disk image file, allowing for safe creation, formatting, mounting, and manipulation of the file system without affecting the host OS.
*   **In-Memory Bitmap Recovery:** A key feature where the free-block bitmap is not stored on disk. Instead, it is rebuilt in memory upon every `mount` operation by scanning all valid inodes, simulating an `fsck`-like consistency check on startup.
*   **Core File System API:** Provides a clean C++ class interface with essential functions like `format`, `mount`, `create`, `delete`, `read`, `write`, and `getsize`.
*   **Dual User Interfaces:**
    *   **Command-Line Interface (CLI):** An interactive shell (`simplefs_shell`) supporting commands like `format`, `mount`, `debug`, `copyin`, `copyout`, and `cat`.
    *   **Graphical User Interface (GUI):** A user-friendly application (`simplefs_gui`) for visually managing disk images, inodes, and file content.

## File System Architecture

SimpleFS organizes the simulated disk into three main regions, assuming a standard block size of 4 KB.

### Disk Layout

1.  **Superblock:** The very first block (block 0) of the disk. It contains global metadata describing the file system's layout.
2.  **Inode Blocks:** A contiguous region of blocks immediately following the superblock. This area is reserved for storing the inode data structures, typically occupying 10% of the total disk blocks.
3.  **Data Blocks:** The remaining blocks on the disk are used for storing file content and, when needed, indirect pointer blocks.

### The Superblock

The superblock is a 16-byte structure containing four 32-bit integer fields:
*   `magic`: A magic number (`0xf0f03410`) that acts as a signature to identify a disk formatted with SimpleFS.
*   `nblocks`: The total number of blocks on the disk.
*   `ninodeblocks`: The number of blocks reserved for the inode table.
*   `ninodes`: The total number of inodes available in the file system.

### The Inode

Each inode is a 32-byte structure that describes a single file. This size allows 128 inodes to fit perfectly into a single 4 KB block.
*   `isvalid`: A flag (1 or 0) indicating whether the inode is currently in use.
*   `size`: The logical size of the file in bytes.
*   `direct[5]`: An array of five direct pointers, each storing the block number of a data block.
*   `indirect`: A single pointer to an indirect block. This block, in turn, contains up to 1024 pointers to additional data blocks.

### The In-Memory Bitmap

A defining characteristic of SimpleFS is that the free-block bitmap is **not persisted on the disk image**. Instead, it is dynamically constructed in memory every time the `fs_mount` function is executed. The mounting process iterates through every inode block, scans all valid inodes, and marks all referenced data blocks (both direct and indirect) as "in-use" in the newly created bitmap. This design choice simulates a consistency check on every mount and simplifies the file system's on-disk structures.

## Project Structure

```
├── include/              # Header files (.h)
│   ├── disk.h
│   ├── fs.h              # Main INE5412_FS class interface
│   ├── file_ops.h
│   └── graphic_interface/ # GUI component headers
│       ├── gui_controller.h
│       ├── system_controller.h
│       ├── button.h
│       └── ...
├── src/                  # Implementation files (.cc)
│   ├── disk.cc           # Disk emulator implementation
│   ├── fs.cc             # Core file system logic implementation
│   ├── file_ops.cc
│   ├── shell.cc          # CLI entry point and logic
│   ├── gui.cc            # GUI application entry point
│   └── graphic_interface/ # GUI component implementations
│       ├── gui_controller.cc
│       ├── system_controller.cc
│       └── ...
├── fonts/                # Fonts used by the GUI
├── Makefile              # Main build script
├── Config.mk             # Make configurations (modules, flags)
└── Utils.mk              # Make utilities
```

## Code Overview

The project's C++ code is organized around several key classes that separate concerns, promoting modularity and reusability.

*   **`INE5412_FS`**: This class is the heart of the file system. It encapsulates all core logic for formatting the disk, mounting the file system, and manipulating inodes and data blocks. It interacts directly with the `Disk` abstraction layer and is completely independent of any user interface.
*   **`Disk`**: Provides a low-level disk emulation layer. It abstracts the underlying host file into a simple, block-based device, offering only `read` and `write` operations for fixed-size blocks.
*   **`SystemController` (GUI)**: Acts as the backend or "controller" for the graphical interface. It manages the application's state, holding instances of the `Disk` and `INE5412_FS` classes. It translates user actions from the GUI into corresponding calls to the file system API.
*   **`GuiController` (GUI)**: Manages the SFML window, handles user input events, and controls the rendering and transitions between different application screens (e.g., Disk Selection, Content View). It is responsible for the "view" and delegates all logic to the `SystemController`.

## Design Decisions

Several key architectural decisions were made to meet the project's goals and scope:

*   **In-Memory Free-Block Bitmap**: The decision not to store the bitmap on disk is intentional. Rebuilding it from scratch on every mount serves two primary purposes: it provides a practical lesson in file system consistency checking (simulating an `fsck` process) and it significantly simplifies the on-disk data structures. The trade-off is a slightly slower mount time, which is an acceptable compromise for a simulation tool.
*   **SFML for GUI**: SFML was chosen as the graphics library for its lightweight nature, cross-platform compatibility, and low-level control over rendering. This allowed for the creation of a custom UI toolkit from scratch, which is ideal for an educational project, without depending on a heavy, all-encompassing UI framework.
*   **Decoupled Core Logic**: The core file system module (`fs.cc`, `fs.h`) is completely decoupled from the user interfaces (`shell.cc`, `gui.cc`). This clean separation means the file system logic could be reused in other applications or tested independently, which is a fundamental principle of good software design.

## Prerequisites

*   **C++ Compiler:** A compiler supporting the C++20 standard (e.g., GCC 10+ or Clang 12+).
*   **Make:** The `make` build automation tool.
*   **SFML Library:** Required for building and running the graphical user interface.
    *   On Debian/Ubuntu-based systems, install it via: `sudo apt-get install libsfml-dev`

## Compilation and Usage

### Compilation

To compile the entire project, run the `make` command in the root directory.

```
make
```

This will produce two executables in the `bin/` directory:
*   `simplefs_shell`: The command-line interface.
*   `simplefs_gui`: The graphical user interface.

### Usage - Command-Line Interface (CLI)

The CLI requires a disk image filename and the total number of blocks as arguments.

*   **To use an existing disk image (e.g., `image.20` with 20 blocks):**
```
./bin/simplefs_shell image.20 20
```

*   **To create a new disk image (e.g., `mydisk` with 50 blocks):**
```
./bin/simplefs_shell mydisk 50
```

Available commands inside the shell include:
*   `format`: Initializes the disk with a new, empty SimpleFS file system.
*   `mount`: Mounts the file system and reconstructs the in-memory free-block bitmap.
*   `debug`: Displays the superblock status and metadata for all valid inodes.
*   `create`: Creates a new, empty inode.
*   `delete <inumber>`: Deletes the specified inode and frees its data blocks.
*   `copyin <host_file> <inumber>`: Copies a file from the host system into an inode.
*   `copyout <inumber> <host_file>`: Copies content from an inode to a file on the host system.
*   `cat <inumber>`: Prints the content of an inode to the console.
*   `help`: Shows a list of available commands.

### Usage - Graphical User Interface (GUI)

Launch the GUI application by running the executable:

```
./bin/simplefs_gui
```

The graphical interface provides a visual workflow for all file system operations:
1.  **Disk Selection Screen:** Create a new disk by specifying a filename and block count, or select an existing disk image file.
2.  **Disk Management:** After selecting a disk, use the on-screen buttons to `Format` and `Mount` it.
3.  **Content View:** Once mounted, the main screen displays the superblock information and a scrollable list of all active inodes.
4.  **Inode Management:** From this screen, you can create new inodes or click on an existing one to view its detailed information.
5.  **Inode Detail Screen:** This view shows the inode's metadata (`debug` output) and its full content (`cat` output). From here, you can `Delete` the inode or perform `CopyIn` and `CopyOut` operations using file dialogs.

## ✒️ Authors
* ##### [Vitor Praxedes Calegari](https://github.com/Vitor-Calegari)
* ##### [Matheus Fernandes Bigolin](https://github.com/mfrdbigolin)
* ##### [Pedro Henrique Taglialenha](https://github.com/Soul-Legend)
