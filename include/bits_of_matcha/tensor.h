#pragma once

#include "bits_of_matcha/Frame.h"
#include "bits_of_matcha/macros/vararg_shape.h"

#include <iostream>

#define MATCHA_TENSOR_API alignas(void*)


namespace matcha {

/**
 * multidimensional array
 * matcha primitive
 */
class MATCHA_TENSOR_API tensor {
public:

  /**
   * @return the tensor frame
   */
   const Frame& frame() const;

  /**
   * @return the tensor dtype
   */
  const Dtype& dtype() const;

  /**
   * @return the tensor shape
   */
  const Shape& shape() const;

  size_t size() const;
  size_t rank() const;

  /**
   * @returns tensor reshaped to specified dimensions
   * @see matcha::reshape
   */
  template <class... Dims>
  tensor reshape(Dims... dims) const { return reshape(VARARG_RESHAPE(dims...)); };

  /**
   * @returns tensor reshaped to specified dimensions
   * @see matcha::reshape
   */
  tensor reshape(const Shape::Reshape& dims) const;

  /**
   * @returns tensor transpose
   */
  tensor transpose() const;

  /**
   * @returns tensor transpose
   */
  tensor t() const;

  /**
   * @param b the second tensor
   * @return the dot product of two tensors
   */
  tensor dot(const tensor& b) const;

  /**
   * @param b the second tensor
   * @return concatenation of two tensors
   */
  tensor cat(const tensor& b) const;

  /**
   * @param b exponent
   * @return tensor to the power of b
   */
  tensor pow(const tensor& b) const;


public:
  tensor();
  tensor(float scalar);

  /**
   * @brief Float tensor filled with value
   * @param shape target shape
   * @return tensor of given shape filled with specified value
   */
  static tensor full(float value, const Shape& shape);

  /**
   * @brief The zero tensor
   * @param shape target shape
   * @return tensor of specified shape full of zeros
   */
  static tensor zeros(const Shape& shape);

  /**
   * @brief The ones tensor
   * @param shape target shape
   * @return tensor of specified shape full of ones
   */
  static tensor ones(const Shape& shape);

  /**
   * The identity tensor
   * @return slice of the identity matrix of given shape
   */
  static tensor eye(const Shape& shape);

  /**
   * @brief The zero tensor
   * @param dims target dimensions
   * @return tensor of specified shape full of zeros
   */
  template <class... Dims>
  static inline tensor zeros(Dims... dims) { return zeros(VARARG_SHAPE(dims...)); }

  /**
   * @brief The ones tensor
   * @param dims target dimensions
   * @return tensor of specified shape full of ones
   */
  template <class... Dims>
  static inline tensor ones(Dims... dims) { return ones(VARARG_SHAPE(dims...)); }

  /**
   * @brief The identity tensor
   * @param dims target dimensions
   * @return slice of the identity matrix of given shape
   */
  template <class... Dims>
  static inline tensor eye(Dims... dims) { return eye(VARARG_SHAPE(dims...)); }

public:
  tensor(const tensor& other);
  tensor(tensor&& other) noexcept;
  tensor& operator=(const tensor& other);
  tensor& operator=(tensor&& other);
  ~tensor();


public:
  /**
   * @brief tensor data
   * @return pointer to tensor data
   * @warning forbidden inside the `Flow`
   */
  void* data();

private:
  friend class Engine;
  explicit tensor(void* internal);
  void* internal_;
};


template <class... Dims>
static inline tensor zeros(Dims... dims) { return tensor::zeros(dims...); }

template <class... Dims>
static inline tensor ones(Dims... dims) { return tensor::ones(dims...); }

template <class... Dims>
static inline tensor eye(Dims... dims) { return tensor::eye(dims...); }

static tensor full(float value, const Shape& shape) { return tensor::full(value, shape); }
static tensor zeros(const Shape& shape) { return tensor::zeros(shape); }
static tensor ones(const Shape& shape) { return tensor::ones(shape); }
static tensor eye(const Shape& shape) { return tensor::eye(shape); }

}

std::ostream& operator<<(std::ostream& os, const matcha::tensor& t);