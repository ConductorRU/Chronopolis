#include "DC.h"
#include "GUIPlan.h"
GUIPlan::GUIPlan(GUI *gui)
{
	_root = gui->GetRoot()->CreateChild();
	_root->SetStyle("width:100px;height:100px;background-color:#aaa;border-radius:50px");
}
GUIPlan::~GUIPlan()
{

}