#pragma once
#include <limits>
#include <iostream>
#include <vector>

class IMenuStrategy;

void waitForEnter();

void runMenuFromStrategies(std::vector<IMenuStrategy*> strategies);