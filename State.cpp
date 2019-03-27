#include "State.h"

#include "Tetrimino.h"

State * Next::update( Tetrimino & actor , double time_delta )
{
	return nullptr;
}

State * Falling::update( Tetrimino & actor , double time_delta )
{
	time += time_delta;

	if( time >= fall_delay )
	{
		//actor.try_move( Direction::down );
		time = 0.0f;
	}

	//if( hit_bottom) 
	// return Sliding
	return nullptr;
}
