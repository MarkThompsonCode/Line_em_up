#include "Tetrimino.h"

using std::make_pair;

void Tetrimino::create( const vector< XMFLOAT2 >	block_positions_relative ,
						Tetri_centre				in_centre ,
						const wstring				texture_diffuse ,
						Quad * const in_playfield ) 
{
	centre			= in_centre;
	block_offsets	= block_positions_relative;
	playfield		= in_playfield;

	directions.insert( make_pair( Direction::left  , XMFLOAT2( -1.0f ,  0.0f ) ) );
	directions.insert( make_pair( Direction::right , XMFLOAT2(  1.0f ,  0.0f ) ) );
	//directions.insert( make_pair( Direction::up  , XMFLOAT2(  0.0f ,  1.0f ) ) );
	directions.insert( make_pair( Direction::down  , XMFLOAT2(  0.0f , -1.0f ) ) );

	rotations.insert( make_pair( Rotation::clock_wise ,			static_cast< float >( M_PI_4 ) ) );
	rotations.insert( make_pair( Rotation::counter_clock_wise , static_cast< float >( -M_PI_4 ) ) );

	add_blocks( texture_diffuse );

	//playfield_boarder( -125 , -250 , 125 , 250 ); 250 - 37.5 = 212.5
	// tetri_j = -25, -37.5 , 25 , 37.5
	set_position( XMFLOAT3( 0 , 0 , ::layer_block ) );

	state = new Falling();
}

void Tetrimino::add_blocks( wstring texture_diffuse )
{
	for( const auto & offset : block_offsets )
	{
		blocks.push_back( Quad( texture_diffuse ) );
	}
}

XMFLOAT3 Tetrimino::get_centre()
{
	// centre = ( vertex0 + vertex1 ) / 2

	const Quad & block = blocks.at( centre.get_block() );

	vector<vertex_rgba_uv> world_vertices = block.get_world_vertices();

	XMFLOAT3 point;

	point.x = ( world_vertices.at( centre.get_vertex0() ).point.x + world_vertices.at( centre.get_vertex1() ).point.x ) / 2.0f;
	point.y = ( world_vertices.at( centre.get_vertex0() ).point.y + world_vertices.at( centre.get_vertex1() ).point.y ) / 2.0f;
	point.z = ::layer_block;

	return point;

	//x = cx + r * cos( a )
	//y = cy + r * sin( a )
	//float θz = atan2( r21 , r11 );
}

void Tetrimino::update( double time_delta )
{
	// state returns nullptr on exit
	State< Tetrimino > * new_state = state->update( *this , time_delta );

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

void Tetrimino::set_position( const XMFLOAT3 in_position )
{
	for( int index = 0; index < blocks.size(); ++index )
	{
		XMFLOAT3 position( 0 , 0 , 0 );
		XMFLOAT2 size		= blocks.at( index ).get_size();
		XMFLOAT2 offset		= block_offsets.at( index );

		position = in_position + ( offset * size );

		blocks.at( index ).set_position( position );
	}
}

void Tetrimino::try_move( Direction in_direction ) // bool push_away
{
	//if within_playfield && ! block_obstructing 

	if( within_playfield( in_direction ) )
	{
		move( in_direction );
	} 
	// else push away ( left/right playfield )
}

void Tetrimino::move( Direction in_direction ) // bool push_away
{
	for( auto & block : blocks )
	{
		block.translate_x( directions.at( in_direction ).x );// *block.get_width() );
		block.translate_y( directions.at( in_direction ).y );// *block.get_height() ); // * delta_time * fall_rate

		//bounding_box.translate( XMFLOAT3 in_translation );
	}
}

vector<vertex_rgba_uv> Tetrimino::get_verticies()
{
	vector<vertex_rgba_uv> verticies_combined;
	
	for( const auto & block : blocks )
	{
		vector<vertex_rgba_uv> verticies = block.get_world_vertices();
		verticies_combined.insert( verticies_combined.end() , verticies.begin() , verticies.end() );
	}

	return verticies_combined;
}

Bounding_box Tetrimino::get_bounding_box()
{
	Bounding_box tetri_box;

	tetri_box.min = XMFLOAT3(  FLT_MAX ,  FLT_MAX ,  FLT_MAX );
	tetri_box.max = XMFLOAT3( -FLT_MAX , -FLT_MAX , -FLT_MAX );

	Bounding_box block_box;

	for( auto & block : blocks )
	{
		block_box = block.get_bounding_box();
		
		tetri_box.min = minimum( block_box.min , tetri_box.min );	// always returns a value
		tetri_box.max = maximum( block_box.max , tetri_box.max );

		/*
		if( block_box.min.x < tetri_box.min.x ) tetri_box.min.x = block_box.min.x; // skips an assignment if not true
		*/
	}

	return tetri_box;
}

Bounding_box Tetrimino::get_bounding_box( const std::vector<vertex_rgba_uv> & verticies )
{
	Bounding_box tetri_box;

	tetri_box.min = XMFLOAT3(  FLT_MAX ,  FLT_MAX ,  FLT_MAX );
	tetri_box.max = XMFLOAT3( -FLT_MAX , -FLT_MAX , -FLT_MAX );

	for( const auto & vertex : verticies )
	{
		tetri_box.min = minimum( tetri_box.min , vertex.point );
		tetri_box.max = maximum( tetri_box.max , vertex.point );
	}

	return tetri_box;
}

void Tetrimino::rotation_z_about_point( std::vector<vertex_rgba_uv> & verticies , XMFLOAT3 point , float radians )
{
	for( auto & vertex : verticies )
	{
		vertex.point -= point;

		rotation_z( vertex.point , radians );

		vertex.point += point;
	}
}

bool Tetrimino::within_playfield( const Rotation in_rotation )
{
	Bounding_box playfield_box			= playfield->get_bounding_box();
	Bounding_box tetri_box				= get_bounding_box();
	vector<vertex_rgba_uv> verticies	= get_verticies();

	//debug_out( "\n tetri_box min.y = %.2f" , tetri_box.min.y );
	
	rotation_z_about_point( verticies , get_centre() , rotations.at( in_rotation ) );

	tetri_box = get_bounding_box( verticies ); // box HAS to be re-calculated after a rotation	

	if( ( tetri_box.min.x ) < playfield_box.min.x ||
		( tetri_box.max.x ) > playfield_box.max.x ||
		( tetri_box.min.y ) < playfield_box.min.y )	// y -50 < -125
	{
		return false; // return when first point outside bounds	
	}

	return true;
}

//XMFLOAT2 Tetrimino::get_block_size()
//{
//	return blocks.front().get_size();
//}

bool Tetrimino::within_playfield( const Direction in_direction )
{ 
	Bounding_box playfield_box	= playfield->get_bounding_box();
	Bounding_box tetri_box		= get_bounding_box();

	tetri_box += ( directions.at( in_direction ) );// *get_block_size() );
	
	if( ( tetri_box.min.x ) < playfield_box.min.x ||	
		( tetri_box.max.x ) > playfield_box.max.x ||	
		( tetri_box.min.y ) < playfield_box.min.y )	// y -50 < -125
	{
		return false; // return when first point outside bounds	
	}

	return true;
}

void Tetrimino::rotate( Rotation in_rotation )
{
	XMFLOAT3 centre = get_centre(); // get_tetri_centre()

	for( auto & block : blocks )
	{
		block.rotation_z_about_point( centre , rotations.at( in_rotation ) );
	}
}

void Tetrimino::try_rotate( Rotation in_rotation )
{
	if( within_playfield( in_rotation ) )
	{
		rotate( in_rotation );
	}
}