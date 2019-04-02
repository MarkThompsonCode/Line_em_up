#pragma once

#include "command.h"
#include "game_actor.h"
//#include "tetrimino.h"
#include "globals.h"

class Rotate : public Command
{
	public:

		Rotate( Rotation in_rotation ) 
			: rotation_direction( in_rotation )
		{}

		virtual void execute( Game_actor & in_actor ) override
		{
			in_actor.try_rotate( rotation_direction );
		}

	private:

		Rotation rotation_direction;
};

class Move : public Command
{
	public:
		Move( Direction in_direction )
			: direction( in_direction )
		{}

		virtual void execute( Game_actor & in_actor ) override
		{
			in_actor.try_move( direction );
		}
	private:
		Direction direction;
};

class Hard_drop : public Command
{
	public:

		virtual void execute( Game_actor & in_actor ) override
		{
			//cout << "\nDrop";
		}
};
