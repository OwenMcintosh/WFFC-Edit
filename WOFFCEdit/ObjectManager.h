#pragma once
#include <vector>
#include "DisplayChunk.h"
#include "DisplayObject.h"
#include "InputCommands.h"

struct InputCommands;

#define SelectedTexture L"database/data/error.dds"
#define DeselectedTexture L"database/data/placeholder.dds"


class ObjectManager
{
public:
	static ObjectManager& Instance()
	{
		static ObjectManager instance; // Guaranteed to be destroyed.
		m_alreadyCreated = true;
		return instance;
	}

	void Initialise(std::vector<DisplayObject>* p_startingObjects, const std::shared_ptr<DX::DeviceResources>& p_deviceResources, const InputCommands* p_InputCommands);
	void HandleInput();
	void Update();
	void SelectAll();
	std::vector<DisplayObject>* GetSceneObjects() { return m_SceneObjects; }
	static bool hasInstanceBeenCreated() { return m_alreadyCreated; }
	DisplayObject GetObject(int p_ID);
	std::vector<int>* getSelectedList() { return &m_selectedObjects; }
	void SelectOrDeselectObject(int p_ID);

protected:
	ObjectManager();

private:
	const InputCommands* m_InputCommands;
	std::vector<int> m_selectedObjects;
	std::vector<DisplayObject>* m_SceneObjects;
	static bool m_alreadyCreated;
	std::shared_ptr<DX::DeviceResources> m_deviceResource;

	void ChangeTexture(int p_ID, const wchar_t* p_TextureFilePath);
};

