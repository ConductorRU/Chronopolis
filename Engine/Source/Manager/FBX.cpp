#include "stdafx.h"
#include "FBX.h"
#include "Manager.h"
#include "../Core/Engine.h"
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
				FbxNode *child = lNode->GetChild(i);
				FbxMesh *mesh = child->GetMesh();
				if(mesh != nullptr)
				{
					Mesh *res = ParseMesh(mesh);
					return res;
				}
			}
		}
		lImporter->Destroy();
		return nullptr;
	}
	Mesh *FBX::ParseMesh(FbxMesh *mesh)
	{
		Vector *pos, *nor;
		Color *col;
		if(mesh == nullptr)
			return nullptr;
		Mesh *res = Engine::Get()->GetManager()->CreateMesh(Engine::Get()->GetManager()->GetInputLayout("default"));
		int numVerts = mesh->GetControlPointsCount();
		int numPoly = mesh->GetPolygonCount();
		FbxVector4* points = mesh->GetControlPoints();
		int vertexId = 0;
		FbxVector4 fVector;
		FbxColor fColor;
		vector<int> tIndex;
		for(int i = 0 ; i < numPoly; ++i)
		{
			int polySize = mesh->GetPolygonSize(i);
			tIndex.clear();
			int vStart = vertexId;
			for(int n = 0; n < polySize; ++n)
			{
				int index = mesh->GetPolygonVertex(i, n);
				tIndex.push_back(index);
				char *v = res->CreateVertex();
				pos = (Vector*)v;
				col = (Color*)&(v[12]);
				nor = (Vector*)&(v[28]);
				*pos = Vector((float)points[index].mData[0], (float)points[index].mData[1], (float)points[index].mData[2]);

				for (int l = 0; l < mesh->GetElementVertexColorCount(); ++l)
				{
					FbxGeometryElementVertexColor* leVtxc = mesh->GetElementVertexColor(l);

					switch (leVtxc->GetMappingMode())
					{
					default:
							break;
					case FbxGeometryElement::eByControlPoint:
						switch (leVtxc->GetReferenceMode())
						{
						case FbxGeometryElement::eDirect:
							fColor = leVtxc->GetDirectArray().GetAt(index);
							break;
						case FbxGeometryElement::eIndexToDirect:
							{
								int id = leVtxc->GetIndexArray().GetAt(index);
								fColor = leVtxc->GetDirectArray().GetAt(id);
							}
							break;
						default:
							break; // other reference modes not shown here!
						}
						break;

					case FbxGeometryElement::eByPolygonVertex:
						{
							switch (leVtxc->GetReferenceMode())
							{
							case FbxGeometryElement::eDirect:
								fColor = leVtxc->GetDirectArray().GetAt(vertexId);
								break;
							case FbxGeometryElement::eIndexToDirect:
								{
									int id = leVtxc->GetIndexArray().GetAt(vertexId);
									fColor = leVtxc->GetDirectArray().GetAt(id);
								}
								break;
							default:
								break; // other reference modes not shown here!
							}
						}
						break;

					case FbxGeometryElement::eByPolygon: // doesn't make much sense for UVs
					case FbxGeometryElement::eAllSame:   // doesn't make much sense for UVs
					case FbxGeometryElement::eNone:       // doesn't make much sense for UVs
						break;
					}
				}
				*col = Color((float)fColor.mRed, (float)fColor.mGreen, (float)fColor.mBlue, (float)fColor.mAlpha);
				int eNormalCount = mesh->GetElementNormalCount();
				for(int l = 0; l < eNormalCount; ++l)
				{
					FbxGeometryElementNormal* leNormal = mesh->GetElementNormal(l);
					if(leNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
					{
						switch (leNormal->GetReferenceMode())
						{
						case FbxGeometryElement::eDirect:
							fVector = leNormal->GetDirectArray().GetAt(vertexId);
							*nor = Vector(fVector.mData[0], fVector.mData[1], fVector.mData[2]);
							break;
						case FbxGeometryElement::eIndexToDirect:
							{
								int id = leNormal->GetIndexArray().GetAt(vertexId);
								fVector = leNormal->GetDirectArray().GetAt(id);
								*nor = Vector(fVector.mData[0], fVector.mData[1], fVector.mData[2]);
							}
							break;
						default:
							break; // other reference modes not shown here!
						}
					}

				}
				++vertexId;
			}
			for(int in = 1; in < tIndex.size() - 1; ++in)
				res->AddTriangle(vStart, vStart + in, vStart + in + 1);
		}
		res->Bake();
		return res;
	}
};