#pragma once

#include "Direction.hpp"
#include "Model.hpp"



class Game;

class Agent
{
public:
    Agent(Game* game);

    // Should be called directly after move (before pellet is repositioned and snake is dead)
    void TeachModel();
    Direction GetNextDirection() const;

private:
    State GetState() const;
    Action GetAction() const;
    float GetReward() const;

    Action GetBestAction(const State& state);
    bool IsBestDirForState(Direction dir, const State& state);
    // Helper functions for GetBestAction() method below:
    //bool UpdateActionIfGood(const State& state, Action& action, Direction dir);
    //void GetDirsInOrder(Direction dirs[4], int dist_up, int dist_down, int dist_left, int dist_right);

private:
    Game* game = nullptr;
    Model model;

    State last_state;

    const float die_reward = -100.0;
    const float eat_reward = 10.0;
    const float nothing_reward = 0.0;
};