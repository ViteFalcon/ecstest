/*
------------------------------------------------------------------------------------------------------------------------
Copyright 2019 Vite Falcon

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
------------------------------------------------------------------------------------------------------------------------
*/

#ifndef NINPOTEST_OPTIONAL_H
#define NINPOTEST_OPTIONAL_H

#include "None.h"
#include "Storage.h"

#include <functional>
#include <stdexcept>
#include <utility>

template <typename T> class Optional {
public:
  Optional();
  explicit Optional(const None &none);
  explicit Optional(const T &val);
  explicit Optional(T &&val) noexcept;

  Optional(const Optional &) = default;
  Optional(Optional &&) noexcept = default;

  template <typename... Args>
  explicit Optional(Args &&... args) : mInitialized(false) {
    assign(std::move(args)...);
  }

  ~Optional();

  explicit operator bool() const { return mInitialized; }

  T &operator*() { return value(); }

  const T &operator*() const { return value(); }

  T &value() { return mStorage.ref(); }

  const T &value() const { return mStorage.ref(); }

  template <typename... Args> void emplace(Args &&... args) {
    assign(std::move(args)...);
  }

  Optional &operator=(const None &none) {
    mInitialized = false;
    return *this;
  }

  Optional &operator=(const T &val) {
    assign(val);
    return *this;
  }

  Optional &operator=(T &&val) {
    assign(std::move(val));
    return *this;
  }

  Optional &operator=(const Optional &) = default;

  Optional &operator=(Optional &&) = default;

  bool isInitialized() const { return mInitialized; }

  T value_or(T defaultValue);

  T value_or(T defaultValue) const;

  T value_or_eval(std::function<T()> valueRetriever);

  T value_or_eval(std::function<T()> valueRetriever) const;

  template <typename ExceptionType, typename = std::enable_if<std::is_base_of<
                                        std::exception, ExceptionType>::value>>
  T value_or_throw(std::function<ExceptionType()> exceptionSupplier) {
    if (!mInitialized) {
      throw exceptionSupplier();
    }
    return value();
  }

  template <typename ExceptionType, typename = std::enable_if<std::is_base_of<
                                        std::exception, ExceptionType>::value>>
  T value_or_throw(std::function<ExceptionType()> exceptionSupplier) const {
    if (!mInitialized) {
      throw exceptionSupplier();
    }
    return value();
  }

  static Optional<T> empty() { return Optional<T>(None::Instance); }

private:
  void assign(const T &val);

  void assign(T &&val);

  template <typename... Args> void assign(Args &&... args) {
    destroy();
    mStorage.emplace(std::move(args)...);
    mInitialized = true;
  }

  void destroy() {
    if (!mInitialized) {
      return;
    }
    mStorage.destroy();
    mInitialized = false;
  }

private:
  bool mInitialized;
  internal::Storage<T> mStorage;
};

template <typename T> Optional<T>::Optional() : mInitialized(false) {}

template <typename T>
Optional<T>::Optional(const None &none) : mInitialized(false) {}

template <typename T>
Optional<T>::Optional(const T &val) : mInitialized(false) {
  assign(val);
}

template <typename T>
Optional<T>::Optional(T &&val) noexcept : mInitialized(false) {
  assign(std::forward<T>(val));
}

template <typename T> Optional<T>::~Optional() { destroy(); }

template <typename T> T Optional<T>::value_or(T defaultValue) {
  return mInitialized ? value() : defaultValue;
}

template <typename T> T Optional<T>::value_or(T defaultValue) const {
  return mInitialized ? value() : defaultValue;
}

template <typename T>
T Optional<T>::value_or_eval(std::function<T()> valueRetriever) {
  return mInitialized ? value() : valueRetriever();
}

template <typename T>
T Optional<T>::value_or_eval(std::function<T()> valueRetriever) const {
  return mInitialized ? value() : valueRetriever();
}

template <typename T> void Optional<T>::assign(const T &val) {
  destroy();
  mStorage.assign(val);
  mInitialized = true;
}

template <typename T> void Optional<T>::assign(T &&val) {
  destroy();
  mStorage.assign(std::move(val));
  mInitialized = true;
}

#endif // NINPOTEST_OPTIONAL_H
