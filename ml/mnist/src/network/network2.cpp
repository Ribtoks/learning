#include "network2.h"

#include "network1.h"
#include <algorithm>
#include <numeric>
#include "common/calculus.h"
#include "common/log.h"

network2_t::network2_t(std::initializer_list<std::shared_ptr<layer_base_t<data_type> > > layers,
                       activator_t<data_type> const &activator):
    layers_(layers),
    activator_(activator)
{ }

network2_t::v_d network2_t::feedforward(network2_t::v_d a) {
    for (auto &layer: layers_) {
        a = layer->feedforward(a);
    }
    return a;
}

void network2_t::backpropagate(network2_t::v_d input, const network2_t::v_d &result) {
    const size_t layers_size = layers_.size();

    // feedforward input
    for (size_t i = 0; i < layers_size; i++) {
        input = layers_[i]->feedforward(input);
    }

    // backpropagate error
    network2_t::v_d error = result;
    for (size_t i = layers_size; i-- > 0;) {
        error = layers_[i]->backpropagate(error);
    }
}
