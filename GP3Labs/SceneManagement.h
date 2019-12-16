#pragma once
#include "pch.h"
#include <msxml.h> // The use of XML files, Microsoft XML 
#include <atlstr.h> // Enables the use of CString

class SceneManagement
{
public:
	CString GetAttributeValue(IXMLDOMDocument* pDoc, CString strElementPath, CString strAttribute);
};

