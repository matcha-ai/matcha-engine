#pragma once

#include "bits_of_matcha/tensor.h"
#include "bits_of_matcha/engine/fn.h"


namespace matcha {
namespace fn {
  Tensor multiply(const Tensor& a, const Tensor& b);
  UnaryFn multiplyWith(const Tensor& b);
  UnaryFn multiplyAgainst(const Tensor& a);
}
}


matcha::Tensor operator*(const matcha::Tensor& a, const matcha::Tensor& b);
const matcha::Tensor& operator*=(matcha::Tensor& a, const matcha::Tensor& b);


namespace matcha {
namespace engine {
namespace fn {


class Multiply : public Fn {
  public:
    Multiply(Tensor* a, Tensor* b);
    Multiply(const matcha::Tensor& a, const matcha::Tensor& b);
};


}
}
}
