#include "game.h"
#include "assets.h"
#include "cakez_lib.h"

// #############################################################################
//                           Game Constants
// #############################################################################
constexpr int WORLD_WIDTH = 320;
constexpr int WORLD_HEIGHT = 180;
constexpr int TILE_SIZE = 8;

// #############################################################################
//                           Game Structs
// #############################################################################

// #############################################################################
//                           Game Functions 
// #############################################################################
EXPORT_FN void update_game(GameState* gameStateIn, RenderData* renderDataIn, Input* inputIn)
{
    if(renderData != renderDataIn)
    {
        gameState = gameStateIn;
        renderData = renderDataIn;
        input = inputIn;
    }
    if(!gameState->initialized)
    {
        renderData->gameCamera.dimensions = {WORLD_WIDTH, WORLD_HEIGHT};
        gameState->initialized = true;
       
    }
    draw_sprite(SPRITE_DICE, {0.0f, 0.0f}); 
}