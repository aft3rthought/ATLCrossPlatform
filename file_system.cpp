
#include "ATLUtil/numeric_casts.h"
#include "ATLCrossPlatform/file_system.h"

#ifdef PLATFORM_WINDOWS
#define atlpcconfig_platform_win_uwp
#endif

namespace atl
{
	void file_loader_type::free()
	{
#if defined(atlpcconfig_platform_mac_osx) || defined(atlpcconfig_platform_ios)
#elif defined(atlpcconfig_platform_win_uwp)
		if(!busy_flag)
		{
			storage_file = nullptr;
			file_stream_buffer = nullptr;
			busy_flag = false;
			error_flag = false;
		}
#endif
	}

	file_loader_prepare_result file_loader_type::prepare(const atl::region_type<const char> & in_file_path)
	{
#if defined(atlpcconfig_platform_mac_osx) || defined(atlpcconfig_platform_ios)
		std::ifstream file(in_application_folder.rootDirectory() + "/" + in_file_path, std::ios::in);
		internal_status = file_data_status::error;
		if(file.good())
		{
			std::streamsize size = 0;
			if(file.seekg(0, std::ios::end).good())
			{
				size = file.tellg();

				if(file.seekg(0, std::ios::beg).good())
				{
					size -= file.tellg();

					internal_data.resize(size);
					file.read((char *)internal_data.data(), size);
					internal_status = file_data_status::ready;
				}
			}
		}
#elif defined(atlpcconfig_platform_win_uwp)
		if(error_flag)
			return file_loader_prepare_result::error_could_not_load_file;

		if(busy_flag)
			return file_loader_prepare_result::loading;

		if(storage_file == nullptr || file_stream_buffer == nullptr)
		{
			busy_flag = true;
			auto storage_folder = Windows::ApplicationModel::Package::Current->InstalledLocation;
			char16 wide_string_buffer[4098];
			auto wide_string_length = MultiByteToWideChar(CP_UTF8, 0, in_file_path.begin(), in_file_path.size(), wide_string_buffer, 4096);
			wide_string_buffer[wide_string_length] = 0;
			wide_string_buffer[wide_string_length + 1] = 0;

			auto file_task = Concurrency::create_task(storage_folder->GetFileAsync(::Platform::StringReference(wide_string_buffer, wide_string_length)));
			file_task.then([this](Windows::Storage::StorageFile ^ loaded_file)
			{
				if(loaded_file != nullptr)
				{
					storage_file = loaded_file;
					auto buffer_task = Concurrency::create_task(Windows::Storage::FileIO::ReadBufferAsync(loaded_file));
					buffer_task.then([this](Windows::Storage::Streams::IBuffer ^ buffer)
					{
						file_stream_buffer = buffer;
						busy_flag = false;
					});
				}
				else
					error_flag = true;
			})
			.then([this](Concurrency::task<void> t)
			{
				try
				{
					t.get();
				}
				catch(Platform::COMException^ e)
				{
					error_flag = true;
					busy_flag = false;
				}
			});
			return file_loader_prepare_result::loading;
		}
		return file_loader_prepare_result::nothing_left_to_do;
#endif
	}

	file_loader_read_result file_loader_type::read(const atl::region_type<unsigned char> & in_output_buffer)
	{
		if(error_flag)
			return {file_loader_read_result_status::error_could_not_load_file, 0};

		if(busy_flag)
			return {file_loader_read_result_status::loading, 0};

		if(storage_file == nullptr || file_stream_buffer == nullptr)
			return {file_loader_read_result_status::need_to_call_prepare, 0};

		if(atl::safe_comparator(in_output_buffer.size()) < atl::safe_comparator(file_stream_buffer->Length))
			return {file_loader_read_result_status::error_insufficient_buffer_size, file_stream_buffer->Length};

		auto data_reader = Windows::Storage::Streams::DataReader::FromBuffer(file_stream_buffer);
		try
		{
			data_reader->ReadBytes(::Platform::ArrayReference<unsigned char>(in_output_buffer.begin(), file_stream_buffer->Length));
		}
		catch(Platform::Exception^ e)
		{
			delete data_reader; // As a best practice, explicitly close the dataReader resource as soon as it is no longer needed.
			return {file_loader_read_result_status::error_could_not_load_file, 0};
		}
		delete data_reader; // As a best practice, explicitly close the dataReader resource as soon as it is no longer needed.                    
		return {file_loader_read_result_status::wrote_to_buffer, file_stream_buffer->Length};
	}

}
