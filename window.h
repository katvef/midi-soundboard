#include "gui-frame.hpp"
#include "midi-handler.h"
#include "audioloader.h"
#include "soundplayer.h"
#include "intermediary.h"

#include <iostream>
#include <string>
#include <vector>

extern bool Done;
extern std::map<std::string, int> SoundBindings;


class SoundboardGui : public Gui
{
	public:
		SoundboardGui() = default;
		~SoundboardGui() = default;


		virtual void Start() final;

		virtual void Update() final;

	private:
		bool ImportingSound;
		// std::vector<const char*> MidiPorts;
		// std::vector<const char*> MidiPorts = getMidiPortNames();
		std::vector<std::string> PossibleOutputs = getOutputDevices();
		std::vector<std::string> LoadedSoundEffects = getSoundNames();
};
