#pragma once

#include "intermediary.hpp"

#include <chrono>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <thread>
#include <unistd.h>
#include <vector>
#include <vlc/vlc.h>

// extern std::string MicrophoneOutput;
// extern std::string PlaybackOutput;


extern bool Done;

void prepPlayers();

std::vector<std::string> getOutputDevices();

void playSound(const char* AudioClipPath);
