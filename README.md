<h1>GameEngine</h1>

Also includes Burgertime as a game made in the engine.

GameEngine started from the minigin framework: https://github.com/avadae/minigin

There are triggers, but no actual collisions/physics.

To make sure the user of the engine doesn't need to manage pointers to components/gameobjects that might be destroyed by another component, I made an ObservingPointer. This pointer can be used instead of a raw pointer to observe other components/gameobjects. if the observed object gets destroyed, the ObservingPointer gets set to nullptr, so the user can check for null instead of having to deal with potential undefined behavior.

I added an option to attach a cameracomponent to a gameobject. It's not active in burger time, but if you want, you can enable it by uncommenting the code in level.cpp on line 175.
It can't follow multiple gameobjects at the same time yet.
It's mostly there in case I'd make other games.
