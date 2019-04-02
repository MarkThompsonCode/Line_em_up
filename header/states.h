#pragma once

#include "state.h"

class Tetrimino;

class Falling : public State< Tetrimino >
{
	public:

		virtual State * update( Tetrimino & actor , double time_delta ) override;

	private:

		double time = 0.0;
		const double fall_delay = 1.0;
};

class Next : public State< Tetrimino >
{
	public:
		virtual State * update( Tetrimino & actor , double time_delta ) override;
};

class Moving : public State< Tetrimino >
{	
	public:

		Moving( const Direction in_direction )
			: direction( in_direction )
		{}

		virtual State * update( Tetrimino & actor , double time_delta ) override;

	private:
		Direction direction
};
