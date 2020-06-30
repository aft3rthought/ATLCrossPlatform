
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

		constexpr unsigned number_0 = 5000;
		constexpr unsigned number_1 = 5001;
		constexpr unsigned number_2 = 5002;
		constexpr unsigned number_3 = 5003;
		constexpr unsigned number_4 = 5004;
		constexpr unsigned number_5 = 5005;
		constexpr unsigned number_6 = 5006;
		constexpr unsigned number_7 = 5007;
		constexpr unsigned number_8 = 5008;
		constexpr unsigned number_9 = 5009;

		constexpr unsigned key_space = 6000;
		constexpr unsigned key_tab = 6001;
		constexpr unsigned key_esc = 6002;
		constexpr unsigned key_comma = 6003;
		constexpr unsigned key_period = 6004;
		constexpr unsigned key_grave = 6005;
		constexpr unsigned key_semicolon = 6006;
		constexpr unsigned key_single_quote = 6007;
		constexpr unsigned key_left_bracket = 6008;
		constexpr unsigned key_right_bracket = 6009;
		constexpr unsigned key_minus_sign = 6010;
		constexpr unsigned key_equals_sign = 6011;
		constexpr unsigned key_backspace = 6012;

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

		constexpr unsigned letter_a = 9000;
		constexpr unsigned letter_b = 9001;
		constexpr unsigned letter_c = 9002;
		constexpr unsigned letter_d = 9003;
		constexpr unsigned letter_e = 9004;
		constexpr unsigned letter_f = 9005;
		constexpr unsigned letter_g = 9006;
		constexpr unsigned letter_h = 9007;
		constexpr unsigned letter_i = 9008;
		constexpr unsigned letter_j = 9009;
		constexpr unsigned letter_k = 9010;
		constexpr unsigned letter_l = 9011;
		constexpr unsigned letter_m = 9012;
		constexpr unsigned letter_n = 9013;
		constexpr unsigned letter_o = 9014;
		constexpr unsigned letter_p = 9015;
		constexpr unsigned letter_q = 9016;
		constexpr unsigned letter_r = 9017;
		constexpr unsigned letter_s = 9018;
		constexpr unsigned letter_t = 9019;
		constexpr unsigned letter_u = 9020;
		constexpr unsigned letter_v = 9021;
		constexpr unsigned letter_w = 9022;
		constexpr unsigned letter_x = 9023;
		constexpr unsigned letter_y = 9024;
		constexpr unsigned letter_z = 9025;

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
