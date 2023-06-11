extra engine features:


logger
render priority



To make sure the user of the engine doesn't need to manage pointers to components/gameobjects that might be destroyed by another component, I made an ObservingPointer. This pointer can be used instead of a raw pointer to observe other components/gameobjects. if the observed object gets destroyed, the ObservingPointer gets set to nullptr, so the user can check for null instead of having to deal with potential undefined behavior.