#pragma once

#include <DirectXMath.h>

#include "types.h"

const float origin_x = 0.0f;
const float origin_y = 0.0f;

enum class Direction { left , right , up , down };
enum class Rotation { clock_wise , counter_clock_wise };

const float layer_playfield = 0.0f;
const float layer_block = -1.0f;

//template< typename type = float>
//type minimum( const type & lhs , const type & rhs )
//{
//	// if lhs < rhs == true return lhs else return rhs
//	return lhs < rhs ? lhs : rhs;
//}

inline void rotation_z( DirectX::XMFLOAT3 & in_point , float in_radians )
{
	in_point.x = in_point.x * cos( in_radians ) - in_point.y * sin( in_radians );
	in_point.y = in_point.y * cos( in_radians ) + in_point.y * sin( in_radians );
}


//typedef struct Tetri_centre
class Tetri_centre
{
	public:

		Tetri_centre( const uint in_block , const uint in_vertex0 , const uint in_vertex1 )
			: block( in_block ) , vertex0( in_vertex0 ) , vertex1( in_vertex1 )
		{
			assert( in_block   >= 0 );
			assert( in_vertex0 >= 0 );
			assert( in_vertex1 >= 0 );
		}

		Tetri_centre() {};

		void initialise( const uint in_block , const uint in_vertex0 , const uint in_vertex1 )
		{
			block = in_block;
			vertex0 = in_vertex0;
			vertex1 = in_vertex1;
		}

		const uint get_block()   const { return block; }
		const uint get_vertex0() const { return vertex0; }
		const uint get_vertex1() const { return vertex1; }

	private:

		uint block   = 0u;
		uint vertex0 = 0u;
		uint vertex1 = 0u;

};// Tetri_centre;

//enum class Graphic_layers { playfield = 0 , block };

//typedef struct Graphic_layers
//{
//	const float playfield	= 0.0f;
//	const float block		= -1.0f;
//} Graphic_layers;

//class Graphic_layers
//{
//	public:
//	const float playfield = 0.0f;
//	const float block = -1.0f;
//};
//
//Graphic_layers graphic_layers;