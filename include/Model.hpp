#pragma once

#include <limits>



struct State
{
    // Values of given booleans from up to down: 1,2,4,..,512,1024,2048

    // Can move in these directions
    bool MOVE_UP = 0;
    bool MOVE_DOWN = 0;
    bool MOVE_LEFT = 0;
    bool MOVE_RIGHT = 0;

    // The closest directions to pellet
    bool PELLET_UP = 0;
    bool PELLET_DOWN = 0;
    bool PELLET_LEFT = 0;
    bool PELLET_RIGHT = 0;

    // Die when move in these directions
    bool DIE_UP = 0;
    bool DIE_DOWN = 0;
    bool DIE_LEFT = 0;
    bool DIE_RIGHT = 0;
};

struct Action
{
    // Values of given booleans from up to down: 1,2,4,8

    // Move in the one of these directions
    bool MOVE_UP = 0;
    bool MOVE_DOWN = 0;
    bool MOVE_LEFT = 0;
    bool MOVE_RIGHT = 0;
};

// Model for the reinforcement learning that uses Q-learning algorithm
class Model
{
public:
    Model();
    void Learn(const State& state, const Action& action, const float& reward, const State& new_best_state, const Action& new_best_action);
    Action BestAction(const State& state) const;

private:
    // Checks if snake rule isn't ignored (snake can't move in the opposite direction)
    // DISABLED because I think AI will see reward for going backward (because of dying in result) and won't choose this action
    //bool CanPerformAction(const State& state, const Action& action);

    // Helper methods to transfer beetwen Q-table values and states/actions
    int StateToIndex(const State& state) const;
    int ActionToIndex(const Action& action) const;
    Action IndexToAction(int index) const;

private:
    float Q[4096][16];   // 2^12 different states, 2^4 different actions

    const float learning_rate = 0.3;
    const float discount_factor = 0.8;
    //const float discount_factor_decrease = 0.01;
};