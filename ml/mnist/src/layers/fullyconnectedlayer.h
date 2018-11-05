#ifndef FULLY_CONNECTED_LAYER_H
#define FULLY_CONNECTED_LAYER_H

#include <functional>

#include "strategy/train_strategy.h"
#include "common/array3d.h"
#include "common/array3d_math.h"
#include "common/log.h"
#include "network/activator.h"
#include "layers/layer_base.h"

template<typename T = double>
class fully_connected_layer_t: public layer_base_t<T> {
public:
    fully_connected_layer_t(size_t layer_in,
                            size_t layer_out,
                            activator_t<T> const &activator):
        dimension_(layer_out),
        weights_(
            shape_matrix(layer_out, layer_in),
            T(0), T(1)/sqrt((T)layer_in)),
        bias_(
            shape_row(layer_out),
            T(0), T(1)),
        z_(shape_row(layer_out), 0),
        a_prev_(shape_row(layer_in), 0),
        nabla_w_(shape_matrix(layer_out, layer_in), 0),
        nabla_b_(shape_row(layer_out), 0),
        activator_(activator)
    { }

public:
    virtual array3d_t<T> feedforward(array3d_t<T> const &input) override {
        a_prev_ = input.clone();
        // z = w*a + b
        z_ = dot21(weights_, a_prev_); z_.add(bias_);
        return activator_.activate(z_);
    }

    virtual array3d_t<T> backpropagate(array3d_t<T> const &error) override {
        array3d_t<T> delta, delta_nabla_w;
        // delta(l) = (w(l+1) * delta(l+1)) [X] derivative(z(l))
        // (w(l+1) * delta(l+1)) comes as the gradient (error) from the "previous" layer
        delta = activator_.derivative(z_); delta.element_mul(error);
        // dC/db = delta(l)
        nabla_b_.add(delta);
        // dC/dw = a(l-1) * delta(l)
        delta_nabla_w = outer_product(delta, a_prev_);
        nabla_w_.add(delta_nabla_w);
        // gradient for the next layer w(l) * delta(l)
        return transpose_dot21(weights_, delta);
    }

    virtual void update_weights(train_strategy_t<T> const &strategy) override {
        strategy.update_bias(bias_, nabla_b_);
        strategy.update_weights(weights_, nabla_w_);
		nabla_b_.reset(0);
		nabla_w_.reset(0);
    }

private:
    // own data
    size_t dimension_;
    array3d_t<T> weights_;
    array3d_t<T> bias_;
    activator_t<T> const &activator_;
    // calculation support
    array3d_t<T> z_, a_prev_;
    array3d_t<T> nabla_w_;
    array3d_t<T> nabla_b_;
};

#endif // FULLY_CONNECTED_LAYER_H
