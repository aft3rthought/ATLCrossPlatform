
#include "ATLUtil/math2d.h"

#pragma once

namespace atlxp
{
	struct switch_input_event_type
	{
		unsigned switch_identifier;
		unsigned device_identifier;
		unsigned event_data;
	};

	inline bool operator == (const switch_input_event_type & lhs, const switch_input_event_type & rhs)
	{
		return lhs.switch_identifier == rhs.switch_identifier && lhs.device_identifier == rhs.device_identifier && lhs.event_data == rhs.event_data;
	}

	inline bool operator != (const switch_input_event_type & lhs, const switch_input_event_type & rhs)
	{
		return !(lhs == rhs);
	}

	constexpr switch_input_event_type switch_input_event(unsigned switch_identifier, unsigned device_identifier, unsigned event_data)
	{
		return {switch_identifier, device_identifier, event_data};
	}

	struct moveable_point_input_type
	{
		bool valid;
		atl::point2f position;
		atl::point2f previous_position;

		void update(const atl::point2f & new_position)
		{
			valid = true;
			previous_position = position;
			position = new_position;
		}

		void invalidate()
		{
			valid = false;
		}
	};

	namespace mouse_switch_events
	{
		constexpr unsigned unrecognized = 0;
		constexpr unsigned engage = 1;
		constexpr unsigned disengage = 2;
	}

	namespace mouse_switch_identifier
	{
		constexpr unsigned unrecognized = 0;
		constexpr unsigned button_left = 1;
		constexpr unsigned button_middle = 2;
		constexpr unsigned button_right = 3;
		constexpr unsigned wheel_up = 4;
		constexpr unsigned wheel_down = 5;
	}

	namespace keyboard_switch_events
	{
		constexpr unsigned unrecognized = 0;
		constexpr unsigned engage = 1;
		constexpr unsigned disengage = 2;
		constexpr unsigned repeat = 3;
	}

	namespace keyboard_switch_identifier
	{
		constexpr unsigned unrecognized = 1;

		constexpr unsigned ascii_range_begin = 5000;
		constexpr unsigned ascii_range_end = 5127;

		constexpr unsigned number_0 = ascii_range_begin + unsigned{'0'};
		constexpr unsigned number_1 = ascii_range_begin + unsigned{'1'};
		constexpr unsigned number_2 = ascii_range_begin + unsigned{'2'};
		constexpr unsigned number_3 = ascii_range_begin + unsigned{'3'};
		constexpr unsigned number_4 = ascii_range_begin + unsigned{'4'};
		constexpr unsigned number_5 = ascii_range_begin + unsigned{'5'};
		constexpr unsigned number_6 = ascii_range_begin + unsigned{'6'};
		constexpr unsigned number_7 = ascii_range_begin + unsigned{'7'};
		constexpr unsigned number_8 = ascii_range_begin + unsigned{'8'};
		constexpr unsigned number_9 = ascii_range_begin + unsigned{'9'};

		constexpr unsigned letter_a = ascii_range_begin + unsigned{'a'};
		constexpr unsigned letter_b = ascii_range_begin + unsigned{'b'};
		constexpr unsigned letter_c = ascii_range_begin + unsigned{'c'};
		constexpr unsigned letter_d = ascii_range_begin + unsigned{'d'};
		constexpr unsigned letter_e = ascii_range_begin + unsigned{'e'};
		constexpr unsigned letter_f = ascii_range_begin + unsigned{'f'};
		constexpr unsigned letter_g = ascii_range_begin + unsigned{'g'};
		constexpr unsigned letter_h = ascii_range_begin + unsigned{'h'};
		constexpr unsigned letter_i = ascii_range_begin + unsigned{'i'};
		constexpr unsigned letter_j = ascii_range_begin + unsigned{'j'};
		constexpr unsigned letter_k = ascii_range_begin + unsigned{'k'};
		constexpr unsigned letter_l = ascii_range_begin + unsigned{'l'};
		constexpr unsigned letter_m = ascii_range_begin + unsigned{'m'};
		constexpr unsigned letter_n = ascii_range_begin + unsigned{'n'};
		constexpr unsigned letter_o = ascii_range_begin + unsigned{'o'};
		constexpr unsigned letter_p = ascii_range_begin + unsigned{'p'};
		constexpr unsigned letter_q = ascii_range_begin + unsigned{'q'};
		constexpr unsigned letter_r = ascii_range_begin + unsigned{'r'};
		constexpr unsigned letter_s = ascii_range_begin + unsigned{'s'};
		constexpr unsigned letter_t = ascii_range_begin + unsigned{'t'};
		constexpr unsigned letter_u = ascii_range_begin + unsigned{'u'};
		constexpr unsigned letter_v = ascii_range_begin + unsigned{'v'};
		constexpr unsigned letter_w = ascii_range_begin + unsigned{'w'};
		constexpr unsigned letter_x = ascii_range_begin + unsigned{'x'};
		constexpr unsigned letter_y = ascii_range_begin + unsigned{'y'};
		constexpr unsigned letter_z = ascii_range_begin + unsigned{'z'};

		constexpr unsigned key_space = ascii_range_begin + unsigned{' '};
		constexpr unsigned key_enter = ascii_range_begin + unsigned{'\n'};
		constexpr unsigned key_tab = ascii_range_begin + unsigned{'\t'};
		constexpr unsigned key_comma = ascii_range_begin + unsigned{','};
		constexpr unsigned key_period = ascii_range_begin + unsigned{'.'};
		constexpr unsigned key_grave = ascii_range_begin + unsigned{'`'};
		constexpr unsigned key_semicolon = ascii_range_begin + unsigned{';'};
		constexpr unsigned key_single_quote = ascii_range_begin + unsigned{'\''};
		constexpr unsigned key_left_bracket = ascii_range_begin + unsigned{'['};
		constexpr unsigned key_right_bracket = ascii_range_begin + unsigned{']'};
		constexpr unsigned key_minus_sign = ascii_range_begin + unsigned{'-'};
		constexpr unsigned key_equals_sign = ascii_range_begin + unsigned{'='};
		constexpr unsigned key_forward_slash = ascii_range_begin + unsigned{'/'};
		//constexpr unsigned key_backslash = ascii_range_begin + unsigned{'\\'};

		constexpr unsigned key_esc = 6000;
		constexpr unsigned key_backspace = 6001;
		constexpr unsigned key_delete = 6002;

		constexpr unsigned modifier_left_shift = 7000;
		constexpr unsigned modifier_right_shift = 7001;
		constexpr unsigned modifier_shift = 7002;
		constexpr unsigned modifier_left_control = 7003;
		constexpr unsigned modifier_right_control = 7004;
		constexpr unsigned modifier_control = 7005;
		constexpr unsigned modifier_alt = 7006;

		constexpr unsigned arrow_up = 8000;
		constexpr unsigned arrow_right = 8001;
		constexpr unsigned arrow_down = 8002;
		constexpr unsigned arrow_left = 8003;

		constexpr unsigned function_key_f1 = 10000;
		constexpr unsigned function_key_f2 = 10001;
		constexpr unsigned function_key_f3 = 10002;
		constexpr unsigned function_key_f4 = 10003;
		constexpr unsigned function_key_f5 = 10004;
		constexpr unsigned function_key_f6 = 10005;
		constexpr unsigned function_key_f7 = 10006;
		constexpr unsigned function_key_f8 = 10007;
		constexpr unsigned function_key_f9 = 10008;
		constexpr unsigned function_key_f10 = 10009;
		constexpr unsigned function_key_f11 = 10010;
		constexpr unsigned function_key_f12 = 10011;
	}
}
