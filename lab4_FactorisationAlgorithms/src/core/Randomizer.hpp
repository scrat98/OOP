#pragma once

#include <ctime>

class Randomizer {
public:
    Randomizer() = delete;

    template<class T>
    static T getRand() {
        static bool init = false;
        if (!init) {
            init = true;
            srand((unsigned int)time(NULL));
        }

        return rand();
    }
};
