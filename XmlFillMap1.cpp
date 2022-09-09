

#include <iostream>
#include <comutil.h>
#include <atlbase.h>
#include <map>
#include <string>
#include <sstream>
#import <msxml6.dll>

using namespace MSXML2;
using namespace std;

struct Param
{
	CComBSTR type;
	CComBSTR name;
	CComBSTR interpolator;
	CComBSTR numkey;
	CComBSTR text;


	Param()
	{
	};
	~Param()
	{

	};

};
struct Object
{
	CComBSTR clsid;
	CComBSTR name;
	CComBSTR type;
	CComBSTR id;
	map <CComBSTR, Param*> mParam;
	Object()
	{

	};
	~Object()
	{

	};
};

void FillParam(MSXML2::IXMLDOMElementPtr& docElement, map<CComBSTR, Param*>& map)
{
	MSXML2::IXMLDOMDocument2Ptr docPtr;
	MSXML2::IXMLDOMNodePtr nodeParams;
	CComBSTR val;
	stringstream s;

	CComBSTR bstrValue = "params";
	docElement->raw_selectSingleNode(CComBSTR(L"params").m_str, &nodeParams);
	if (docElement)
	{
		docElement = nodeParams;

		MSXML2::IXMLDOMNodeListPtr ParamList = nodeParams->selectNodes(L"param");
		if (ParamList)
		{
			//Iterating list
			for (int cntr = 0; cntr < ParamList->length; cntr++)
			{
				MSXML2::IXMLDOMElementPtr elementParam = ParamList->item[cntr];
				if (elementParam)
				{
					Param* Node = new Param();
					if (Node)
					{
						bstrValue.Empty();
						CComVariant varValue;
						bstrValue = L"param";

						HRESULT hr = elementParam->raw_getAttribute(bstrValue.m_str, &varValue);
						if (hr == S_OK && varValue.bstrVal)
							bstrValue = varValue.bstrVal;

						hr = elementParam->raw_getAttribute(CComBSTR(L"name"), &varValue);
						if (hr == S_OK && varValue.bstrVal)
							Node->name = varValue.bstrVal;

						bstrValue.Empty();
						bstrValue = elementParam->Gettext().operator char* ();
						Node->text = bstrValue;

						bstrValue.Empty();
						hr = elementParam->raw_getAttribute(CComBSTR(L"type").m_str, &varValue);
						if (hr == S_OK && varValue.bstrVal)
							Node->type = varValue.bstrVal;

						bstrValue.Empty();
						hr = elementParam->raw_getAttribute(CComBSTR(L"interpolator").m_str, &varValue);
						if (hr == S_OK && varValue.bstrVal)
							Node->interpolator = varValue.bstrVal;

						bstrValue.Empty();
						hr = elementParam->raw_getAttribute(CComBSTR(L"numkey").m_str, &varValue);
						if (hr == S_OK && varValue.bstrVal)
							Node->numkey = varValue.bstrVal;

						bstrValue.Empty();
						varValue.Clear();

						s << cntr;
						bstrValue = s.str().c_str();
						//Inserting values in map
						map.insert(make_pair(bstrValue, Node));
					}
				}
			}
			cout << "Size of map: " << map.size();
			cout << endl;
		}
	}
}


void FillObject(MSXML2::IXMLDOMElementPtr& docElement, Object* Objectptr)
{
	MSXML2::IXMLDOMNodePtr objectObj;

	CComBSTR bstrValue = "object";
	CComVariant varValue;
	docElement->raw_selectSingleNode(CComBSTR(L"object"), &objectObj);
	if (objectObj)
	{
		docElement = objectObj;
		if (Objectptr)
		{
			bstrValue.Empty();
			HRESULT hr = docElement->raw_getAttribute(CComBSTR("clsid"), &varValue);
			Objectptr->clsid = varValue.bstrVal;

			bstrValue.Empty();
			hr = docElement->raw_getAttribute(CComBSTR("name"), &varValue);
			Objectptr->name = varValue.bstrVal;

			bstrValue.Empty();
			hr = docElement->raw_getAttribute(CComBSTR("type"), &varValue);
			Objectptr->type = varValue.bstrVal;

			bstrValue.Empty();
			hr = docElement->raw_getAttribute(CComBSTR("id"), &varValue);
			Objectptr->id = varValue.bstrVal;

		}
	}
	FillParam(docElement, Objectptr->mParam);
}

void PrintValues(Object* inPtr)
{
	if (inPtr)
	{
		wcout << "\nObject clsid : " << (inPtr->clsid).m_str;
		wcout << "\nObject Name : " << (inPtr->name).m_str;
		wcout << "\nObject Type : " << (inPtr->type).m_str;
		wcout << "\nObject id : " << (inPtr->id).m_str;

		map<CComBSTR, Param*>::iterator iter;
		for (iter = (inPtr->mParam).begin(); iter != (inPtr->mParam.end()); iter++)
		{
			cout << endl;
			if (iter->second)
			{
				wcout << "\nParam name: " << ((*iter).second->name).m_str;
				wcout << "\nParam type: " << ((*iter).second->type).m_str;
				wcout << "\nParam text: " << ((*iter).second->text).m_str;
				wcout << "\nParam interpolator: " << ((*iter).second->interpolator).m_str;
				wcout << "\nParam numkey: " << ((*iter).second->numkey).m_str;
			}
		}
	}
	else
	{
		cout << "\nPointer is null";
	}
}

int ReadXml(Object* ObjectPtr)
{
	//Initializing COM 
	::CoInitialize(NULL);

	//Creating DOMDocument
	MSXML2::IXMLDOMDocument2Ptr docPtr;

	HRESULT hr = docPtr.CreateInstance(__uuidof(MSXML2::DOMDocument60));
	if (hr != S_OK || !docPtr)
	{
		return 0;
	}

	//Setting Path of XML file
	CComVariant bstrXmlPath = L"d:\\test.xml";
	CComVariant varValue;
	CComBSTR bstrValue;

	//Loading XML
	VARIANT_BOOL vbRet = VARIANT_FALSE;
	docPtr->raw_load(bstrXmlPath, &vbRet);
	if (vbRet == VARIANT_TRUE)
	{
		MSXML2::IXMLDOMElementPtr docElement;
		docPtr->get_documentElement(&docElement);

		if (ObjectPtr)
		{
			if (docElement)
			{
				FillObject(docElement, ObjectPtr);
			}
			PrintValues(ObjectPtr);
		}
	}
	bstrXmlPath.Clear();
	return 0;
}

int main()
{
	Object* ObjObject = new Object();

	if (ObjObject)
	{
		ReadXml(ObjObject);
	}

	if (ObjObject)
	{
		delete ObjObject;
	}
	ObjObject = NULL;
	system("pause");
	return 0;
}