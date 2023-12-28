# C++ Elective Submission: Fighting game engine

This project was created as part of an elective course at H_DA. It features a framework resembling a component based engine, which was created during the lectures of the course by Valentin Klink. The task for the submission was to use this framework and extend it to create a game.

Solution is not included, but can build with cmake with the included cmakelist


Controls:
  Player 1:
    wasd -> directions
    space -> action A
    altL -> action B
  Player 1:
    arrowkeys -> directions
    strgR -> action A
    shiftR -> action B
  Other:
    num1 -> load new character for player 1 (enter filename via console eg. "Lyn")
    num2 -> load new character for player 2 (enter filename via console eg. "Moon")

Character files in json format are located in the asset folder


Originally my intention was to make a runtime character editor to create new fighter from scratch or adjust existing ones, but I had not enough time.
In the end I just implemented serialization for the character Data and a way to serialize them either from instanced Fighter object or by EntityId and the actual data in the different properties. 
The serialization from entityID is important for the character editor and maybe I will work on that in the future.

## Code Contributions by Mai Vollmer

- GameDevAndBeyond_Template
  - Code
    - GameDevAndBeyond
      - Game.cpp
      - Game.h
      - main.cpp
    - SFMLEngine
      - FightingEngine
        - AnimatorSystem.cpp
        - AnimatorSystem.h
        - CharacterEditor.cpp
        - CharacterEditor.h
        - Fighter.cpp
        - Fighter.h
        - InputSystem.cpp
        - InputSystem.h
        - LogicSystem.cpp
        - LogicSystem.h
        - Lyn.h
        - SailorMoon.h
        - TransformSystem.cpp
        - TransformSystem.h
