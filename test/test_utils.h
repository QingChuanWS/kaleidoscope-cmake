// (c) Copyright 2022 - 2025 Advanced Micro Devices, Inc. All Rights Reserved.

#pragma once
#include <bits/types/FILE.h>
#include <memory>
#include <sstream>
#include <string>

inline std::unique_ptr<std::istream>
get_input_stream(const std::string &input) {
  return std::make_unique<std::istringstream>(input);
}