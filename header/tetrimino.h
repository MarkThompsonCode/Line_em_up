﻿#pragma once

// std
#include <vector>  
#include <map>
#define _USE_MATH_DEFINES
#include <math.h>
// DirectX
#include <DirectXMath.h>
// DX11
#include "types.h"
#include "quad.h"
#include "game_actor.h"
// Line_em_up
#include "globals.h"
//#include "DX11_math.h"
#include "states.h"

class Tetrimino : public Game_actor
{
	public:

		Tetrimino() {} // ? = default

		void create( const vector< XMFLOAT2 >	block_positions_relative ,
					 Tetri_centre				in_centre ,
					 const std::wstring			texture_diffuse ,
					 Quad * const in_playfield );

		void update( double time_delta );
		void render();

		// virtual void execute_command( Command in_command ) override;

		virtual void try_move( Direction in_direction ) override;
		virtual void try_rotate( Rotation in_rotation ) override;

	private:

		void move( const Direction in_direction );
		//void rotate( const float in_angle );
		void rotate( const Rotation in_rotation );
		void add_blocks( wstring texture_diffuse );
		void set_position( const XMFLOAT3 in_position );
		bool within_playfield( const Direction in_direction );
		bool within_playfield( const Rotation in_rotation );
		//bool within_playfield( const vector<vertex_rgba_uv> & in_verticies );
		XMFLOAT3 get_centre();
		Bounding_box get_bounding_box();
		vector<vertex_rgba_uv> get_verticies();
		void rotation_z_about_point( vector<vertex_rgba_uv> & verticies , XMFLOAT3 point , float radians );
		Bounding_box get_bounding_box( const vector<vertex_rgba_uv> & verticies );

		//XMFLOAT2 get_block_size();

		//bool can_fit
		//bool block_obstructing(const playfield_blocks * in_blocks, const string in_direction){}

	private:

		std::vector< Quad >			blocks;
		Quad * playfield = nullptr;
		//XMFLOAT3					position{};
		//XMFLOAT3					position_initial{};
		Tetri_centre				centre;
		vector< XMFLOAT2 >			block_offsets;
		double						drop_velocity{};
		//Grid_size					grid_size {};
		enum class					states { next , falling , fast_drop , hard_drop , moving , rotating , sliding , locked , };  // row_made , delete_row_blocks

		State< Tetrimino > * state = nullptr;

		//std::map < wstring , DirectX::XMINT2 > translate;
		//rectangle					border{};

		std::map< Direction , XMFLOAT2 >	directions;
		std::map< Rotation , float >		rotations;
};

// 1 block.y = 25 pixels
// 25 pixels / 1 second = 25 pixels/sec

// speed = distance / time
// distance = speed * time

// tetri : public state

// block.update() 
//   block.y += 25 * time (

//Bounding_box get_bounding_box()
//{
//	Bounding_box box;
//	return box;
//}
//
//Bounding_box get_bounding_oriented_box()
//{
//	Bounding_box box = get_bounding_box();
//
//	box.min.x = blocks.front().get_bounding_box().min.x;
//	box.min.y = blocks.front().get_bounding_box().min.y;
//	box.min.z = blocks.front().get_bounding_box().min.z;
//
//	//for( auto & block : blocks )
//	//{
//
//	//	if( block.point.x < box.min.x ) box.min.x = vertex.point.x;
//	//	if( vertex.point.x > box.max.x ) box.max.x = vertex.point.x;
//
//	//	if( vertex.point.y < box.min.y ) box.min.y = vertex.point.y;
//	//	if( vertex.point.y > box.max.y ) box.max.y = vertex.point.y;
//
//	//	if( vertex.point.z < box.min.z ) box.min.z = vertex.point.z;
//	//	if( vertex.point.z > box.max.z ) box.max.z = vertex.point.z;
//	//}
//
//	return box;
//	// for each vertex in apply SRT matrix multiplication
//	// find min and max vertices
//
//	/*	rectangle tetri_border = blocks.front().get_border();
//
//	for( auto & block : blocks )
//	{
//	rectangle block_boarder = block.get_border();
//
//	if( block_boarder.left < tetri_border.left )
//	tetri_border.left = block_boarder.left;
//
//	if( block_boarder.right > tetri_border.right )
//	tetri_border.right = block_boarder.right;
//
//	if( block_boarder.top > tetri_border.top )
//	tetri_border.top = block_boarder.top;
//
//	if( block_boarder.bottom < tetri_border.bottom )
//	tetri_border.bottom = block_boarder.bottom;
//	}
//
//	return tetri_border;*/
//	//rectangle temp_boarder{};
//
//	//return temp_boarder;
//}