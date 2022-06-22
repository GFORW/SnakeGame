#pragma once
#include "snake.h"

Snake::Snake(const int& x, const int& y)  // set starting position
{
	ptrBody = std::make_unique<std::vector<COORD>>(std::vector<COORD>{ COORD{(short)x, (short)y}});
	ptrOldBody = std::make_unique<std::vector<COORD>>(*ptrBody);
}

Snake::~Snake()
{
}

void Snake::addPiece() 
{
	ptrBody->resize(ptrBody->size() + 1);
	ptrBody->erase(ptrBody->begin() + 1, ptrBody->end());
	ptrBody->insert(ptrBody->begin()+1, ptrOldBody->begin(), ptrOldBody->end());
}