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


		virtual void Start() final
		{
		}

		virtual void Update() final
		{
	// ===================================================================================== //
	// =============================== Sound Effects Window ================================ //
	// ===================================================================================== //

		ImVec2 ViewportSize = ImGui::GetMainViewport()->WorkSize;
		ImVec2 LogBoxSize = ImVec2(0, 150);
		float ContentRelation = 0.2f;

		ImGuiWindowFlags ToolWindowFlags = 0;
		ToolWindowFlags |= ImGuiWindowFlags_MenuBar;
		ToolWindowFlags |= ImGuiWindowFlags_NoTitleBar;
		ToolWindowFlags |= ImGuiWindowFlags_NoResize;
		ToolWindowFlags |= ImGuiWindowFlags_NoCollapse;
		ToolWindowFlags |= ImGuiWindowFlags_AlwaysAutoResize;

		ImGui::SetNextWindowPos(ImVec2(ViewportSize.x * ContentRelation, 0));
		ImGui::SetNextWindowSize(ImVec2(ViewportSize.x * (1.0f - ContentRelation), ViewportSize.y - LogBoxSize.y));

        {
            ImGui::Begin("Sound Effects Window", NULL, ToolWindowFlags);

			ImGui::BeginMenuBar();
			if (ImGui::BeginMenu("Sounds"))
			{
				if (ImGui::MenuItem("Import"))
				{
					// Import Sound
					importSound();
					LoadedSoundEffects = getSoundNames();
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Devices"))
			{
				if (ImGui::BeginMenu("Midi Port"))
				{
					std::vector<const char*> MidiPorts = getMidiPortNames();
					for (int i = 0; i < MidiPorts.size(); i++)
					{
						if (ImGui::MenuItem(MidiPorts[i]))
						{
							std::cout << "New Port: " << MidiPorts[i] << '\n';
							changeConfig("MidiPort", MidiPorts[i]);
						}
					}
					ImGui::EndMenu();
				}
				// PossibleOutputs = getOutputDevices();
				if (ImGui::BeginMenu("Microphone"))
				{
					for (int i = 0; i < PossibleOutputs.size(); i++)
					{
						std::string MicOutName = PossibleOutputs[i] + "##Mic";
						if (ImGui::MenuItem(MicOutName.c_str()))
						{
							std::cout << "New Microphone Output: " << PossibleOutputs[i].c_str() << '\n';
							changeConfig("MicrophoneOutput", PossibleOutputs[i].c_str());
						}
					}
					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Playback"))
				{
					for (int i = 0; i < PossibleOutputs.size(); i++)
					{
						std::string PlaybackName = PossibleOutputs[i] + "##Playback";
						if (ImGui::MenuItem(PlaybackName.c_str()))
						{
							std::cout << "New Playback Device: " << PossibleOutputs[i].c_str() << '\n';

							changeConfig("PlaybackOutput", PossibleOutputs[i].c_str());
						}
					}
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();

			ImGui::SetNextWindowPos(ImVec2(ViewportSize.x * 0.5f, ViewportSize.y * 0.5f));
			ImGui::SetNextWindowSize(ImVec2(200, 100));

			// if (ImGui::TreeNode("Test Tree"))
			// {
			// 	ImGui::SeparatorText("Text Number 1");
			// 	ImGui::SeparatorText("Text Number 2");
			// 	ImGui::SeparatorText("Text Number 3");
			//
			// 	ImGui::TreePop();
			// }

			ImGui::BeginTable("Test Table", 1);

			// List of Loaded sound effects.
			ImGui::TableNextColumn();

			ImU32 HeaderColor = ImGui::GetColorU32(ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
			ImU32 ButtonColor = ImGui::GetColorU32(ImVec4(0.6f, 0.3f, 0.1f, 1.0f));
			ImU32 ButtonColorHover = ImGui::GetColorU32(ImVec4(0.4f, 0.2f, 0.0f, 1.0f));
			ImU32 ButtonColorActive = ImGui::GetColorU32(ImVec4(0.2f, 0.1f, 0.0f, 1.0f));

			// Set Color of the following elements.
			ImGui::PushStyleColor(ImGuiCol_Button, ButtonColor);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ButtonColorHover);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ButtonColorActive);
			ImGui::PushStyleColor(ImGuiCol_Header, HeaderColor);
			ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ButtonColorHover);
			ImGui::PushStyleColor(ImGuiCol_HeaderActive, ButtonColorActive);

			ImGui::Text("Sound Effects");

			ImVec2 CursorPosition = ImVec2(15, 55);
			int SoundAmount = LoadedSoundEffects.size();
			for (int i = 0; i < SoundAmount; i++)
			{
				ImGui::SetCursorPos(CursorPosition);
				if (ImGui::CollapsingHeader(LoadedSoundEffects[i].c_str()))
				{
					// On Sound effect Clicked
					CursorPosition.y += 25;
					CursorPosition.x += 20;
					ImGui::SetCursorPos(CursorPosition);

					std::string ButtonName;
					std::string MidiKey;
					bool SoundIsBound = SoundBindings.contains(LoadedSoundEffects[i]);
					if (SoundIsBound)
						MidiKey = std::to_string(SoundBindings.at(LoadedSoundEffects[i]));
					ButtonName = (SoundIsBound) ? "MIDI: [" + MidiKey + "]##" : "[Not Bound]##";
					ButtonName += std::to_string(i);

					if (ImGui::Button(ButtonName.c_str(), ImVec2(90, 50)))
					{
						// Bind key to sound effect
						int NewKey = awaitInput();
						bindSoundToKey(NewKey, LoadedSoundEffects[i]);
					}

					CursorPosition.x += 100;
					CursorPosition.y += 30;
					ImGui::SetCursorPos(CursorPosition);

					std::string SoundOption1;
					SoundOption1 = "Remove##";
					SoundOption1 += std::to_string(i);
					if (ImGui::Button(SoundOption1.c_str(), ImVec2(80, 20)))
					{

					}
					CursorPosition.x -= 120;
					CursorPosition.y += 20;

				}

				CursorPosition.y += 20;
				// CursorPosition.x -= 20;
			}

			// Apply changes to prior elements.
			ImGui::PopStyleColor(); ImGui::PopStyleColor();
			ImGui::PopStyleColor(); ImGui::PopStyleColor();
			ImGui::PopStyleColor(); ImGui::PopStyleColor();

			ImGui::EndTable();
            ImGui::End();
        }

		// ===================================================================================== //
		// ============================== Browser Content Window =============================== //
		// ===================================================================================== //
			
			ImGuiWindowFlags MainWindowFlags = 0;
			MainWindowFlags |= ImGuiWindowFlags_MenuBar;
			MainWindowFlags |= ImGuiWindowFlags_NoTitleBar;
			MainWindowFlags |= ImGuiWindowFlags_NoResize;
			MainWindowFlags |= ImGuiWindowFlags_NoCollapse;
			MainWindowFlags |= ImGuiWindowFlags_AlwaysAutoResize;

			ImGui::SetNextWindowPos(ImVec2(0, 0));
			ImGui::SetNextWindowSize(ImVec2(ViewportSize.x * ContentRelation, ViewportSize.y));

			{
				ImGui::Begin("Main Window", NULL, MainWindowFlags);

				ImGui::BeginMenuBar();
				if (ImGui::BeginMenu("Sound Libraries"))
				{
					if (ImGui::MenuItem("Add"))
					{
						// Add reference directory
					}

					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();

				ImGui::End();
			}

		// ===================================================================================== //
		// ================================== Log Window ======================================= //
		// ===================================================================================== //

			ImGuiWindowFlags LogWindowFlags = 0;
			LogWindowFlags |= ImGuiWindowFlags_NoTitleBar;
			LogWindowFlags |= ImGuiWindowFlags_NoResize;
			LogWindowFlags |= ImGuiWindowFlags_NoCollapse;
			LogWindowFlags |= ImGuiWindowFlags_AlwaysAutoResize;
			LogWindowFlags |= ImGuiWindowFlags_AlwaysUseWindowPadding;

			ImGui::SetNextWindowPos(ImVec2(ViewportSize.x * ContentRelation, ViewportSize.y - LogBoxSize.y));
			ImGui::SetNextWindowSize(ImVec2(ViewportSize.x * (1.0f - ContentRelation), LogBoxSize.y));

			{
				ImGui::Begin("Log Window", NULL, LogWindowFlags);

				// ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

				ImGui::Text("Available Region %.3f", ImGui::GetMainViewport()->WorkSize.y);
				ImGui::End();
			}
		}

	private:
		bool ImportingSound;
		std::vector<std::string> PossibleOutputs = getOutputDevices();
		std::vector<std::string> LoadedSoundEffects = getSoundNames();
};
