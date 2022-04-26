#pragma once
#include "snake.h"
#include <vector>

Snake::Snake(int x, int y)  // set starting position
{
	body.push_back(COORD{ (SHORT)x, (SHORT)y });
	body.push_back(COORD{(SHORT)x, (SHORT)(y - 1) });

	oldbody = body;
	ptrBody = std::make_unique<std::vector<COORD>>(body);
	ptrOldBody = std::make_unique<std::vector<COORD>>(oldbody);;
}

Snake::~Snake()
{
}

void Snake::addPiece()
{
	ptrBody->erase(ptrBody->begin() + 1, ptrBody->end());  // preallocate memory
	ptrBody->reserve(ptrBody->size() + ptrBody->size());
	ptrBody->insert(ptrBody->end(), ptrOldBody->begin(), ptrOldBody->end());
}

void Snake::move()  // move to game?
{

	COORD* head = ptrBody->data();
	COORD temp_head = *head;

	*ptrOldBody = *ptrBody;
	switch (dir)
	{
	case left:
		temp_head.X -= 1;
		break;
	case up:
		temp_head.Y -= 1;
		break;
	case right:
		temp_head.X += 1;
		break;
	case down:
		temp_head.Y += 1;
		break;
	}
	// check for forbiden movement
	if ((temp_head.X == ptrBody->at(1).X) && (temp_head.Y == ptrBody->at(1).Y))
		return;
	else *head = temp_head;
	// move tail
	for (size_t i = 1; i < ptrBody->size(); i++)
	{
		ptrBody->at(i) = ptrOldBody->at(i-1);
	}
}

COORD Snake::getCOORD(int i)
{
	return body.at(i);
}

int Snake::getLength()
{
	return body.size();
}

