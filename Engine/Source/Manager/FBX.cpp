#include "stdafx.h"
#include "FBX.h"
#include "../Mesh/Mesh.h"
namespace DEN
{
	Mesh *FBX::Import(const string &filename)
	{
		FbxManager* lSdkManager = FbxManager::Create();
		FbxIOSettings * ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
		lSdkManager->SetIOSettings(ios);

		FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "");
		bool lImportStatus = lImporter->Initialize(filename.c_str(), -1, lSdkManager->GetIOSettings());
		if(!lImportStatus)
		{
      printf("Call to FbxImporter::Initialize() failed.\n");
      printf("Error returned: %s\n\n", lImporter->GetStatus().GetErrorString());
      return nullptr;
    }
		FbxScene* lScene = FbxScene::Create(lSdkManager,"myScene");
		lImporter->Import(lScene);
		int lFileMajor, lFileMinor, lFileRevision;
		lImporter->GetFileVersion(lFileMajor, lFileMinor, lFileRevision);
		int i;
    FbxNode* lNode = lScene->GetRootNode();
		if(lNode)
		{
			for(i = 0; i < lNode->GetChildCount(); i++)
			{
            
			}
		}
		lImporter->Destroy();

	}
};