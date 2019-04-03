#include "State.h"
#include "Tetrimino.h"

State< Tetrimino > * Next::update( Tetrimino & actor , double time_delta )
{
	return nullptr;
}

State< Tetrimino > * Falling::update( Tetrimino & actor , double time_delta )
{

	//time += time_delta;
	
	//if(time <= 1)


	//if( time >= fall_delay )
	//{
		actor.try_move( Direction::down );
	//	time = 0.0f;
	//}

	//if( hit_bottom) 
	// return new Sliding()
	return nullptr;
}

State< Tetrimino > * Moving::update( Tetrimino & actor , double time_delta )
{
	//if(moved += right

	//actor.try_move( direction );

	return nullptr;
	// return new Falling();
}

State< Tetrimino > * Rotating::update( Tetrimino & actor , double time_delta )
{
	//if( angle < m_pi_2 ) angle += 0.1;
	return nullptr;
}
	





