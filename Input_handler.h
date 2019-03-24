#pragma once

class Input_handler
{
	public:

	Input_handler()
	{
		// Methods to bind commands...
		//command_map.insert( make_pair( Keyboard::Keys::Z , new Rotate( Rotation::clock_wise ) ) );
		//command_map.insert( make_pair( Keyboard::Keys::X , new Rotate( Rotation::counter_clock_wise ) ) );
	}

	Command * handle_input( const uchar in_char )
	{
		const auto result = command_map.find( in_char );

		if( result != command_map.end() ) return result->second;
		else return nullptr;
	}

	// add_command()

	private:

	map< uchar , Command * > command_map;
};