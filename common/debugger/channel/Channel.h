#ifndef RED_O_LATOR_CHANNEL_H
#define RED_O_LATOR_CHANNEL_H

#include <mutex>
#include <condition_variable>
#include <optional>

template<class T>
class Channel {
   private:
    std::condition_variable read_cond;
    std::condition_variable write_cond;
    std::mutex mutex;
    bool _isClosed = false;
    bool _isEmpty = true;
    std::optional<T> value;

   public:
    std::optional<T> read();
    std::optional<T> tryRead();
    bool write(const T& data);

    bool isExhausted();
    bool isClosed();
    bool isEmpty();

    void close();

    ~Channel();
};


template <class T>
std::optional<T> Channel<T>::read() {
    std::unique_lock<std::mutex> lock(mutex);
    read_cond.wait(lock, [&]() {
        return !_isEmpty || _isClosed;
    });

    if (_isEmpty && _isClosed) {
        return {};
    }
    T result = value.value();
    value.reset();
    _isEmpty = true;

    write_cond.notify_one();

    return result;
}

template <class T>
std::optional<T> Channel<T>::tryRead() {
    std::unique_lock<std::mutex> lock(mutex);

    if (_isEmpty || _isClosed) {
        return {};
    }
    T result = value.value();
    value.reset();
    _isEmpty = true;

    write_cond.notify_one();

    return result;
}

template <class T>
bool Channel<T>::write(const T& data) {
    std::unique_lock<std::mutex> lock(mutex);
    write_cond.wait(lock, [&]() {
        return _isEmpty || _isClosed;
    });

    if (_isClosed) {
        return false;
    }
    _isEmpty = false;
    value = data;

    read_cond.notify_one();

    return true;
}

template <class T>
void Channel<T>::close() {
    std::unique_lock<std::mutex> lock(mutex);
    _isClosed = true;
    read_cond.notify_all();
    write_cond.notify_all();
}

template <class T>
bool Channel<T>::isExhausted() {
    return _isClosed && _isEmpty;
}

template <class T>
bool Channel<T>::isClosed() {
    return _isClosed;
}

template <class T>
bool Channel<T>::isEmpty() {
    return _isEmpty;
}

template <class T>
Channel<T>::~Channel() {
    close();
}


#endif  // RED_O_LATOR_CHANNEL_H
