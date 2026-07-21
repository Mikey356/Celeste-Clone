#include "game.h"
#include "assets.h"
#include "cakez_lib.h"

// #############################################################################
//                           Game Constants
// #############################################################################

// #############################################################################
//                           Game Structs
// #############################################################################

// #############################################################################
//                           Game Functions 
// #############################################################################
bool just_pressed(GameInputType type)
{
    KeyMapping mapping = gameState->keyMappings[type];
    for(int idx = 0; idx < mapping.keys.count; idx++)
    {
        if(input->keys[mapping.keys[idx]].justPressed)
        {
            return true;
        }
    }

    return false;
}

bool is_down(GameInputType type)
{
    KeyMapping mapping = gameState->keyMappings[type];
    for(int idx = 0; idx < mapping.keys.count; idx++)
    {
        if(input->keys[mapping.keys[idx]].isDown)
        {
            return true;
        }
    }

    return false;
}

Tile* get_tile(int x, int y)
{
    Tile* tile = nullptr;

    if(x >= 0 && x < WORLD_GRID.x && y >= 0 && y < WORLD_GRID.y){
        tile = &gameState->worldGrid[x][y];
    }

    return tile;
}

Tile* get_tile(IVec2 worldPos)
{
    int x = worldPos.x / TILE_SIZE;
    int y = worldPos.y / TILE_SIZE;
    
    return get_tile(x, y);
}

void simulate()
{

    // Update Player
    {
      gameState->player.prevPos = gameState->player.pos;
      if(is_down(MOVE_LEFT))
      {
          gameState->player.pos.x -= 1;
      }

      if(is_down(MOVE_RIGHT))
      {
          gameState->player.pos.x += 1;
      }


      if(is_down(MOVE_DOWN))
      {
          gameState->player.pos.y += 1;
      }

      if(is_down(MOVE_UP))
      {
          gameState->player.pos.y -= 1;
      }
}

bool updateTiles = false;
if(is_down(MOUSE_LEFT))
{
    IVec2 worldPos = screen_to_world(input->mousePos);
    IVec2 mouseWorldPos = input->mousePosWorld; 
    Tile* tile = get_tile(worldPos);
    if(tile)
    {
        tile->isVisible = true;
        updateTiles = true;
    }
}

if(is_down(MOUSE_RIGHT))
{
    IVec2 worldPos = screen_to_world(input->mousePos);
    IVec2 mousePosWorld = input->mousePosWorld; 
    Tile* tile = get_tile(worldPos);
    if(tile)
    {
        tile->isVisible = false;
        updateTiles = true;
    }
}

if(updateTiles)
{      
   // Neighboring Tiles
   int neighborOffsets[24] = {0, -1,    -1, 0,    1, 0,     0, 1,     -1,-1,   1, -1,   -1,1, 1,1,   0,-2,   -2,0, 
    2,0,   0,2
 };
 // Topleft      = BIT(4) = 16
 // Topright     = BIT(5) = 32 
 // Bottomleft   = BIT(6) = 64
 // Bottom right = BIT(7) = 128
    for(int y = 0; y < WORLD_GRID.y; y++)
    {
        for(int x = 0; x < WORLD_GRID.x; x++)
        {
            Tile* tile = get_tile(x, y);
            if(!tile->isVisible) {
                continue;
            }
            tile->neighbourMask = 0;
            int neighborCount = 0;
            int extendedNeighborCount = 0;
            int emptyNeighborSlot = 0;
            // Look for surrounding 12 Neighbors
            for(int n = 0; n < 12; n++)
            {
                Tile* neighbor = get_tile(x + neighborOffsets[n * 2], y + neighborOffsets[n*2 + 1]);
                // No neighbor means the edge of the world
                if(!neighbor || neighbor->isVisible)
                {
                    tile->neighbourMask |= BIT(n);
                    if(n < 8) // Counting direct neighbors
                    {
                        neighborCount++;
                    }
                    else // Counting neighbors 1 tile away
                    {
                       extendedNeighborCount++;
                    }
                }
                else if(n<8) {
                    emptyNeighborSlot = n;
                }
            
                if(neighborCount == 7 && emptyNeighborSlot >= 4) /*We have a corner*/{
                    tile->neighbourMask = 16 + (emptyNeighborSlot - 4);
                }
                else if(neighborCount == 8 && extendedNeighborCount == 4) {
                    tile->neighbourMask = 20;
                }
                else
                {
                    tile->neighbourMask = tile->neighbourMask & 0b1111;
                }
            }
        }
    }
}

// #############################################################################
//                           Game Functions(exposed) 
// #############################################################################
EXPORT_FN void update_game(GameState* gameStateIn, RenderData* renderDataIn, Input* inputIn, float dt)
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
            
        // Tileset
        {
            IVec2 tilePosition = {48, 0};

            for(int y = 0; y < 5; y++)
            {
                for(int x = 0; x < 4; x++)
                {
                    gameState->tileCoords.add({tilePosition.x + x * 8, tilePosition.y + y * 8});
                }
            }

            // Black inside
            gameState->tileCoords.add({tilePosition.x, tilePosition.y + 5 * 8});
        }
        
        // Key Mappings
        {
           gameState->keyMappings[MOVE_UP].keys.add(KEY_W);
           gameState->keyMappings[MOVE_UP].keys.add(KEY_UP);
           gameState->keyMappings[MOVE_DOWN].keys.add(KEY_S);
           gameState->keyMappings[MOVE_DOWN].keys.add(KEY_DOWN);
           gameState->keyMappings[MOVE_LEFT].keys.add(KEY_A); 
           gameState->keyMappings[MOVE_LEFT].keys.add(KEY_LEFT);
           gameState->keyMappings[MOVE_RIGHT].keys.add(KEY_D);
           gameState->keyMappings[MOVE_RIGHT].keys.add(KEY_RIGHT);
           gameState->keyMappings[MOUSE_LEFT].keys.add(KEY_MOUSE_LEFT);
           gameState->keyMappings[MOUSE_RIGHT].keys.add(KEY_MOUSE_RIGHT);
        }
       
        renderData->gameCamera.position.x = 160;
        renderData->gameCamera.position.y = -90;
    }     



// Fixed Update Loop
{
    gameState->updateTimer += dt;
    while(gameState->updateTimer >= UPDATE_DELAY)
    {
        gameState->updateTimer -= UPDATE_DELAY;
        simulate();

        // Relative Mouse here, because more frames than simulation
        input->relMouse = input->mousePos - input->prevMousePos;
        input->prevMousePos = input->mousePos;

        // Clear the transition count for every key
        {
            for(int keyCode = 0; keyCode < KEY_COUNT; keyCode++)
            {
                input->keys[keyCode].justReleased = false;
                input->keys[keyCode].justPressed = false;
                input->keys[keyCode].halfTransitionCount = 0;
            }
        }
    }
}



    float interpolatedDT = (float)(gameState->updateTimer / UPDATE_DELAY);
   
    // Draw Player
    {
      Player& player = gameState->player;
      IVec2 playerPos = lerp(player.prevPos, player.pos, interpolatedDT);
      draw_sprite(SPRITE_DICE, playerPos); 
    }


    // Drawing Tileset
    {

        for(int y = 0; y < WORLD_GRID.y; y++)
        {
            for(int x = 0; x < WORLD_GRID.x; x++)
            {
                Tile* tile = get_tile(x, y);

                if(!tile->isVisible) {
                    continue;
                }
               
                // Draw Tile
                Transform transform = {};
                // Draw the tile around the center
                transform.pos = {x * (float)TILE_SIZE, y * (float)TILE_SIZE};
                transform.size = {8, 8};
                transform.spriteSize = {8, 8};
                transform.atlasOffset = gameState->tileCoords[tile->neighbourMask];
                draw_quad(transform);
            }
        }
    }
 
}