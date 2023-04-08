#include "Model.hpp"



Model::Model()
{
    for(int i = 0; i < 4096; i++)
    {
        for(int j = 0; j < 16; j++)
        {
            Q[i][j] = 0.0f;
        }
    }
}

void Model::Learn(const State& state, const Action& action, const float& reward, const State& new_best_state, const Action& new_best_action)
{
    // Get value from Q-table for given state and action
    float& value = Q[StateToIndex(state)][ActionToIndex(action)];

    const float& best_new_value = Q[StateToIndex(new_best_state)][ActionToIndex(new_best_action)];

    // Update value by computing this equation (it's based on Bellman equation)
    value += learning_rate*(reward + discount_factor*best_new_value - value);
}

Action Model::BestAction(const State& state) const
{
    // Get table of actions for current state
    const float* actions = Q[StateToIndex(state)];
    float best_action_value = actions[0];
    int best_action_index = 0;

    // Find the best action
    for(int i = 1; i < 16; i++)
    {
        if(actions[i] > best_action_value)  /*&& CanPerformAction(state, IndexToAction(i))*/
        {
            best_action_value = actions[i];
            best_action_index = i;
        }
    }
    return IndexToAction(best_action_index);
}

/*
bool Model::CanPerformAction(const State& state, const Action& action)
{
    if(state.MOVE_UP == 1)
    {
        if(action.MOVE_DOWN == 1)
            return false;
        else
            return true;
    }
    else if(state.MOVE_DOWN == 1)
    {
        if(action.MOVE_UP == 1)
            return false;
        else
            return true;
    }
    else if(state.MOVE_LEFT == 1)
    {
        if(action.MOVE_RIGHT == 1)
            return false;
        else
            return true;
    }
    else if(state.MOVE_RIGHT == 1)
    {
        if(action.MOVE_LEFT == 1)
            return false;
        else
            return true;
    }
    // TODO: Decide if should handle it by assert (this should not happen but still have to handle it)
}
*/

int Model::StateToIndex(const State& state) const
{
    int index = 0;

    if(state.MOVE_UP)
        index += 1;
    if(state.MOVE_DOWN)
        index += 2;
    if(state.MOVE_LEFT)
        index += 4;
    if(state.MOVE_RIGHT)
        index += 8;

    if(state.PELLET_UP)
        index += 16;
    if(state.PELLET_DOWN)
        index += 32;
    if(state.PELLET_LEFT)
        index += 64;
    if(state.PELLET_RIGHT)
        index += 128;

    if(state.DIE_UP)
        index += 256;
    if(state.DIE_DOWN)
        index += 512;
    if(state.DIE_LEFT)
        index += 1024;
    if(state.DIE_RIGHT)
        index += 2048;
    
    return index;
}

int Model::ActionToIndex(const Action& action) const
{
    int index = 0;

    if(action.MOVE_UP)
        index += 1;
    if(action.MOVE_DOWN)
        index += 2;
    if(action.MOVE_LEFT)
        index += 4;
    if(action.MOVE_RIGHT)
        index += 8;
    
    return index;
}

Action Model::IndexToAction(int index) const
{
    bool binary[4];

    int i = 0;
    while(index > 0)
    {
        binary[i] = (index%2 == 1);
        index /= 2;
        i++;
    }

    Action action;
    action.MOVE_UP = binary[0];
    action.MOVE_DOWN = binary[1];
    action.MOVE_LEFT = binary[2];
    action.MOVE_RIGHT = binary[3];

    return action;
}