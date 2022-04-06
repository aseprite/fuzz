// Copyright (C) 2022  Igara Studio S.A.

#include "json11.hpp"

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
  std::string err;
  auto json = json11::Json::parse((const char*)data, size, err);
  return 0;
}
