#pragma once

#include "input.h"
#include "cakez_lib.h"
#include "renderer_interface.h"


// #############################################################################
//                           Game Structs 
// #############################################################################
struct GameState
{
    bool initialized = false;
    IVec2 playerPos;
};

// #############################################################################
//                           Game Globals 
// #############################################################################
static GameState* gameState;
// #############################################################################
//                           Game Functions (Exposed) 
// #############################################################################
extern "C"
{
    EXPORT_FN void update_game(GameState* gameStateIn, RenderData* renderDataIn, Input* inputIn);
}