#include "ATLUtil/numeric_casts.h"
#include "ATLCrossPlatform/clipboard_image.h"

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
	if(backing_buffer.size() <= 0) return acquire_image_from_clipboard_status_type::no_buffer;
	if(loading_flag) return acquire_image_from_clipboard_status_type::in_progress;
	loading_flag = true;
	done_flag = false;

	// Get the bitmap and display it.
	auto data_package_view = Windows::ApplicationModel::DataTransfer::Clipboard::GetContent();
	if(data_package_view->Contains(Windows::ApplicationModel::DataTransfer::StandardDataFormats::Bitmap))
	{
		auto read_clipboard_task = Concurrency::create_task(data_package_view->GetBitmapAsync());
		read_clipboard_task.then([this](Windows::Storage::Streams::IRandomAccessStreamReference^ clipboard_contents)
		{
			if(clipboard_contents != nullptr)
			{
				auto buffer_task = Concurrency::create_task(clipboard_contents->OpenReadAsync());
				buffer_task.then([this](Windows::Storage::Streams::IRandomAccessStream^ clipboard_buffer)
				{
					auto bitmap_set_source_task = Concurrency::create_task(Windows::Graphics::Imaging::BitmapDecoder::CreateAsync(clipboard_buffer));
					bitmap_set_source_task.then([this](Windows::Graphics::Imaging::BitmapDecoder^ decoder)
					{
							const auto clipboard_image_pixel_width = decoder->PixelWidth;
							const auto clipboard_image_pixel_height = decoder->PixelHeight;
							const auto bytes_needed = clipboard_image_pixel_width * clipboard_image_pixel_height * 4;
							if(atl::safe_comparator(backing_buffer.size()) < atl::safe_comparator(bytes_needed))
							{
								api_error = acquire_image_from_clipboard_windows_API_error_type::buffer_too_small;
								done_flag = true;
								loading_flag = false;
							}
							else
							{
								image_width = clipboard_image_pixel_width;
								image_height = clipboard_image_pixel_height;
								Windows::Graphics::Imaging::BitmapTransform^ identity_bitmap_transform = ref new Windows::Graphics::Imaging::BitmapTransform;
								auto decoder_task = Concurrency::create_task(decoder->GetPixelDataAsync(Windows::Graphics::Imaging::BitmapPixelFormat::Rgba8,
																			 Windows::Graphics::Imaging::BitmapAlphaMode::Straight,
																			 identity_bitmap_transform,
																			 Windows::Graphics::Imaging::ExifOrientationMode::RespectExifOrientation,
																			 Windows::Graphics::Imaging::ColorManagementMode::DoNotColorManage));
								decoder_task.then([this](Windows::Graphics::Imaging::PixelDataProvider^ pixel_data_provider)
								{
									if(pixel_data_provider)
									{
										Platform::Array<unsigned char>^ bitmap_bytes = pixel_data_provider->DetachPixelData();
										std::copy(bitmap_bytes->begin(), bitmap_bytes->end(), backing_buffer.begin());
										api_error = acquire_image_from_clipboard_windows_API_error_type::none;
										done_flag = true;
										loading_flag = false;
									}
									else
									{
										api_error = acquire_image_from_clipboard_windows_API_error_type::error_retrieving_bitmap;
										done_flag = true;
										loading_flag = false;
									}
								});
							}
					});
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
	loading_flag = false;
	return acquire_image_from_clipboard_status_type::no_bitmap_on_clipboard;
}
#endif
}
