#pragma once

// The three screens the game can be in. Kept as a simple enum switched
// on inside Game rather than a full State-pattern class hierarchy --
// three flat screens don't carry enough per-state behavior to justify
// the extra indirection for this phase.
enum class GameState
{
    Menu,
    Playing,
    GameOver
};
