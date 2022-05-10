#include "bits_of_matcha/engine/tensor/Tensor.h"
#include "bits_of_matcha/engine/tensor/TensorCtx.h"
#include "bits_of_matcha/engine/tensor/iterations.h"
#include "bits_of_matcha/engine/memory/memory.h"
#include "bits_of_matcha/engine/flow/Tracer.h"
#include "bits_of_matcha/Engine.h"
#include "bits_of_matcha/print.h"

#include <sstream>


namespace matcha::engine {

Tensor::Tensor(const Frame& frame, Op* op)
  : frame_(frame)
  , ctx_(this)
  , buffer_(nullptr)
  , op_(op)
{
//  print("created tensor ", this);
  Tracer::handleNewTensor(this);
}

Tensor::Tensor(const Dtype& dtype, const Shape& shape, Op* op)
  : Tensor(Frame{dtype, shape}, op)
{}

Tensor::~Tensor() {
//  print("destroying ", this, " (", frame_.string(), ")");
  free();
}

const Frame& Tensor::frame() const {
  return frame_;
}

const Dtype& Tensor::dtype() const {
  return frame_.dtype();
}

const Shape& Tensor::shape() const {
  return frame_.shape();
}

size_t Tensor::rank() const {
  return shape().rank();
}

size_t Tensor::size() const {
  return shape().size();
}

size_t Tensor::bytes() const {
  return dtype().size() * size();
}

Buffer* Tensor::buffer() {
  return buffer_;
}

Buffer* Tensor::malloc() {
  if (buffer_) return buffer_;
  buffer_ = engine::malloc(bytes());
  buffer_->bind();
  return buffer_;
}

TensorCtx& Tensor::ctx() {
  return ctx_;
}

Op* Tensor::op() {
  return op_;
}

void Tensor::setOp(Op* op) {
  op_ = op;
}

void Tensor::shareBuffer(Buffer* buffer) {
  if (buffer == buffer_) return;
  if (buffer_) buffer_->unbind();
  buffer_ = buffer;
  if (buffer_) buffer->bind();
}

void Tensor::shareBuffer(Tensor* tensor) {
  shareBuffer(tensor->buffer());
}

void Tensor::free() {
  if (buffer_) buffer_->unbind();
  buffer_ = nullptr;
}

void* Tensor::readData() {
  if (Tracer::current()) {
    throw std::runtime_error("reading tensor data directly inside the Flow is forbidden");
  }
  if (!buffer_) return nullptr;
  return buffer_->payload();
}

tensor ref(Tensor* internal) {
  return Engine::ref(internal);
}

Tensor* unref(tensor& external) {
  return Engine::unref(external);
}

Tensor* unref(tensor* external) {
  return Engine::unref(external);
}

Tensor* deref(const tensor* external) {
  return Engine::deref(external);
}

Tensor* deref(const tensor& external) {
  return Engine::deref(external);
}

}