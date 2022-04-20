#include <matcha/matcha>


tensor params = tensor::ones(3, 3);
tensor another = 3;
tensor more = 0;

auto flow = (matcha::Flow) [](const tensor& a) {
  tensor b = a + another + params.dot(a);
  auto c = b + a;
  return (c.t().dot(params) + more).t() + params.t() + (another + more);
};

int main() {
//  flow.requireGrad(another);
//  flow.requireGrad(params);
//  flow.requireGrad(more);
//  tensor a = tensor::ones(3, 3);
//  flow(a);

  auto rand = matcha::random::Uniform {
    .a = 10,
    .b = 100,
    .seed = 3,
  };
  tensor x = rand(5).t().t();
  print(x);

  return 0;
}
