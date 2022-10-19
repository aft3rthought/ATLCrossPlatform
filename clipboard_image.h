#include "ATLUtil/region.h"

#ifdef PLATFORM_WINDOWS
#include <atomic>
#endif

#ifdef PLATFORM_WINDOWS
#define atlpcconfig_platform_win_uwp
#endif

namespace atl
{
#ifdef PLATFORM_WINDOWS
enum class acquire_image_from_clipboard_status_type
{
	ready,
	in_progress,
	success,
	no_buffer,
	buffer_too_small,
	no_bitmap_on_clipboard,
	error_retrieving_bitmap,
};

enum class acquire_image_from_clipboard_windows_API_error_type
{
	none,
	buffer_too_small,
	error_retrieving_bitmap,
};
#endif

struct acquire_image_from_clipboard_request_type
{
#ifdef PLATFORM_WINDOWS
	std::atomic_bool loading_flag = false;
	std::atomic_bool done_flag = false;
	std::atomic<acquire_image_from_clipboard_windows_API_error_type> api_error = acquire_image_from_clipboard_windows_API_error_type::none;
	atl::region_type<unsigned char> backing_buffer = {nullptr, nullptr};
	unsigned int image_width = 0;
	unsigned int image_height = 0;

	acquire_image_from_clipboard_status_type acquire_image_from_clipboard();
#endif
};
}
