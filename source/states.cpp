#include "State.h"

#include "Tetrimino.h"

State< Tetrimino > * Next::update( Tetrimino & actor , double time_delta )
{
	return nullptr;
}

State< Tetrimino > * Falling::update( Tetrimino & actor , double time_delta )
{
	time += time_delta;

	if( time >= fall_delay )
	{
		actor.try_move( Direction::down );
		time = 0.0f;
	}

	//if( hit_bottom) 
	// return new Sliding
	return nullptr;
}

//State< Tetrimino > * Moving::Moving( Tetrimino & actor , double time_delta ) {}

State< Tetrimino > * Moving::update( Tetrimino & actor , double time_delta )
{

	return nullptr;
}

