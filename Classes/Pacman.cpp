#include "Pacman.h"

Pacman* Pacman::pacmanInstance = 0;
PacmanDestroyer Pacman::destroyer;

PacmanDestroyer::~PacmanDestroyer() {
    delete pacmanInstance;
}
void PacmanDestroyer::initialize( Pacman* p ) {
    pacmanInstance = p;
}
Pacman *Pacman::getInstance() {
    if(!pacmanInstance)     {
        pacmanInstance = new Pacman();
        destroyer.initialize( pacmanInstance);
    }
    return pacmanInstance;
}
