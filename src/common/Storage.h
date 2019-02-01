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

#ifndef NINPOTEST_STORAGE_H
#define NINPOTEST_STORAGE_H

#include <cstring>
#include <utility>

namespace internal {
template <typename T, size_t Size = sizeof(T)> class Storage {
public:
  Storage() = default;
  Storage(const Storage &) = default;
  Storage(Storage &&) noexcept = default;

  void assign(const T &value) { ::new (&mData[0]) T(value); }

  void assign(T &&value) { ::new (&mData[0]) T(std::move(value)); }

  template <typename... Args> void emplace(Args... args) {
    ::new (&mData[0]) T(std::move(args)...);
  }

  Storage &operator=(const Storage &other) {
    std::memcpy(mData, other.mData, sizeof(T));
    return *this;
  }

  constexpr size_t size() const { return Size; }

  T *ptr() { return static_cast<T *>((void *)mData); }

  const T *ptr() const { return static_cast<const T *>((const void *)mData); }

  T &ref() { return *ptr(); }

  const T &ref() const { return *ptr(); }

  void destroy() { ref().T::~T(); }

private:
  char mData[Size];
};
} // namespace internal

#endif // NINPOTEST_STORAGE_H
