#include "disk.h"
#include "file_ops.h"
#include "fs.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
  char line[1024];
  char cmd[1024];
  char arg1[1024];
  char arg2[1024];
  int inumber, result, args;

  if (argc != 3) {
    cout << "use: " << argv[0] << " <diskfile> <nblocks>\n";
    return 1;
  }

  int nblocks = atoi(argv[2]);
  if (nblocks < 3) {
    std::cout << "Choose a number of blocks greater than two!\n";
    return 1;
  }

  Disk disk(argv[1], nblocks);

  if (disk.size() == -1) {
    return 1;
  }

  INE5412_FS fs(&disk);

  cout << "opened emulated disk image " << argv[1] << " with " << disk.size()
       << " blocks\n";

  while (1) {
    cout << " simplefs> ";
    fflush(stdout);

    if (!fgets(line, sizeof(line), stdin))
      break;

    if (line[0] == '\n')
      continue;

    line[strlen(line) - 1] = 0;

    args = sscanf(line, "%s %s %s", cmd, arg1, arg2);

    if (args == 0)
      continue;

    if (!strcmp(cmd, "format")) {
      if (args == 1) {
        if (fs.fs_format()) {
          cout << "disk formatted.\n";
        } else {
          cout << "format failed!\n";
        }
      } else {
        cout << "use: format\n";
      }
    } else if (!strcmp(cmd, "mount")) {
      if (args == 1) {
        if (fs.fs_mount()) {
          cout << "disk mounted.\n";
        } else {
          cout << "mount failed!\n";
        }
      } else {
        cout << "use: mount\n";
      }
    } else if (!strcmp(cmd, "debug")) {
      if (args == 1) {
        fs.fs_debug();
      } else {
        cout << "use: debug\n";
      }
    } else if (!strcmp(cmd, "getsize")) {
      if (args == 2) {
        inumber = atoi(arg1);
        result = fs.fs_getsize(inumber);
        if (result >= 0) {
          cout << "inode " << inumber << " has size " << result << "\n";
        } else {
          cout << "getsize failed!\n";
        }
      } else {
        cout << "use: getsize <inumber>\n";
      }

    } else if (!strcmp(cmd, "create")) {
      if (args == 1) {
        inumber = fs.fs_create();
        if (inumber > 0) {
          cout << "created inode " << inumber << "\n";
        } else {
          cout << "create failed!\n";
        }
      } else {
        cout << "use: create\n";
      }
    } else if (!strcmp(cmd, "delete")) {
      if (args == 2) {
        inumber = atoi(arg1);
        if (fs.fs_delete(inumber)) {
          cout << "inode " << inumber << " deleted.\n";
        } else {
          cout << "delete failed!\n";
        }
      } else {
        cout << "use: delete <inumber>\n";
      }
    } else if (!strcmp(cmd, "cat")) {
      if (args == 2) {
        inumber = atoi(arg1);
        if (!File_Ops::do_copyout(inumber, "/dev/stdout", &fs)) {
          cout << "cat failed!\n";
        }
      } else {
        cout << "use: cat <inumber>\n";
      }

    } else if (!strcmp(cmd, "copyin")) {
      if (args == 3) {
        inumber = atoi(arg2);
        if (File_Ops::do_copyin(arg1, inumber, &fs)) {
          cout << "copied file " << arg1 << " to inode " << inumber << "\n";
        } else {
          cout << "copy failed!\n";
        }
      } else {
        cout << "use: copyin <filename> <inumber>\n";
      }

    } else if (!strcmp(cmd, "copyout")) {
      if (args == 3) {
        inumber = atoi(arg1);
        if (File_Ops::do_copyout(inumber, arg2, &fs)) {
          cout << "copied inode " << inumber << " to file " << arg2 << "\n";
        } else {
          cout << "copy failed!\n";
        }
      } else {
        cout << "use: copyout <inumber> <filename>\n";
      }

    } else if (!strcmp(cmd, "help")) {
      cout << "Commands are:\n";
      cout << "    format\n";
      cout << "    mount\n";
      cout << "    debug\n";
      cout << "    create\n";
      cout << "    delete  <inode>\n";
      cout << "    cat     <inode>\n";
      cout << "    copyin  <file> <inode>\n";
      cout << "    copyout <inode> <file>\n";
      cout << "    help\n";
      cout << "    quit\n";
      cout << "    exit\n";
    } else if (!strcmp(cmd, "quit")) {
      break;
    } else if (!strcmp(cmd, "exit")) {
      break;
    } else {
      cout << "unknown command: " << cmd << "\n";
      cout << "type 'help' for a list of commands.\n";
      result = 1;
    }
  }

  cout << "closing emulated disk.\n";
  disk.close();

  return 0;
}
