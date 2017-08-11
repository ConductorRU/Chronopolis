#include "stdafx.h"
#include "Compiler\Compiler.h"
namespace Lib
{
void InitFunc(Compiler *comp)
{
comp->AddFunc("int int::operator+(int)");
}
}
