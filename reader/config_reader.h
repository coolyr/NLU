#ifndef DIALOG_UTIL_CONFIG_READER_H_
#define DIALOG_UTIL_CONFIG_READER_H_

#include <libconfig.h++>
#include <string>
#include <autil/StringUtil.h>
#include "../log.h"
#include "../common_macros.h"

DIALOG_SEG_BEGIN_NAMESPACE(qp);
class ConfigReader;

class SettingReader
{
public:
	explicit SettingReader(libconfig::Setting& setting)
		: setting_(setting)
	{
	}
	~SettingReader() {}

public:
	bool Has(const std::string& path)
	{
		return setting_.exists(path.c_str());
	}

	int32_t Size()
	{
		return setting_.getLength();
	}

	SettingReader operator[](int32_t index) const
	{
		return SettingReader(setting_[index]);
	}

	template<typename ValueT>
	ValueT Get(const std::string& path, const ValueT& default_value = ValueT());

	template<typename ValueT>
	bool Get(const std::string& path, ValueT* value);

private:
	libconfig::Setting& setting_;
};  // SettingReader

class ConfigReader
{
public:
	explicit ConfigReader(const std::string& path)
	{
		cfg_.readFile(path.c_str());
	}

	~ConfigReader() {}

public:
	bool Has(const std::string& path)
	{
		return cfg_.exists(path.c_str());
	}

	int32_t Size(const std::string& path)
	{
		return cfg_.lookup(path.c_str()).getLength();
	}

	SettingReader Get(const std::string& path)
	{
		return SettingReader(cfg_.lookup(path.c_str()));
	}

	template<typename ValueT>
	ValueT Get(const std::string& path, const ValueT& default_value = ValueT());

	template<typename ValueT>
	bool Get(const std::string& path, ValueT* value);

private:
	libconfig::Config cfg_;

private:
	DISALLOW_COPY_AND_ASSIGN(ConfigReader);
};  // ConfigReader

template<typename ValueT>
ValueT ConfigReader::Get(const std::string& path,
	const ValueT& default_value)
{
	ValueT value;
	return cfg_.lookupValue(path.c_str(), value) ? value : default_value;
}  // Get

template<typename ValueT>
bool ConfigReader::Get(const std::string& path, ValueT* value)
{
	return cfg_.lookupValue(path.c_str(), *value);
}  // Get

template<typename ValueT>
ValueT SettingReader::Get(const std::string& path,
	const ValueT& default_value)
{
	ValueT value;
	return setting_.lookupValue(path.c_str(), value) ? value : default_value;
}  // Get

template<typename ValueT>
bool SettingReader::Get(const std::string& path, ValueT* value)
{
	return setting_.lookupValue(path.c_str(), *value);
}  // Get

DIALOG_SEG_END_NAMESPACE(qp);

#endif  // DIALOG_UTIL_CONFIG_READER_H_
