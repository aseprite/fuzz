// Copyright (C) 2020-2022  Igara Studio S.A.

#include "flic/flic.h"

#include <cassert>
#include <cstddef>
#include <cstdint>

class MemoryFileInterface : public flic::FileInterface {
public:
  MemoryFileInterface(const uint8_t* data, size_t size)
    : m_data(data)
    , m_size(size)
    , m_pos(0)
    , m_ok(true) {
  }
  bool ok() const override {
    return m_ok;
  }
  size_t tell() override { return m_pos; }
  void seek(size_t absPos) override {
    if (absPos >= m_size) {
      m_pos = m_size;
      m_ok = false;
    }
    else {
      m_pos = absPos;
      m_ok = true;
    }
  }
  uint8_t read8() override {
    if (!m_ok) return 0;
    if (m_pos >= m_size) {
      m_ok = false;
      return 0;
    }
    return m_data[m_pos++];
  }
  void write8(uint8_t value) override {
    // unused
    assert(false);
  }

private:
  const uint8_t* m_data;
  size_t m_size;
  size_t m_pos;
  bool m_ok;
};

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
  MemoryFileInterface file(data, size);
  flic::Decoder decoder(&file);
  flic::Header header;
  if (!decoder.readHeader(header))
    return 1;

  // TODO "header.width*header.height" can overflow/give a negative
  //      value and this is not obvious from the API client side.
  if (size_t(header.width) * size_t(header.height) > 1024*1024*1024) // No more than 1 GB
    return 0;

  std::vector<uint8_t> buffer(header.width * header.height);
  flic::Frame frame;
  frame.pixels = &buffer[0];
  frame.rowstride = header.width;

  for (int i=0; i<header.frames; ++i) {
    if (!decoder.readFrame(frame))
      return 2;
  }

  return 0;
}
