#pragma once

enum ActionType {Nothing, Raise, Call, Fold};

struct Action{
    ActionType action;
    int amount; //used if there is a raise
};