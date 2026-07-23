#pragma once

#include "soundplayer.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <variant>
#include <vector>

extern std::string ContentDirectory;
extern std::string SoundDirectory;
extern std::string ConfigPath;
extern std::string MidiMapPath;


// #ifndef GENERAL_H_
// #define GENERAL_H_
// template<typename Var>
struct ConfigurationVariables
{
	std::string MicrophoneOutput = "";
	std::string PlaybackOutput   = "";

	int MuteSoundboard = -1;
	int PlayLastSound  = -1;

	int         MidiPort     = 1;
	std::string MidiPortName = "";
};
extern struct ConfigurationVariables* Configs;
// #endif

// Singleton
class Config
{
 public:
	enum ConfigType { STRING, INT };
	enum ConfigKey { MicrophoneOutput, MuteSoundboard, PlayLastSound, PlaybackOutput, MidiPortName, MidiPort, LENGTH };
	typedef std::variant<std::string, int> ConfigValue;

 private:
	std::array<std::string, LENGTH> config_keys;

	inline std::string getConfigKey(const std::string& line);
	inline std::string getConfigValue(const std::string& line);
	inline bool        isCommentOrEmpty(const std::string_view& line);

 protected:
	Config()
	{
		config_keys[MidiPort]     = "0";
		config_keys[MidiPortName] = "null";
	}
	Config(const Config&)            = delete;
	Config& operator=(const Config&) = delete;

	static Config* instance;

 public:
	static inline constexpr int keyGetType(ConfigKey key)
	{
		switch (key) {
		case MicrophoneOutput: return STRING;
		case PlaybackOutput:   return INT;
		case MuteSoundboard:   return INT;
		case PlayLastSound:    return INT;
		case MidiPortName:     return STRING;
		case MidiPort:         return INT;
		case LENGTH:           break;
		}
		throw "Config key not found";
	}

	static inline constexpr std::string keyToString(ConfigKey key)
	{
		switch (key) {
		case MicrophoneOutput: return "MicrophoneOutput";
		case MuteSoundboard:   return "MuteSoundboard";
		case PlaybackOutput:   return "PlaybackOutput";
		case PlayLastSound:    return "PlayLastSound";
		case MidiPortName:     return "MidiPortName";
		case MidiPort:         return "MidiPort";
		case LENGTH:           break;
		}
		throw "Config key not found";
	}

	static inline constexpr ConfigKey keyFromString(std::string key)
	{
		if (key == "MicrophoneOutput") { return ConfigKey::MicrophoneOutput; }
		if (key == "PlaybackOutput") { return ConfigKey::PlaybackOutput; }
		if (key == "MuteSoundboard") { return ConfigKey::MuteSoundboard; }
		if (key == "MidiPortName") { return ConfigKey::MidiPortName; }
		if (key == "PlayLastSound") { return ConfigKey::PlayLastSound; }
		if (key == "MidiPort") { return ConfigKey::MidiPort; }
		throw "Invalid config key " + key;
	}

	static Config* getInstance();

	[[nodiscard]] ConfigValue operator[](ConfigKey k) { return getValue(k); }
	ConfigValue               getValue(ConfigKey key);

	void serialize();
	void deserialize();
};

// int getOutputDevices();

int changeConfig(std::string ValueToChange, std::string NewValue);

int initializeApplication();

void bindSoundToKey(int KeyCode, std::string SoundName);

void importSound();

int addMidimapping(int Key, std::string InMapping);

int loadMidimap();
