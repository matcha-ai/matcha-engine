#include "bits_of_matcha/engine/ops/Add.h"
#include "bits_of_matcha/engine/cpu/kernels/elementwiseBinaryBack.h"
#include "bits_of_matcha/engine/cpu/kernels/fill.h"

namespace matcha::engine::ops {

Add::Add(Tensor* a, Tensor* b)
 : ElementwiseBinaryOp(a, b)
{}

OpMeta<Add> Add::meta {
  .name = "Add",
  .back = [](auto& ctx) { return new AddBack(ctx); },
};

void Add::run() {
  runCpu(std::plus());
}


AddBack::AddBack(const BackCtx& ctx)
  : ElementwiseBinaryOpBack(ctx)
{}

OpMeta<AddBack> AddBack::meta {
  .name = "AddBack",
};

void AddBack::run() {
  if (outputs[0]) {
    cpu::fill(outputs[0]->malloc(), outputs[0]->size(), (float) 0);

    cpu::elementwiseBinaryBack(
      [](float& a, float& b, float& c) {
        a += c;
      },
      outputs[0]->buffer(),
      forward_->inputs[1]->buffer(),
      inputs[0]->buffer(),
      iter_
    );
  }
  if (outputs[1]) {
    cpu::fill(outputs[1]->malloc(), outputs[1]->size(), (float) 0);

    cpu::elementwiseBinaryBack(
      [](float& a, float& b, float& c) {
        b += c;
      },
      forward_->inputs[0]->buffer(),
      outputs[1]->buffer(),
      inputs[0]->buffer(),
      iter_
    );
  }
}

}