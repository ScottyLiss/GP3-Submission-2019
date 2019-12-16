#include "SceneManagement.h"
#include <cstring>
#include <iostream>
#include <comutil.h>


/*
CString SceneManagement::GetAttributeValue(IXMLDOMDocument* pDoc, CString strElementPath, CString strAttribute)
{
	
	IXMLDOMNode* pNode = NULL;
	IXMLDOMNamedNodeMap* pAttributes = NULL;
	BSTR bstrPath = strElementPath.AllocSysString();
	BSTR bstrAttribute = strAttribute.AllocSysString();

	try
	{
		HRESULT hr;

		hr = pDoc->selectSingleNode(bstrPath, &pNode);
		if (hr != S_OK)
			throw hr;

		hr = pNode->get_attributes(&pAttributes);
		if (hr != S_OK)
			throw hr;

		hr = pAttributes->getNamedItem(bstrAttribute, &pNode);
		if (hr != S_OK)
			throw hr;

		BSTR bstrValue;
		pNode->Release();
		hr = pNode->get_text(&bstrValue);
		if (hr != S_OK)
			throw hr;

		pNode->Release();
		pAttributes->Release();

		return (LPCTSTR)_bstr_t(bstrValue, false);

	}
	catch (HRESULT hr)
	{
		SysFreeString(bstrPath);
		SysFreeString(bstrAttribute);
		if (pNode)
			pNode->Release();
		if (pAttributes)
			pAttributes->Release();

		return _T("");
	}
}
*/