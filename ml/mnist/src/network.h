#ifndef NETWORK_H
#define NETWORK_H

#include <initializer_list>
#include <vector>
#include <tuple>
#include "calculus_types.h"

class network_t {
public:
    using v_d = vector_t<double>;
    using m_d = matrix_t<double>;
    using training_data = std::vector<std::tuple<v_d, v_d>>;
    
public:
    network_t(std::initializer_list<int> layers);

public:
    // train network using stochastic gradient descent
    void train_sgd(const training_data &data,
                   size_t epochs,
                   size_t mini_batch_size,
                   double eta);

private:
    // evaluates number of correct classified inputs 
    size_t evaluate(const training_data &data, const std::vector<size_t> &indices) const;
    // feeds input a to the network and returns output
    v_d feedforward(v_d a) const;
    // updates network weights and biases using one
    // iteration of gradient descent using mini_batch of inputs and outputs
    void update_mini_batch(const training_data &data,
                           const std::vector<size_t> &indices,
                           double eta);

    void backpropagate(const v_d &input,
                       const v_d &result,
                       std::vector<v_d> &nabla_b,
                       std::vector<m_d> &nabla_w);

    v_d &activate(v_d &z) const;
    v_d &activation_derivative(v_d &z) const;
    v_d cost_derivative(const v_d &actual, const v_d &expected) const;
    
private:
    //  dimensions of layers
    std::vector<int> layers_;
    // bias(i) is a vector of biases of neurons in layer (i)
    std::vector<v_d> biases_;
    // weight(i) is a matrix of weights between layer (i) and (i + 1)
    std::vector<m_d> weights_;
};

#endif // NETWORK_H
