#include <mlpack.hpp>
#include <mlpack/methods/reinforcement_learning/reinforcement_learning.hpp>
#include <mlpack/methods/ann/ffn.hpp>
#include <ensmallen.hpp>

int main(int argc, char** argv)
{
  // This is a simple test to ensure that the library can be included and
  // compiled. We don't need to do anything complex here; a simple agent
  // definition is sufficient to trigger template instantiations.
  using namespace mlpack;
  using namespace mlpack::rl;
  using namespace mlpack::ann;

  // Define the network.
  FFN<MeanSquaredError<>> network;
  network.Add<Linear>(CartPole::State::dimension, 128);
  network.Add<ReLU>();
  network.Add<Linear>(128, CartPole::Action::size);

  // Define the policy.
  GreedyPolicy<CartPole> policy(0.1, 1000, 0.1);

  // Define the replay method.
  RandomReplay<CartPole> replayMethod(10, 10000);

  // Define training config.
  TrainingConfig config;

  // Define the optimizer
  ens::Adam updater;

  // Define the Q-Learning agent.
  QLearning<CartPole, decltype(network), ens::Adam, decltype(policy)> agent(config, network, policy, replayMethod, updater);

  return 0;
}