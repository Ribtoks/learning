#include <memory>
#include <string>
#include <initializer_list>

#include "strategy/sdg_strategy.h"
#include "common/array3d.h"
#include "common/array3d_math.h"
#include "network/activator.h"
#include "layers/crossentropyoutputlayer.h"
#include "layers/fullyconnectedlayer.h"
#include "layers/convolutionlayer.h"
#include "layers/poolinglayer.h"
#include "network/network2.h"
#include "parsing/mnist_dataset.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        throw std::runtime_error("Data root not specified through the command line");
    }
    
    std::string data_root(argv[1]);
    mnist_dataset_t mnist_dataset(data_root);

    size_t mini_batch_size = 10;
    double learning_rate = 0.1;
    double decay_rate = 20.0;

    auto training_data = mnist_dataset.training_data();
    activator_t<double> sigmoid_activator(sigmoid_v, sigmoid_derivative_v);
    // derivative returns 1 because it is cancelled out when using cross-entropy
    activator_t<double> softmax_activator(stable_softmax_v,
                                          [](array3d_t<double> const &x){
        return array3d_t<double>(shape_row(x.size()), 1.0);});
    activator_t<double> relu_activator(relu_v, relu_v);
    sdg_strategy_t<double> sdg_strategy(mini_batch_size,
                                        training_data.size(),
                                        decay_rate,
                                        learning_rate);
    network2_t network(
                std::initializer_list<network2_t::layer_type>(
    {
                        std::make_shared<convolution_layer_t<double>>(
                        shape_matrix(28, 28), // input size
                        shape_matrix(5, 5), // filter size
                        20, // number of filters
                        1, // stride length
                        padding_type::valid,
                        relu_activator),
                        std::make_shared<pooling_layer_t<double>>(
                        2, // window_size
                        1), // stride length
                        std::make_shared<fully_connected_layer_t<double>>(20*12*12, 100, sigmoid_activator),
                        std::make_shared<fully_connected_layer_t<double>>(100, 10, softmax_activator),
                        std::make_shared<crossentropy_output_layer_t<double>>()}));

    size_t epochs = 60;

    network.train(training_data,
                  sdg_strategy,
                  epochs,
                  mini_batch_size);
    
    return 0;
}
