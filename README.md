## Friendly fire/FFA for Deadlock

### Install:

1. Create the folder `addons/foo/bin/win64` (it doesn't have to be called foo but use the same name elsewhere)
2. Put server.dll in `addons/foo/bin/win64/server.dll`
3. Edit gameinfo.gi by adding `Game citadel/addons/foo` at the top of the `SearchPaths` block

For example:

```
SearchPaths
{
    Game 				citadel/addons/meow
    Game				citadel
    Game				core
}
```

4. Copy or symlink `citadel/cfg` into `addons/foo/cfg`
5. Launch game and run `owo_friendlyfire 1` in game

### Known problems

- Melee friendly fire does not work (yet?)
- Abilities and items that require a target to select dont work (yet?)
- Several abilities do not apply damage or are broken in other ways
  - Abilities which will not hurt teammates:
    - Bebop's abilities
    - Lash's Flog
    - McGinnis' Mini Turret
    - McGinnis' Spectral Wall
    - Paradox's Time Wall
    - Shiv's Slice and Dice
  - Other issues:
    - Abrams' Shoulder Charge is very busted
    - Dynamo's Rejuvenating Aura still heals teammates
    - McGinnis' Medicinal Specter still heals teammates
    - Mo and Krill's Disarm will disarm them
    - Pocket's Affliction will afflict him but will not do damage to him
