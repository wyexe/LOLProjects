#include "stdafx.h"
#include "VarBase.h"

vector<ServerAccountInfo> AccountList;
CLLock AccountListLock;
HANDLE hSocketThread = NULL;