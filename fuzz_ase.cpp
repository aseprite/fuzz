// Copyright (C) 2020-2022  Igara Studio S.A.

#include "base/ints.h"
#include "dio/aseprite_decoder.h"
#include "dio/decode_delegate.h"
#include "dio/file_interface.h"

#include <cassert>
#include <cstdlib>

class MemoryFileInterface : public dio::FileInterface {
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
  size_t readBytes(uint8_t* buf, size_t n) override {
    if (m_pos+n > m_size)
      n = m_size - m_pos;
    if (n <= 0)
      return 0;

    std::copy(m_data+m_pos,
              m_data+m_pos+n, buf);
    return n;
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

  dio::DecodeDelegate delegate;
  dio::AsepriteDecoder decoder;
  decoder.initialize(&delegate, &file);
  if (!decoder.decode())
    return 1;

  return 0;
}
