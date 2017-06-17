#include <cstdio>
#include <cstdlib>

#include <algorithm>
#include <iostream>
#include <vector>

float normalized_random() {
    return rand() / static_cast<float>(RAND_MAX);
}

class Node {
    public:
        Node(float probability_retransmit,
             float probability_new_frame) :
            probability_retransmit(probability_retransmit),
            probability_new_frame(probability_new_frame),
            has_frame(false)
        {
        }

    public:

        bool would_transmit() {
            if (has_frame) {
                return normalized_random() < probability_retransmit;
            }

            return (has_frame = (normalized_random() < probability_new_frame));
        }

        void clear() {
            has_frame = false;
        }

    private:

        const float probability_retransmit;
        const float probability_new_frame;
        bool has_frame;
};

float aloha_simulation(size_t n_nodes,
                       float probability_retransmit,
                       float probability_new_frame,
                       size_t num_simulations) {
    std::vector<Node> nodes(n_nodes, Node(probability_retransmit,
                                          probability_new_frame));

    size_t n_successful = 0;
    /* For each simulation, we assume that either two people transmit
     * at the same time (collision) or we get lucky and only one node
     * transmits */
    for (size_t i = 0; i < num_simulations; ++i) {
        size_t n_transmitting = 0;
        for (Node &node : nodes) {
            if (node.would_transmit()) {
                ++n_transmitting;
            }
        }

        if (n_transmitting == 1) {
            std::for_each(nodes.begin(), nodes.end(), [](Node &node) {
                node.clear();
            });

            ++n_successful;
        }
    }

    return num_simulations / static_cast<float>(n_successful);
}

void usage() {
    std::cout << "aloha N_NODES PROB_RETRANSMIT "
              << "PROB_NEW_FRAME NUM_SIMULATIONS" << std::endl;
}

int main(int argc, char **argv) {
    if (argc != 5) {
        usage();
        exit(1);
    }

    size_t n_nodes = atoi(argv[1]);
    float probability_retransmit = atof(argv[2]);
    float probability_new_frame = atof(argv[3]);
    size_t num_simulations = atof(argv[4]);

    std::cout << "Aloha (n nodes "
              << n_nodes
              << ") (n simulations "
              << num_simulations
              << ") (prob retransmit "
              << probability_retransmit
              << ") (prob_new_frame "
              << probability_new_frame
              << ")"
              << std::endl;

    std::cout << "Utilization: " << aloha_simulation(n_nodes,
                                                     probability_retransmit,
                                                     probability_new_frame,
                                                     num_simulations)
                                 << std::endl;

    return 0;
}