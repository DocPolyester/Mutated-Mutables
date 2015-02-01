// Copyright 2012 Olivier Gillet.
//
// Author: Olivier Gillet (ol.gillet@gmail.com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// 
// See http://creativecommons.org/licenses/MIT/ for more information.
//
// -----------------------------------------------------------------------------
//
// Basic ringbuffer template

#ifndef STMLIB_UTILS_RING_BUFFER_H_
#define STMLIB_UTILS_RING_BUFFER_H_

#include "stmlib/stmlib.h"

namespace stmlib {

template<typename T, size_t size>
class RingBuffer {
 public:
  RingBuffer() { }
  
  inline void Init() {
    read_ptr_ = write_ptr_ = 0;
  }
  
  inline size_t capacity() const { return size; }
  
  inline size_t writable() const {
    return (read_ptr_ - write_ptr_ - 1) % size;
  }
  
  inline size_t readable() const {
    return (write_ptr_ - read_ptr_) % size;
  }
  
  inline void Write(T v) {
    while (!writable());
    Overwrite(v);
  }
  
  inline void Overwrite(T v) {
    size_t w = write_ptr_;
    buffer_[w] = v;
    write_ptr_ = (w + 1) % size;
  }

  inline T Read() {
    while (!readable());
    return ImmediateRead();
  }
  
  inline T ImmediateRead() {
    size_t r = read_ptr_;
    T result = buffer_[r];
    read_ptr_ = (r + 1) % size;
    return result;
  }
  
  inline void Flush() {
    write_ptr_ = read_ptr_;
  }

 private:
  T buffer_[size];
  volatile size_t read_ptr_;
  volatile size_t write_ptr_;

  DISALLOW_COPY_AND_ASSIGN(RingBuffer);
};

}  // namespace stmlib

#endif   // STMLIB_UTILS_RING_BUFFER_H_
