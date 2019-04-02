#pragma once

// std
#include <map>
// DX11
#include "game.h"
#include "camera.h"
#include "quad.h"
#include "MS_Keyboard.h"
#include "input_handler.h"
// Line_em_up
#include "globals.h"
#include "tetrimino.h"
#include "commands.h"

// namespace Line_em_up {}

class Line_em_up : public Game
{
	public:

		Line_em_up( const HWND in_window , const unsigned int in_resolution_width , const unsigned int window_height );

		void			reset();
		virtual void	update() override;
		virtual void	render() override;
		virtual void	input( const int in_key ) override;

	private:

		//DirectX::Keyboard		keyboard;

		Camera					camera;
		Quad					playfield;

		vector< Tetrimino >		tetrimino_types;
		//list< Tetrimino >		tetrimino_in_play;

		Tetrimino				tetrimino_J;
		//Tetri_centre			centre_J( 2 , 0 , 1 );
		//	   [3]
		//	   +2]
		//	[0][1]

		//vector< Tetrimino >::iterator active_tetri;  // using vec<>::itr active_tetri // active_tetri = blocks.front();

		vector< XMFLOAT2 >		block_offsets_J { XMFLOAT2( -0.5f , -1.0f ) , XMFLOAT2( 0.5f , -1.0f ) , XMFLOAT2( 0.5f , 0.0f ) , XMFLOAT2( 0.5f ,  1.0f ) };
		vector< XMFLOAT2 >		block_offsets_T { XMFLOAT2( -1.5f ,  0.5f ) , XMFLOAT2( 0.0f ,  0.5f ) , XMFLOAT2( 1.5f , 0.5f ) , XMFLOAT2( 0.0f , -0.5f ) };

		Timer					timer;

		// stage = speed ( difficulty )
		const std::map< uint , float >		level_fall_rates{ { 1u , 1.0f } };	// 1 block per second
		double								drop_velocity{ -1.0 }; // playfield square per second


};