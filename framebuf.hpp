#pragma once

#include <cstdint>
#include <memory>
#include <stdexcept>

#include "format.hpp"

constexpr size_t FrameBufferMaxSize = 8192 * 8192 * 4;

struct FrameBuffer {

  FrameBuffer() = default;
  inline FrameBuffer(
    uint32_t height,
    uint32_t width,
    SpaVideoFormat_e const& format
  ){
    update_param(height, width, format);
  }
  
  inline void update_param(
    uint32_t height,
    uint32_t width,
    SpaVideoFormat_e const& format
  ){
    if (data.get() == nullptr) {
      data.reset(new uint8_t[FrameBufferMaxSize]);
    }

    int bytes_per_pixel = spa_videoformat_bytesize(format);
    if (bytes_per_pixel == -1) {
      throw std::runtime_error("Invalid format");
    }

    // always store in (height, width):(stride, 1) layout
    uint32_t needed_stride = (width * bytes_per_pixel + 4 - 1) / 4 * 4;
    uint32_t needed_size = height * needed_stride;
    if (needed_size > data_size) {
      data_size = needed_size;
    }
    assert(data_size <= FrameBufferMaxSize);
    this->height = height;
    this->width = width;
    this->row_byte_stride = needed_stride;
    this->format = format;
  }

  std::unique_ptr<uint8_t[]> data{nullptr};
  size_t data_size{0};
  uint32_t height{0};
  uint32_t width{0};
  uint32_t row_byte_stride{0};
  SpaVideoFormat_e format{SpaVideoFormat_e::INVALID};

};
