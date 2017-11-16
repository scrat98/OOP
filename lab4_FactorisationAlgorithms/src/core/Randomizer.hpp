#pragma once

#include <ctime>

class Randomizer {
public:
    template<class T>
    static T getRand() {
        static bool init = false;
        if (!init) {
            init = true;
            srand(time(NULL));
        }

        return rand();
    }

private:
    Randomizer();
};
