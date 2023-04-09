#include "Agent.hpp"
#include "Game.hpp"
#include "Utilities.hpp"
#include <cassert>
#include <random>
#include <iostream>



Agent::Agent(Game* game): game(game)
{
}

void Agent::TeachModel()
{
    State new_state = GetState();

    model.Learn(last_state, GetAction(), GetReward(), new_state, GetBestAction(new_state));

    last_state = new_state;
}

Direction Agent::GetNextDirection() const
{
    // Get best action to current state
    Action action = model.BestAction(GetState());

    //if(action.MOVE_UP == 1 || action.MOVE_DOWN == 1 || action.MOVE_LEFT == 1 || action.MOVE_RIGHT == 1)
    //    std::cerr << "AI made it!" << std::endl;
    
    // Get the best direction to move
    if(action.MOVE_UP == 1)
        return Direction::UP;
    if(action.MOVE_DOWN == 1)
        return Direction::DOWN;
    if(action.MOVE_LEFT == 1)
        return Direction::LEFT;
    if(action.MOVE_RIGHT == 1)
        return Direction::RIGHT;

    // Action is unknown - there is no best action for current state at the moment
    // In this case get random direction to move
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0,3);

    int rand_num = dist(rng);
    if(rand_num == 0)
        return Direction::UP;
    else if(rand_num == 1)
        return Direction::DOWN;
    else if(rand_num == 2)
        return Direction::LEFT;
    else if(rand_num == 3)
        return Direction::RIGHT;
    
    assert(0 && "This should never happen");
}

State Agent::GetState() const
{
    // Get references
    const Snake& snake = game->GetSnake();
    const Pellet& pellet = game->GetPellet();

    // Initalize state from game status
    State state;

    // Initalize state MOVE_XXX and DIE_XXX
    if(snake.GetMoveDir() == Direction::UP)
    {
        state.MOVE_UP = 1;
        state.MOVE_LEFT = 1;
        state.MOVE_RIGHT = 1;

        state.DIE_DOWN = 1;
        state.DIE_UP = snake.WillCollide(Direction::UP);
        state.DIE_LEFT = snake.WillCollide(Direction::LEFT);
        state.DIE_RIGHT = snake.WillCollide(Direction::RIGHT);
    }
    else if(snake.GetMoveDir() == Direction::DOWN)
    {
        state.MOVE_DOWN = 1;
        state.MOVE_LEFT = 1;
        state.MOVE_RIGHT = 1;

        state.DIE_UP = 1;
        state.DIE_DOWN = snake.WillCollide(Direction::DOWN);
        state.DIE_LEFT = snake.WillCollide(Direction::LEFT);
        state.DIE_RIGHT = snake.WillCollide(Direction::RIGHT);
    }
    else if(snake.GetMoveDir() == Direction::LEFT)
    {
        state.MOVE_LEFT = 1;
        state.MOVE_UP = 1;
        state.MOVE_DOWN = 1;

        state.DIE_RIGHT = 1;
        state.DIE_UP = snake.WillCollide(Direction::UP);
        state.DIE_DOWN = snake.WillCollide(Direction::DOWN);
        state.DIE_LEFT = snake.WillCollide(Direction::LEFT);
    }
    else if(snake.GetMoveDir() == Direction::RIGHT)
    {
        state.MOVE_RIGHT = 1;
        state.MOVE_UP = 1;
        state.MOVE_DOWN = 1;

        state.DIE_LEFT = 1;
        state.DIE_UP = snake.WillCollide(Direction::UP);
        state.DIE_DOWN = snake.WillCollide(Direction::DOWN);
        state.DIE_RIGHT = snake.WillCollide(Direction::RIGHT);
    }
    // Initalize state PELLET_XXX
    Vec2i vector_dist = pellet.GetPos() - snake.GetHeadPos();
    if(vector_dist.x > 8)
        state.PELLET_LEFT = 1;
    else
        state.PELLET_RIGHT = 1;
    if(vector_dist.y > 4)
        state.PELLET_UP = 1;
    else
        state.PELLET_DOWN = 1;
    
    return state;
}

Action Agent::GetAction() const
{
    const Snake& snake = game->GetSnake();
    Action action;

    if(snake.GetMoveDir() == Direction::UP)
        action.MOVE_UP = 1;
    if(snake.GetMoveDir() == Direction::DOWN)
        action.MOVE_DOWN = 1;
    if(snake.GetMoveDir() == Direction::LEFT)
        action.MOVE_LEFT = 1;
    if(snake.GetMoveDir() == Direction::RIGHT)
        action.MOVE_RIGHT = 1;
    
    return action;
}

float Agent::GetReward() const
{
    const Snake& snake = game->GetSnake();
    const Pellet& pellet = game->GetPellet();

    float reward = 0.0;

    if(snake.IsSelfCollision())
        reward += die_reward;
    else if(snake.GetHeadPos() == pellet.GetPos())
        reward += eat_reward;
    
    if(are_opposite(snake.GetMoveDir(), snake.GetLastMoveDir()))
        reward += opposite_move_reward;
    
    return reward;
}

Action Agent::GetBestAction(const State& state)
{
    Action action;
    bool found = false;

    if(IsBestDirForState(Direction::UP, state))
    {
        action.MOVE_UP = 1;
        found = true;
    }
    else if(IsBestDirForState(Direction::DOWN, state))
    {
        action.MOVE_DOWN = 1;
        found = true;
    }
    else if(IsBestDirForState(Direction::LEFT, state))
    {
        action.MOVE_LEFT = 1;
        found = true;
    }
    else if(IsBestDirForState(Direction::RIGHT, state))
    {
        action.MOVE_RIGHT = 1;
        found = true;
    }

    if(!found)
    {
        if(state.MOVE_UP == 1 && state.DIE_UP == 0)
        {
            action.MOVE_UP = 1;
            found = true;
        }
        else if(state.MOVE_DOWN == 1 && state.DIE_DOWN == 0)
        {
            action.MOVE_DOWN = 1;
            found = true;
        }
        else if(state.MOVE_LEFT == 1 && state.DIE_LEFT == 0)
        {
            action.MOVE_LEFT = 1;
            found = true;
        }
        else if(state.MOVE_RIGHT == 1 && state.DIE_RIGHT == 0)
        {
            action.MOVE_RIGHT = 1;
            found = true;
        }
    }

    assert(found && "None of action is the best??");

    return action;
}

bool Agent::IsBestDirForState(Direction dir, const State& state)
{
    if(dir == Direction::UP)
    {
        if(state.PELLET_UP == 1 && state.MOVE_UP == 1 && state.DIE_UP == 0)
        {
            return true;
        }
    }
    else if(dir == Direction::DOWN)
    {
        if(state.PELLET_DOWN == 1 && state.MOVE_DOWN == 1 && state.DIE_DOWN == 0)
        {
            return true;
        }
    }
    else if(dir == Direction::LEFT)
    {
        if(state.PELLET_LEFT == 1 && state.MOVE_LEFT == 1 && state.DIE_LEFT == 0)
        {
            return true;
        }
    }
    else if(dir == Direction::RIGHT)
    {
        if(state.PELLET_RIGHT == 1 && state.MOVE_RIGHT == 1 && state.DIE_RIGHT == 0)
        {
            return true;
        }
    }

    return false;
}
/*
Action Agent::GetBestAction(const State& state)
{
    Action action;

    const Snake& snake = game->GetSnake();
    const Pellet& pellet = game->GetPellet();

    Vec2i vector_dist = pellet.GetPos() - snake.GetHeadPos();

    // Absolute distances from snake to pellet in 4 directions
    int dist_down;
    int dist_up;
    int dist_left;
    int dist_right;
    if(vector_dist.x < 0)
    {
        dist_left = -vector_dist.x;
        dist_right = 16 - dist_left;
    }
    else
    {
        dist_right = vector_dist.x;
        dist_left = 16 - dist_right;
    }
    if(vector_dist.y < 0)
    {
        dist_up = -vector_dist.y;
        dist_down = 9 - dist_up;
    }
    else
    {
        dist_down = vector_dist.y;
        dist_up = 9 - dist_down;
    }

    Direction shortest_to_longest_dist[4];
    GetDirsInOrder(shortest_to_longest_dist, dist_up, dist_down, dist_left, dist_right);

    for(Direction dir : shortest_to_longest_dist)
    {
        if(UpdateActionIfGood(state, action, dir))
            return action;
    }

    return action;
}

bool Agent::UpdateActionIfGood(const State& state, Action& action, Direction dir)
{
    if(dir == Direction::UP)
    {
        if(state.MOVE_UP == 1 && state.DIE_UP == 0)
        {
            action.MOVE_UP = 1;
            return true;
        }
        return false;
    }
    if(dir == Direction::DOWN)
    {
        if(state.MOVE_DOWN == 1 && state.DIE_DOWN == 0)
        {
            action.MOVE_DOWN = 1;
            return true;
        }
    }
    if(dir == Direction::LEFT)
    {
        if(state.MOVE_LEFT == 1 && state.DIE_LEFT == 0)
        {
            action.MOVE_LEFT = 1;
            return true;
        }
    }
    if(dir == Direction::RIGHT)
    {
        if(state.MOVE_RIGHT == 1 && state.DIE_RIGHT == 0)
        {
            action.MOVE_RIGHT = 1;
            return true;
        }
    }

    // TODO: Think what to do here if dir is Direction::NONE
    return false;
}

void Agent::GetDirsInOrder(Direction dirs[4], int dist_up, int dist_down, int dist_left, int dist_right)
{
    if(dist_up <= dist_down && dist_up <= dist_left && dist_up <= dist_right) // UP is first
    {
        dirs[0] = Direction::UP;
        if(dist_down <= dist_left && dist_down <= dist_right) // DOWN is second
        {
            dirs[1] = Direction::DOWN;
            if(dist_left <= dist_right) // LEFT is third, RIGHT is fourth
            {
                dirs[2] = Direction::LEFT;
                dirs[3] = Direction::RIGHT;
            }
            else // RIGTH is third, LEFT is fourth
            {
                dirs[2] = Direction::RIGHT;
                dirs[3] = Direction::LEFT;
            }
        }
        else if(dist_left <= dist_right) // LEFT is second
        {
            dirs[1] = Direction::LEFT;
            if(dist_down <= dist_right) // DOWN is third, RIGHT is fourth
            {
                dirs[2] = Direction::DOWN;
                dirs[3] = Direction::RIGHT;
            }
            else // RIGTH is third, DOWN is fourth
            {
                dirs[2] = Direction::RIGHT;
                dirs[3] = Direction::DOWN;
            }
        }
        else // RIGHT is second
        {
            dirs[1] = Direction::RIGHT;
            if(dist_down <= dist_left) // DOWN is third, LEFT is fourth
            {
                dirs[2] = Direction::DOWN;
                dirs[3] = Direction::LEFT;
            }
            else // LEFT is third, DOWN is fourth
            {
                dirs[2] = Direction::LEFT;
                dirs[3] = Direction::DOWN;
            }
        }
    }
    else if(dist_down <= dist_left && dist_down <= dist_right) // DOWN is first
    {
        dirs[0] = Direction::DOWN;
        if(dist_up <= dist_left && dist_up <= dist_right) // UP is second
        {
            dirs[1] = Direction::UP;
            if(dist_left <= dist_right) // LEFT is third, RIGHT is fourth
            {
                dirs[2] = Direction::LEFT;
                dirs[3] = Direction::RIGHT;
            }
            else // RIGTH is third, LEFT is fourth
            {
                dirs[2] = Direction::RIGHT;
                dirs[3] = Direction::LEFT;
            }
        }
        else if(dist_left <= dist_right) // LEFT is second
        {
            dirs[1] = Direction::LEFT;
            if(dist_up <= dist_right) // UP is third, RIGHT is fourth
            {
                dirs[2] = Direction::UP;
                dirs[3] = Direction::RIGHT;
            }
            else // RIGTH is third, UP is fourth
            {
                dirs[2] = Direction::RIGHT;
                dirs[3] = Direction::UP;
            }
        }
        else // RIGHT is second
        {
            dirs[1] = Direction::RIGHT;
            if(dist_up <= dist_left) // UP is third, LEFT is fourth
            {
                dirs[2] = Direction::UP;
                dirs[3] = Direction::LEFT;
            }
            else // LEFT is third, UP is fourth
            {
                dirs[2] = Direction::LEFT;
                dirs[3] = Direction::UP;
            }
        }
    }
    else if(dist_left <= dist_right) // LEFT is first
    {
        dirs[0] = Direction::LEFT;
        if(dist_up <= dist_down && dist_up <= dist_right) // UP is second
        {
            dirs[1] = Direction::UP;
            if(dist_down <= dist_right) // DOWN is third, RIGHT is fourth
            {
                dirs[2] = Direction::DOWN;
                dirs[3] = Direction::RIGHT;
            }
            else // RIGTH is third, DOWN is fourth
            {
                dirs[2] = Direction::RIGHT;
                dirs[3] = Direction::DOWN;
            }
        }
        else if(dist_down <= dist_right) // DOWN is second
        {
            dirs[1] = Direction::DOWN;
            if(dist_up <= dist_right) // UP is third, RIGHT is fourth
            {
                dirs[2] = Direction::UP;
                dirs[3] = Direction::RIGHT;
            }
            else // RIGTH is third, UP is fourth
            {
                dirs[2] = Direction::RIGHT;
                dirs[3] = Direction::UP;
            }
        }
        else // RIGHT is second
        {
            dirs[1] = Direction::RIGHT;
            if(dist_up <= dist_down) // UP is third, DOWN is fourth
            {
                dirs[2] = Direction::UP;
                dirs[3] = Direction::DOWN;
            }
            else // DOWN is third, UP is fourth
            {
                dirs[2] = Direction::DOWN;
                dirs[3] = Direction::UP;
            }
        }
    }
    else // RIGHT is first
    {
        dirs[0] = Direction::RIGHT;
        if(dist_up <= dist_down && dist_up <= dist_left) // UP is second
        {
            dirs[1] = Direction::UP;
            if(dist_down <= dist_left) // DOWN is third, LEFT is fourth
            {
                dirs[2] = Direction::DOWN;
                dirs[3] = Direction::LEFT;
            }
            else // LEFT is third, DOWN is fourth
            {
                dirs[2] = Direction::LEFT;
                dirs[3] = Direction::DOWN;
            }
        }
        else if(dist_down <= dist_left) // DOWN is second
        {
            dirs[1] = Direction::DOWN;
            if(dist_up <= dist_left) // UP is third, LEFT is fourth
            {
                dirs[2] = Direction::UP;
                dirs[3] = Direction::LEFT;
            }
            else // LEFT is third, UP is fourth
            {
                dirs[2] = Direction::LEFT;
                dirs[3] = Direction::UP;
            }
        }
        else // LEFT is second
        {
            dirs[1] = Direction::LEFT;
            if(dist_up <= dist_down) // UP is third, DOWN is fourth
            {
                dirs[2] = Direction::UP;
                dirs[3] = Direction::DOWN;
            }
            else // DOWN is third, UP is fourth
            {
                dirs[2] = Direction::DOWN;
                dirs[3] = Direction::UP;
            }
        }
    }
}
*/