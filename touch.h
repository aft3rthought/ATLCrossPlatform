
#include "ATLUtil/math2d.h"

#pragma once

namespace atl
{
    enum class key_event_code : int
    {
        unrecognized,

        number_0,
        number_1,
        number_2,
        number_3,
        number_4,
        number_5,
        number_6,
        number_7,
        number_8,
        number_9,

        key_tab,
        key_esc,

        modifier_shift,
        modifier_ctrl,
        modifier_alt,

        arrow_up,
        arrow_right,
        arrow_down,
        arrow_left,

        letter_a,
        letter_b,
        letter_c,
        letter_d,
        letter_e,
        letter_f,
        letter_g,
        letter_h,
        letter_i,
        letter_j,
        letter_k,
        letter_l,
        letter_m,
        letter_n,
        letter_o,
        letter_p,
        letter_q,
        letter_r,
        letter_s,
        letter_t,
        letter_u,
        letter_v,
        letter_w,
        letter_x,
        letter_y,
        letter_z,
    };

    enum class key_event_modifier : int
    {
        shift = 1,
        ctrl = 2,
        alt = 4
    };

    inline bool key_event_code_is_number(key_event_code code)
    {
        switch(code)
        {
            case key_event_code::number_0:
            case key_event_code::number_1:
            case key_event_code::number_2:
            case key_event_code::number_3:
            case key_event_code::number_4:
            case key_event_code::number_5:
            case key_event_code::number_6:
            case key_event_code::number_7:
            case key_event_code::number_8:
            case key_event_code::number_9:
                return true;
            default:
                break;
        }
        return false;
    }

    inline bool key_event_code_is_letter(key_event_code code)
    {
        switch(code)
        {
            case key_event_code::letter_a:
            case key_event_code::letter_b:
            case key_event_code::letter_c:
            case key_event_code::letter_d:
            case key_event_code::letter_e:
            case key_event_code::letter_f:
            case key_event_code::letter_g:
            case key_event_code::letter_h:
            case key_event_code::letter_i:
            case key_event_code::letter_j:
            case key_event_code::letter_k:
            case key_event_code::letter_l:
            case key_event_code::letter_m:
            case key_event_code::letter_n:
            case key_event_code::letter_o:
            case key_event_code::letter_p:
            case key_event_code::letter_q:
            case key_event_code::letter_r:
            case key_event_code::letter_s:
            case key_event_code::letter_t:
            case key_event_code::letter_u:
            case key_event_code::letter_v:
            case key_event_code::letter_w:
            case key_event_code::letter_x:
            case key_event_code::letter_y:
            case key_event_code::letter_z:
                return true;
            default:
                break;
        }
        return false;
    }

    struct key_event
    {
        key_event_code code;
        key_event_modifier modifiers_mask;

        key_event(key_event_code in_code,
                  key_event_modifier in_modifiers_mask)
        :
        code(in_code),
        modifiers_mask(in_modifiers_mask)
        {}
    };

    namespace standard_cursor_components
    {
        const int left_mouse_button = -1;
        const int right_mouse_button = -2;
        const int middle_mouse_button = -3;
    }

    template <int MaxComponents>
    struct user_cursor
    {
        struct component_list
        {
            component_list() : count(0) {};

            int count;
            int components[MaxComponents];

            bool has(int in_component) const
            {
                for(auto component : *this)
                {
                    if(component == in_component)
                        return true;
                }
                return false;
            }
            bool full() const { return count < MaxComponents; }
            void add(int in_id) { components[count] = in_id; count++; }
            void reset() { count = 0; }
            auto begin() const { return std::begin(components); }
            auto end() const { return std::begin(components) + count; }
        };

        component_list new_components;
        component_list steady_components;
        component_list lost_components;
        atl::point2f position;
        atl::point2f last_position;

        void update(const atl::point2f & in_position, const component_list & in_current_components)
        {
            last_position = position;
            position = in_position;

            // for everything previously new or steady but not current, add to lost
            lost_components.reset();
            for(const auto & id : steady_components)
            {
                if(!in_current_components.has(id))
                    lost_components.add(id);
            }
            for(const auto & id : new_components)
            {
                if(!in_current_components.has(id) && !lost_components.has(id))
                    lost_components.add(id);
            }

            // for everything in new & current, add to steady
            steady_components.reset();
            for(const auto & id : new_components)
            {
                if(in_current_components.has(id))
                    steady_components.add(id);
            }

            // for everything no in steady (after an update) but in current, add to new
            new_components.reset();
            for(const auto & id : in_current_components)
            {
                if(!steady_components.has(id))
                    new_components.add(id);
            }
        }
    };
}