#pragma once

#include "IArchive.h"
#include <fstream>

class BinaryOutputArchive : public IArchive
{

public:

	BinaryOutputArchive(const std::string& filePath);

	virtual bool IsInput() const override { return false; }
	virtual bool IsOutput() const override { return true; }

	virtual void Serialize(bool& value, const std::string& label) override;
	virtual void Serialize(sf::Uint8& value, const std::string& label) override;
	virtual void Serialize(sf::Int8& value, const std::string& label) override;
	virtual void Serialize(sf::Uint16& value, const std::string& label) override;
	virtual void Serialize(sf::Int16& value, const std::string& label) override;
	virtual void Serialize(sf::Uint32& value, const std::string& label) override;
	virtual void Serialize(sf::Int32& value, const std::string& label) override;
	virtual void Serialize(sf::Uint64& value, const std::string& label) override;
	virtual void Serialize(sf::Int64& value, const std::string& label) override;
	virtual void Serialize(float& value, const std::string& label) override;
	virtual void Serialize(double& value, const std::string& label) override;
	virtual void Serialize(std::string& value, const std::string& label) override;
	virtual void Serialize(VectorInterface& value, const std::string& label) override;
	virtual void Serialize(ObjectInterface& value, const std::string& label) override;

	template<typename T>
	void Write(const T& value)
	{
		m_fileStream.write(reinterpret_cast<const char*>(&value), sizeof(T));
	}

private:

	std::ofstream m_fileStream;

};

class BinaryInputArchive : public IArchive
{
public:

	BinaryInputArchive(const std::string& filePath);

	virtual bool IsInput() const override { return true; }
	virtual bool IsOutput() const override { return false; }

	virtual void Serialize(bool& value, const std::string& label) override;
	virtual void Serialize(sf::Uint8& value, const std::string& label) override;
	virtual void Serialize(sf::Int8& value, const std::string& label) override;
	virtual void Serialize(sf::Uint16& value, const std::string& label) override;
	virtual void Serialize(sf::Int16& value, const std::string& label) override;
	virtual void Serialize(sf::Uint32& value, const std::string& label) override;
	virtual void Serialize(sf::Int32& value, const std::string& label) override;
	virtual void Serialize(sf::Uint64& value, const std::string& label) override;
	virtual void Serialize(sf::Int64& value, const std::string& label) override;
	virtual void Serialize(float& value, const std::string& label) override;
	virtual void Serialize(double& value, const std::string& label) override;
	virtual void Serialize(std::string& value, const std::string& label) override;
	virtual void Serialize(VectorInterface& value, const std::string& label) override;
	virtual void Serialize(ObjectInterface& value, const std::string& label) override;

	template<typename T>
	void Read(T& value)
	{
		m_fileStream.read(reinterpret_cast<char*>(&value), sizeof(T));
		sizeRead += sizeof(T);
	}

private:

	std::ifstream m_fileStream;
	size_t fileSize = 0;
	size_t sizeRead = 0;

};