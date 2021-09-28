
#include "ATLUtil/numeric_casts.h"
#include "ATLCrossPlatform/file_system.h"
#include <collection.h>

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
			auto wide_string_length = MultiByteToWideChar(UINT{CP_UTF8}, DWORD{0}, in_file_path.begin(), atl::default_ptrdiff_to_int(in_file_path.size()), wide_string_buffer, 4096);
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
#pragma message("TODO: Consider setting these flags")
		//dataWriter.UnicodeEncoding(Windows::Storage::Streams::UnicodeEncoding::Utf16LE);
		//dataWriter.ByteOrder(Windows::Storage::Streams::ByteOrder::LittleEndian);
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

	file_write_begin_result_type file_writer_type::write(const atl::region_type<unsigned char>& bytes_to_write)
	{
		if(busy_flag) return file_write_begin_result_type::already_writing;
		busy_flag = true;
		error_flag = false;

		auto write_data = [this, bytes_to_write]()
		{
			if(storage_file != nullptr)
			{
				Windows::Storage::FileIO::WriteBytesAsync(storage_file, ::Platform::ArrayReference<unsigned char>(bytes_to_write.begin(), atl::default_ptrdiff_to_int(bytes_to_write.size())));
				busy_flag = false;
			}
			else
			{
				error_flag = true;
				busy_flag = false;
			}
		};

		if(storage_file != nullptr)
		{
			Concurrency::create_task(write_data);
		}
		else
		{
			auto file_save_picker = ref new ::Windows::Storage::Pickers::FileSavePicker;
			file_save_picker->SuggestedStartLocation = ::Windows::Storage::Pickers::PickerLocationId::DocumentsLibrary;
			file_save_picker->SuggestedFileName = L"document";
			auto extensions = ref new ::Platform::Collections::Vector<::Platform::String^>();
			extensions->Append(".data");
			file_save_picker->FileTypeChoices->Insert(L"Data", extensions);
			Concurrency::create_task(file_save_picker->PickSaveFileAsync())
				.then([this](::Windows::Storage::StorageFile^ created_file) { storage_file = created_file; })
				.then(write_data);
		}
		return file_write_begin_result_type::began_writing;
	}

	file_write_await_result_type file_writer_type::await()
	{
		if(busy_flag) return file_write_await_result_type::busy_writing;
		return file_write_await_result_type::not_writing;
	}
}
