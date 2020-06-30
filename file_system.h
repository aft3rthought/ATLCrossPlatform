

#pragma once

#include "ATLUtil/region.h"

#ifdef PLATFORM_IOS
#include <fstream>
#endif

#ifdef PLATFORM_OSX
#include <fstream>
#endif

#ifdef PLATFORM_WINDOWS
#include <wrl.h>
#include <ppltasks.h>
#endif

namespace atl
{
	enum class file_loader_prepare_result
	{
		loading,
		error_could_not_load_file,
		nothing_left_to_do,
    };

	enum class file_loader_read_result_status
	{
		need_to_call_prepare,
		loading,
		wrote_to_buffer,
		error_could_not_load_file,
		error_insufficient_buffer_size,
	};

	struct file_loader_read_result
	{
		file_loader_read_result_status status;
		size_t minimum_buffer_size;
	};

    struct file_loader_type
    {
		file_loader_prepare_result prepare(const atl::region_type<const char> & in_file_path);
		file_loader_read_result read(const atl::region_type<unsigned char> & in_output_buffer);
		void free();

#ifdef PLATFORM_WINDOWS
		std::atomic_bool busy_flag = false;
		std::atomic_bool error_flag = false;
		Windows::Storage::StorageFile ^ storage_file = nullptr;
		Windows::Storage::Streams::IBuffer ^ file_stream_buffer = nullptr;
#endif
    };

	enum class file_write_begin_result_type
	{
		already_writing,
		failed_to_start_writing,
		began_writing,
	};

	enum class file_write_await_result_type
	{
		busy_writing,
		not_writing,
	};

	struct file_writer_type
	{
		file_write_begin_result_type write(const atl::region_type<unsigned char>& in_output_buffer);
		file_write_await_result_type await();

#ifdef PLATFORM_WINDOWS
		std::atomic_bool busy_flag = false;
		std::atomic_bool error_flag = false;
		Windows::Storage::StorageFile^ storage_file = nullptr;
#endif
	};
}
