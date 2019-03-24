#pragma once

#include <map>

template< typename type >
class Command
{
	public:

		virtual void execute( type & actor ) = 0;
};

/*
class Rotate : public Command< Tetrimino >
{
	public:

	Rotate( const Rotation in_rotation ) : rotation_direction( in_rotation )
	{}

	virtual void execute( Tetrimino & in_tetrimino ) override
	{
		in_tetrimino.rotate( rotation_direction );
	}

	private:

	Rotation rotation_direction;
};

class Hard_drop : public Command< Tetrimino >
{
	public:

	virtual void execute( Tetrimino & in_tetrimino ) override
	{
		//cout << "\nDrop";
	}
};
*/