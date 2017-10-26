#include <iostream>
#include "WavReader.hpp"

int main()
{
    WavReader wav("../media/molly.wav");
    wav.save("../media/molly_out.wav");
}