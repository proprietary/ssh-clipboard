#ifndef _INCLUDE_SSH_CLIPBOARD_POSIX_LISTENER_H
#define _INCLUDE_SSH_CLIPBOARD_POSIX_LISTENER_H

#include "listener.h"

namespace ssh_clipboard::listener {

  class PosixListener : public NativeListener {
  public:
    PosixListener();
    ~PosixListener() override;
    void start() override;
    void stop() override;
  private:
    int fd_;
    std::thread thread_;
    bool running_;
  };

} // namespace ssh_clipboard::listener

#endif // _INCLUDE_SSH_CLIPBOARD_POSIX_LISTENER_H
