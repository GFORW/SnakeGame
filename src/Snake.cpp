#pragma once
#include "snake.h"
#include <vector>

Snake::Snake(int x, int y)  // set starting position
{
	ptrBody = std::make_unique<std::vector<COORD>>(std::vector<COORD>{COORD{ (SHORT)x, (SHORT)y }, COORD{ (SHORT)x, (SHORT)(y -1) }});
	ptrOldBody = std::make_unique<std::vector<COORD>>(*ptrBody);
}

Snake::~Snake()
{
}

void Snake::addPiece()
{
	ptrBody->erase(ptrBody->begin() + 1, ptrBody->end()); 
	ptrBody->reserve(ptrBody->size() + ptrBody->size()); // preallocate memory
	ptrBody->insert(ptrBody->end(), ptrOldBody->begin(), ptrOldBody->end());
}