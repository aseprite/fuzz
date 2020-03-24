// Copyright (C) 2020  Igara Studio S.A.

#include "tga/tga.h"

#include <cassert>
#include <cstddef>
#include <cstdint>

class MemoryFileInterface : public tga::FileInterface {
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
  tga::Decoder decoder(&file);
  tga::Header header;
  if (!decoder.readHeader(header))
    return 1;

  tga::Image image;
  image.bytesPerPixel = header.bytesPerPixel();
  image.rowstride = header.width * header.bytesPerPixel();
  std::vector<uint8_t> buffer(image.rowstride * header.height);
  image.pixels = &buffer[0];

  if (!decoder.readImage(header, image, nullptr))
    return 2;

  return 0;
}
