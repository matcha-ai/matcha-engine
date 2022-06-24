#include "bits_of_matcha/nn/Net.h"
#include "bits_of_matcha/nn/Layer.h"


namespace matcha::nn {

Net::Net(const AnyOp& function)
  : function_(function)
{}

Net::Net(const std::vector<UnaryOp>& sequence) {
  function_ = [sequence] (tensor feed) {
    for (auto& op: sequence) feed = op(feed);
    return feed;
  };
}

Net::Net(std::initializer_list<UnaryOp> sequence)
  : Net(std::vector(sequence))
{}

void Net::fit(Dataset ds) {
  Layer::netStack_.push(this);
  trainFlow_ = function_;
  trainFlow_.build({ds.get()["x"]});
  ds.reset();

  initCallbacks();

  int epochs = 10;
  for (int epoch = 0; epoch < epochs; epoch++) {
    epochBegin(epoch, epochs);
    int batch = 0;
    for (auto i: ds) {
      batchBegin(batch, ds.size());
      tensor x = i["x"];
      tensor t = i["y"];
      Backprop backprop(params);
//      print(88 * (x.reshape(28, 28) != 0), "\n\n");
      tensor y = trainFlow_(x);
//      print(y);
      tensor l = loss(t, y);
//      for (auto&& [param, grad]: backprop(l)) {
//        optimizer(*param, grad);
//      }
      batchEnd(batch, (int) ds.size());
      batch++;
    }
    epochEnd(epoch, epochs);
  }
  if (Layer::netStack_.top() != this)
    throw std::runtime_error("net stack corruption");
  Layer::netStack_.pop();
}

void Net::initCallbacks() {
  for (auto&& cb: callbacks) if (cb) cb->init(this);
}

void Net::epochBegin(size_t epoch, size_t max) {
  for (auto&& cb: callbacks) if (cb) cb->onEpochBegin(epoch, max);
}

void Net::epochEnd(size_t epoch, size_t max) {
  for (auto&& cb: callbacks) if (cb) cb->onEpochEnd(epoch, max);
}

void Net::batchBegin(size_t batch, size_t max) {
  for (auto&& cb: callbacks) if (cb) cb->onBatchBegin(batch, max);
}

void Net::batchEnd(size_t batch, size_t max) {
  for (auto&& cb: callbacks) if (cb) cb->onBatchEnd(batch, max);
}

void Net::Params::add(tensor* tensor) {
  tensors_.insert(tensor);
}

std::_Rb_tree_const_iterator<tensor*> Net::Params::begin() {
  return tensors_.begin();
}

std::_Rb_tree_const_iterator<tensor*> Net::Params::end() {
  return tensors_.end();
}

std::_Rb_tree_const_iterator<tensor*> Net::Params::begin() const {
  return tensors_.begin();
}

std::_Rb_tree_const_iterator<tensor*> Net::Params::end() const {
  return tensors_.end();
}

size_t Net::Params::size() const {
  return tensors_.size();
}

size_t Net::Params::total() const {
  size_t total = 0;
  for (auto&& t: tensors_) total += t->size();
  return total;
}


}