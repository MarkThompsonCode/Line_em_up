#pragma once

#include "state.h"
class Tetrimino;

class Next : public State< Tetrimino >
{
	public:
	virtual State * update( Tetrimino & actor , double time_delta ) override;
};

class Falling : public State< Tetrimino >
{
	public:

		virtual State * update( Tetrimino & actor , double time_delta ) override;

	private:

		double time = 0.0;
		double fall_rate = 1.0;
};

class Moving : public State< Tetrimino >
{
	public:

		Moving( const Direction in_direction )
			: direction( in_direction )
		{}

		virtual State * update( Tetrimino & actor , double time_delta ) override;

	private:
		Direction direction;
};

class Rotating : public State< Tetrimino >
{
	public:

		Rotating( const Rotation in_rotation )
			: rotation( in_rotation )
		{}

		virtual State * update( Tetrimino & actor , double time_delta ) override;

	private:

		Rotation rotation;
		float angle;
		double time = 0.0;
};