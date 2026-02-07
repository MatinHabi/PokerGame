#pragma once

enum ActionType {Nothing, Fold, Raise, Call};

struct Action{
    ActionType action;
    int amount; //used if there is a raise
};