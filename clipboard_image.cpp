#include "ATLUtil/numeric_casts.h"
#include "ATLCrossPlatform/copy_paste_image.h"

#ifdef PLATFORM_WINDOWS
#include <wrl.h>
#include <ppltasks.h>
#endif

#ifdef PLATFORM_WINDOWS
#define atlpcconfig_platform_win_uwp
#endif

namespace atl
{
#ifdef PLATFORM_WINDOWS
acquire_image_from_clipboard_status_type acquire_image_from_clipboard_request_type::acquire_image_from_clipboard()
{
	if(buffer.size() <= 0) return acquire_image_from_clipboard_status_type::no_buffer;
	if(loading_flag) return acquire_image_from_clipboard_status_type::in_progress;
	loading_flag = true;
	done_flag = false;

	// Get the bitmap and display it.
	auto dataPackageView = Windows::ApplicationModel::DataTransfer::Clipboard::GetContent();
	if(dataPackageView->Contains(Windows::ApplicationModel::DataTransfer::StandardDataFormats::Bitmap))
	{
		auto read_clipboard_task = Concurrency::create_task(dataPackageView->GetBitmapAsync());
		read_clipboard_task.then([this](Windows::Storage::Streams::IRandomAccessStreamReference^ clipboard_contents)
		{
			if(clipboard_contents != nullptr)
			{
				auto buffer_task = Concurrency::create_task(clipboard_contents->OpenReadAsync());
				buffer_task.then([this](Windows::Storage::Streams::IBuffer^ clipboard_buffer)
				{
					if(atl::safe_comparator(buffer.size()) < atl::safe_comparator(clipboard_buffer->Length))
					{
						api_error = acquire_image_from_clipboard_windows_API_error_type::buffer_too_small;
						done_flag = true;
						loading_flag = false;
					}
					else
					{
						auto clipboard_buffer_reader = Windows::Storage::Streams::DataReader::FromBuffer(clipboard_buffer);
						try
						{
							clipboard_buffer_reader->ReadBytes(::Platform::ArrayReference<unsigned char>(buffer.begin(), clipboard_buffer->Length));
						}
						catch(Platform::Exception^ e)
						{
							api_error = acquire_image_from_clipboard_windows_API_error_type::error_retrieving_bitmap;
							done_flag = true;
							loading_flag = false;
							return;
						}
						api_error = acquire_image_from_clipboard_windows_API_error_type::none;
						done_flag = true;
						loading_flag = false;
					}
				});
			}
			else
			{
				api_error = acquire_image_from_clipboard_windows_API_error_type::error_retrieving_bitmap;
				done_flag = true;
				loading_flag = false;
			}
		});
	}
	return acquire_image_from_clipboard_status_type::no_bitmap_on_clipboard;
}
#endif
}
