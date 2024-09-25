#include "pvz/utility/InputStream.h"


namespace pvz {

detail::FileStreamProvider::FileStreamProvider(const std::filesystem::path& path)
	: m_Stream(path.string(), std::ios::in | std::ios::binary)
{}

Buffer detail::FileStreamProvider::read(const uint32_t length) 
{
	Buffer buffer(length);
	m_Stream.read(reinterpret_cast<char*>(buffer.data()), buffer.size());

	return buffer;
}

InputStream::InputStream(IStreamPtr stream)
	: m_Stream(std::move(stream))
{}

Buffer InputStream::read(const uint32_t length) {
	return m_Stream->read(length);
}

InputStream loadFile(const std::filesystem::path& path) {
	return InputStream(std::make_unique<detail::FileStreamProvider>(path));
}

}
