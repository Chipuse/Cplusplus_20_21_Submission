#include "BinArchive.h"
#include <fstream>

BinaryOutputArchive::BinaryOutputArchive(const std::string& filePath)
{
	m_fileStream.open(filePath, std::ios::binary);
}

void BinaryOutputArchive::Serialize(bool& value, const std::string& label)
{
	Write(value);
}

void BinaryOutputArchive::Serialize(sf::Uint8& value, const std::string& label)
{
	Write(value);
}

void BinaryOutputArchive::Serialize(sf::Int8& value, const std::string& label)
{
	Write(value);
}

void BinaryOutputArchive::Serialize(sf::Uint16& value, const std::string& label)
{
	Write(value);
}

void BinaryOutputArchive::Serialize(sf::Int16& value, const std::string& label)
{
	Write(value);
}

void BinaryOutputArchive::Serialize(sf::Uint32& value, const std::string& label)
{
	Write(value);
}

void BinaryOutputArchive::Serialize(sf::Int32& value, const std::string& label)
{
	Write(value);
}

void BinaryOutputArchive::Serialize(sf::Uint64& value, const std::string& label)
{
	Write(value);
}

void BinaryOutputArchive::Serialize(sf::Int64& value, const std::string& label)
{
	Write(value);
}

void BinaryOutputArchive::Serialize(float& value, const std::string& label)
{
	Write(value);
}

void BinaryOutputArchive::Serialize(double& value, const std::string& label)
{
	Write(value);
}

void BinaryOutputArchive::Serialize(std::string& value, const std::string& label)
{
	const size_t length = value.length();
	Write(length);

	m_fileStream.write(value.data(), length);
}

void BinaryOutputArchive::Serialize(VectorInterface& value, const std::string& label)
{
	const size_t size = value.GetSize();
	Write(size);

	for (int i = 0; i < size; i++)
	{
		value.SerializeElement(*this, i);
	}
}

void BinaryOutputArchive::Serialize(ObjectInterface& value, const std::string& label)
{
	value.SerializeObject(*this);
}








BinaryInputArchive::BinaryInputArchive(const std::string& filePath)
{
	m_fileStream.open(filePath, std::ios::binary);
}

void BinaryInputArchive::Serialize(bool& value, const std::string& label)
{
	Read(value);
}

void BinaryInputArchive::Serialize(sf::Uint8& value, const std::string& label)
{
	Read(value);
}

void BinaryInputArchive::Serialize(sf::Int8& value, const std::string& label)
{
	Read(value);
}

void BinaryInputArchive::Serialize(sf::Uint16& value, const std::string& label)
{
	Read(value);
}

void BinaryInputArchive::Serialize(sf::Int16& value, const std::string& label)
{
	Read(value);
}

void BinaryInputArchive::Serialize(sf::Uint32& value, const std::string& label)
{
	Read(value);
}

void BinaryInputArchive::Serialize(sf::Int32& value, const std::string& label)
{
	Read(value);
}

void BinaryInputArchive::Serialize(sf::Uint64& value, const std::string& label)
{
	Read(value);
}

void BinaryInputArchive::Serialize(sf::Int64& value, const std::string& label)
{
	Read(value);
}

void BinaryInputArchive::Serialize(float& value, const std::string& label)
{
	Read(value);
}

void BinaryInputArchive::Serialize(double& value, const std::string& label)
{
	Read(value);
}

void BinaryInputArchive::Serialize(std::string& value, const std::string& label)
{
	size_t length = 0;
	Read(length);

	value.resize(length);
	m_fileStream.read(value.data(), length);
	sizeRead += length;
}

void BinaryInputArchive::Serialize(VectorInterface& value, const std::string& label)
{
	size_t size = 0;
	Read(size);

	value.Resize(size);

	for (int i = 0; i < size; i++)
	{
		value.SerializeElement(*this, i);
	}
}

void BinaryInputArchive::Serialize(ObjectInterface& value, const std::string& label)
{
	value.SerializeObject(*this);
}
