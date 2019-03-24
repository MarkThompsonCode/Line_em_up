#include "Tetrimino.h"

using std::make_pair;

void Tetrimino::create( const vector< XMFLOAT2 >	block_positions_relative ,
			 Tetri_centre				in_centre ,
			 const wstring				texture_diffuse ,
			 const Playfield & playfield )  // const * playfield
{
	centre = in_centre;
	block_offsets = block_positions_relative;
	playfield = in_playfield;

	directions.insert( make_pair( Direction::left , XMFLOAT2( -1.0f , 0.0f ) ) );
	directions.insert( make_pair( Direction::right , XMFLOAT2( 1.0f , 0.0f ) ) );
	directions.insert( make_pair( Direction::down , XMFLOAT2( 0.0f , -1.0f ) ) );

	rotations.insert( make_pair( Rotation::clock_wise , 1.570796326794f ) );// M_PI_2 ) );
	rotations.insert( make_pair( Rotation::counter_clock_wise , -1.570796326794f ) );// M_PI_2 ) );

	add_blocks( texture_diffuse );

	//set_position();

	state = new Falling();
}

void Tetrimino::add_blocks( wstring texture_diffuse )
{
	for( auto & offset : block_offsets )
	{
		Quad new_block( texture_diffuse );

		XMFLOAT2 position_initial;
		// 0 (screen center) - 1/2 block width
		position_initial.x = 0.0f;// -0.5 * new_block.get_width();
		position_initial.y = 0.0f;//playfield.top - ( 0.5f * new_block.get_height() );

								  // each block + offset
		float x = position_initial.x + ( offset.x * new_block.get_width() );// -½width );
		float y = position_initial.y + ( offset.y * new_block.get_height() );// -½height );

		new_block.set_position( XMFLOAT3( x , y , layer_block ) );

		blocks.push_back( new_block );
	}
}

XMFLOAT3 Tetrimino::get_centre()
{
	// centre = ( vertex0 + vertex1 ) / 2

	const Quad & block = blocks.at( centre.block );

	vector<vertex_rgba_uv> world_vertices = block.get_world_vertices();

	XMFLOAT3 point;

	point.x = ( world_vertices.at( centre.vertex0 ).point.x + world_vertices.at( centre.vertex1 ).point.x ) / 2.0f;
	point.y = ( world_vertices.at( centre.vertex0 ).point.y + world_vertices.at( centre.vertex1 ).point.y ) / 2.0f;
	point.z = layer_block;

	return point;

	//x = cx + r * cos( a )
	//y = cy + r * sin( a )
	//float θz = atan2( r21 , r11 );
}

void Tetrimino::update( double time_delta )
{
	// state returns nullptr on exit
	State * new_state = state->update( *this , time_delta );

	// transistioning to a new state
	if( new_state != nullptr )
	{
		delete state;
		state = new_state;
	}

	// if ( position => m_playfield->boarder().bottom ) m_current_state = state::locked    // && slide_time_elapsed <= 0.0f;

	//for( auto & block : blocks ) { block.update( time_delta ); }


	float fall_rate = 25 * time_delta;

	//move( Direction::down )
	//   move( 0, -y = fall_rate * time_delta ( = 10 pixels per second = ? locked at 60fps )
}

void Tetrimino::render()
{
	for( auto & block : blocks ) { block.render(); }
}

void Tetrimino::move( const Direction in_direction )
{
	if( within_playfield( in_direction ) ) //&& ! block_obstructing )
	{
		for( auto & block : blocks )
		{
			block.translate_x( directions.at( in_direction ).x );// *block.get_width() );
			block.translate_y( directions.at( in_direction ).y );// *block.get_height() ); // * delta_time * fall_rate
		}
	}
}

bool Tetrimino::within_playfield( const Direction in_direction )
{
	XMFLOAT2 test_move( 0 , 0 );
	rectangle border; // playfield->get_boarder();

	test_move.x = directions.at( in_direction ).x;// * blocks.front().get_width();
	test_move.y = directions.at( in_direction ).y;// * blocks.front().get_height();

	for( const auto & block : blocks )
	{
		vector<vertex_rgba_uv> verticies = block.get_world_vertices();

		for( const auto & vertex : verticies )
		{
			if( ( vertex.point.x + test_move.x ) < playfield.left ||
				( vertex.point.x + test_move.x ) > playfield.right ||
				( vertex.point.y + test_move.y ) < playfield.bottom )
			{
				return false; // block would be outside playfield bounds

				// push back
			}
		}
	}

	return true;
}

bool Tetrimino::can_rotate( const Rotation in_rotation )
{
	// test quad

	// for each block 
	// for 
	return true;
}

void Tetrimino::rotate( Rotation in_rotation )
{
	if( can_rotate( in_rotation ) )
	{
		XMFLOAT3 centre = get_centre();

		for( auto & block : blocks )
		{
			block.rotate_point_z( centre , 0.2f );// rotations.at( in_rotation ) );
		}
	}

	// debug_out( "\ncentre.x = %.2f , y = %.2f" , centre.x , centre.y );
	// L"centre.x = " += to_wstring( centre.x )
}