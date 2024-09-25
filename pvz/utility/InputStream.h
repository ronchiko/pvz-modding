#pragma once

#include <vector>
#include <memory>
#include <fstream>
#include <filesystem>

#include "pvz/utility/Buffer.h"


namespace pvz {

namespace detail {

class IStreamProvider {
public:
	virtual ~IStreamProvider() = default;

	virtual Buffer read(uint32_t length) = 0;
};

/**
   An input stream that is sourced from a file.
 */
class FileStreamProvider final : public IStreamProvider {
public:
	explicit FileStreamProvider(const std::filesystem::path& path);

	Buffer read(uint32_t length) override;

private:
	std::ifstream m_Stream;
};

}

class InputStream {
public:
	using IStreamPtr = std::unique_ptr<detail::IStreamProvider>;

	explicit InputStream(IStreamPtr stream);

	/**
	   Reads a POD type from the stream.
	 */
	template<typename PodT>
	PodT read();

	/**
	   Reads an amount of bytes from the stream.
	 */
	Buffer read(uint32_t length);

private:
	IStreamPtr m_Stream;
};

/**
	Attempts to load the file from the stream.

	@param path - The path to load.
 */
InputStream loadFile(const std::filesystem::path& path);

template<typename PodT>
PodT InputStream::read() {
	const auto data = read(sizeof(PodT));
	return *reinterpret_cast<const PodT*>(data.data());
}

}
