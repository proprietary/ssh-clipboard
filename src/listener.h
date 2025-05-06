#ifndef _INCLUDE_SSH_CLIPBOARD_LISTENER_H
#define _INCLUDE_SSH_CLIPBOARD_LISTENER_H

#include <mutex>
#include <condition_variable>
#include <queue>
#include <memory>

namespace ssh_clipboard::listener {

  class NativeListener {
  public:
    virtual ~NativeListener() {}
    virtual void start() = 0;
    virtual void stop() = 0;
  };

  auto create_native_listener() -> std::unique_ptr<NativeListener>;

template <typename T>
  class Queue {
  public:
    void push(const T& value) {
      {
        std::lock_guard<std::mutex> lock{mutex_};
        queue_.push(value);
      }
      cv_.notify_one();
    }

    T pop() {
      std::unique_lock<std::mutex> lock{mutex_};
      cv_.wait(lock, [this]() { return !queue_.empty(); });
      T value = queue_.front();
      queue_.pop();
      return value;
    }
  private:
    std::queue<T> queue_;
    std::mutex mutex_;
    std::condition_variable cv_;
  };


} // namespace ssh_clipboard::listener

#endif // _INCLUDE_SSH_CLIPBOARD_LISTENER_H
