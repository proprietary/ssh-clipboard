#include "ssh_clipboard.h"

int main(int argc, char *argv[], char *envp[]) {
  ssh_clipboard::copy_to_clipboard();
  return 0;
}
