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

class Config
{
 public:
	enum ConfigTypes { STRING, INT };
	enum ConfigKeys { MicrophoneOutput, MuteSoundboard, PlayLastSound, PlaybackOutput, MidiPortName, MidiPort, LENGTH };
	typedef std::variant<std::string, int> ConfigValue;

 private:
	std::vector<std::string> config_keys;

	inline std::string getConfigKey(const std::string& line);

	inline std::string getConfigValue(const std::string& line);

	inline bool isCommentOrEmpty(const std::string_view& line);

	void deserialize();

 public:
	static inline constexpr int keyGetType(ConfigKeys key)
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
		throw "Key not found";
	}

	static inline constexpr std::string keyToString(ConfigKeys key)
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
		throw "Key not found";
	}

	static inline constexpr ConfigKeys keyFromString(std::string key)
	{
		if (key == "MicrophoneOutput") { return ConfigKeys::MicrophoneOutput; }
		if (key == "PlaybackOutput") { return ConfigKeys::PlaybackOutput; }
		if (key == "MuteSoundboard") { return ConfigKeys::MuteSoundboard; }
		if (key == "MidiPortName") { return ConfigKeys::MidiPortName; }
		if (key == "PlayLastSound") { return ConfigKeys::PlayLastSound; }
		if (key == "MidiPort") { return ConfigKeys::MidiPort; }
		throw "Invalid config key " + key;
	}

	void serialize();

	ConfigValue getValue(ConfigKeys key);

	Config();
};
// int getOutputDevices();

int changeConfig(std::string ValueToChange, std::string NewValue);

int initializeApplication();

void bindSoundToKey(int KeyCode, std::string SoundName);

void importSound();

int addMidimapping(int Key, std::string InMapping);

int loadMidimap();
