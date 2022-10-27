#include "KDException.h"
#include <sstream>

KDException::KDException( int line, const char* file ) noexcept
	:
	line(line),
	file(file)
{}

const char* KDException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl << GetOriginString();
	whatBuffer = oss.str();

	return whatBuffer.c_str();
}
const char* KDException::GetType() const noexcept
{
	return "KD Exception";
}
int KDException::GetLine() const noexcept
{
	return line;
}
const std::string& KDException::GetFile() const noexcept
{
	return file;
}
std::string KDException::GetOriginString() const noexcept
{
	std::ostringstream oss;
	oss << "[File] " << file << std::endl
		<< "[Line] " << line;

	return oss.str();
}
