#include "State.h"

#include "Tetrimino.h"

State * Next::update( Tetrimino & actor , double time_delta )
{
	return nullptr;
}

State * Falling::update( Tetrimino & actor , double time_delta )
{
	actor.move( Direction::down );

	//if( hit_bottom) 
	// return Sliding
	return nullptr;
}
