#include "ObjectManager.h"

bool ObjectManager::m_alreadyCreated = false;

void ObjectManager::Initialise(std::vector<DisplayObject>* p_SceneObjects, const std::shared_ptr<DX::DeviceResources>& p_deviceResources, const InputCommands* p_InputCommands)
{
	m_SceneObjects = p_SceneObjects;
	m_deviceResource = p_deviceResources;
	m_InputCommands = p_InputCommands;

	// enforce ID consistency
	for (int i = 0; i < m_SceneObjects->size(); ++i)
	{
		(*m_SceneObjects)[i].m_ID = i;
	}
}

void ObjectManager::HandleInput()
{

	if (m_InputCommands->selectAll) {
		SelectAll();
	}

}

void ObjectManager::Update()
{

}

DisplayObject ObjectManager::GetObject(int p_id)
{
	for (int i = 0; i < m_SceneObjects->size(); ++i)
	{
		if (i == p_id)
		{
			return (*m_SceneObjects)[i];
		}
	}
	return {};
}

void ObjectManager::SelectOrDeselectObject(int p_ID)
{

	// not multi-selecting or we've clicked on no object
	if(!m_InputCommands->control || p_ID < 0){

		for (auto& object : m_selectedObjects) {

			ChangeTexture(object, DeselectedTexture);

		}
		
		m_selectedObjects.clear();

	}

	// check for de-selection 
	for (auto& object : m_selectedObjects) {

		// if clicked object exists in selected object vector, change it's texture and remove
		if (object == p_ID) {
			ChangeTexture(p_ID, DeselectedTexture);
			m_selectedObjects.erase(std::find(m_selectedObjects.begin(), m_selectedObjects.end(), p_ID));
			return;
		}

	}


	// add selected object to vector and change it's texture
	if (p_ID >= 0) {
		m_selectedObjects.push_back(p_ID);
		ChangeTexture(p_ID, SelectedTexture);
	}
}

//Select all objects in the scene
void ObjectManager::SelectAll()
{
	//Clear the vector as we're about to select everything anyway
	m_selectedObjects.clear();
	//Select everything
	for (int i = 0; i < m_SceneObjects->size(); ++i)
	{
		m_selectedObjects.push_back(i);
		ChangeTexture(i, SelectedTexture);
	}
}

ObjectManager::ObjectManager()
{
}

void ObjectManager::ChangeTexture(int p_ID, const wchar_t* p_TextureFilePath) {

	for (int i = 0; i < m_SceneObjects->size(); i++) {

		if ((*m_SceneObjects)[i].m_ID != p_ID) {
			continue;
		}
		
		//Then set the "error" texture to act as a indicator
		DirectX::CreateDDSTextureFromFile(m_deviceResource->GetD3DDevice(), p_TextureFilePath,
			nullptr, &(*m_SceneObjects)[i].m_texture_diffuse);
		(*m_SceneObjects)[i].m_model->UpdateEffects([&](DirectX::IEffect* effect)
			{
				const auto basic_effect = dynamic_cast<DirectX::BasicEffect*>(effect);
				if (basic_effect)
				{
					basic_effect->SetTexture((*m_SceneObjects)[i].m_texture_diffuse);
				}

			});
	}
}