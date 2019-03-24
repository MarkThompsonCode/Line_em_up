#pragma once

// std
#include <map>
// DX11
#include "DX11.h"
#include "Camera.h"
#include "Quad.h"
// Line_em_up
#include "Tetrimino.h"

//#define _USE_MATH_DEFINES
// namespace Line_em_up {}

class Line_em_up : public DX11
{
	private:

		//Input_handler keyboard_handler;

		Camera					camera;
		Quad					playfield;

		double					drop_velocity{ -1.0 }; // playfield square per second

		vector< Tetrimino >		tetrimino_types;
		//list< Tetrimino >		tetrimino_in_play;

		Tetrimino				tetrimino_J;
		//	   [3]
		//	   +2]
		//	[0][1]

		//vector< Tetrimino >::const_iterator active_tetri;  // using vec<>::itr active_tetri // active_tetri = blocks.front();
		vector< XMFLOAT2 >		block_offsets_J { XMFLOAT2( -0.5f , -1.0f ) , XMFLOAT2( 0.5f , -1.0f ) , XMFLOAT2( 0.5f , 0.0f ) , XMFLOAT2( 0.5f , 1.0f ) };
		vector< XMFLOAT2 >		block_offsets_T  { XMFLOAT2( f , f ) , XMFLOAT2( f , f ) , XMFLOAT2( f , f ) , XMFLOAT2( f , f ) };

		Timer					timer;

		// stage ; speed ( difficulty )
		const std::map< uint , float >		level_fall_rates{ { 1u , 1.0f } };	// 1 block per second

	public:

	Line_em_up( HINSTANCE h_instance , UINT window_width , UINT window_height );
	//	: DX11( h_instance , window_width , window_height );

		//Input_handler::update

		void reset();
		void update( const long double time_delta );
		void render();

};