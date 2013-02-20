#include <locale>
#include <codecvt>
#include <iostream>
#include <string>

#include "ParsedPrimitiveHelper.h"
#include "PDFRectangle.h"
#include "PDFReal.h"
#include "PDFInteger.h"
#include "PDFTextString.h"
#include "UnicodeString.h"
#include "XObjectContentContext.h"
#include "Trace.h"
#include "PDFWriter.h"
#include "PDFPage.h"
#include "PDFFormXObject.h"
#include "PageContentContext.h"
#include "PDFImageXObject.h"
#include "PDFParser.h"
#include "PDFDictionary.h"
#include "PDFObjectCast.h"
#include "PDFObject.h"
#include "PDFIndirectObjectReference.h"
#include "PDFStreamInput.h"
#include "PDFArray.h"
#include "PDFLiteralString.h"
#include "PDFDocumentCopyingContext.h"
#include "ObjectsContext.h"
#include "PDFStream.h"
#include "DictionaryContext.h"
#include "OutputStreamTraits.h"

#include <iostream>

using namespace PDFHummus;
//void copyResources(PDFObject* inObject, PDFParser* inParser, PDFFormXObject* xObject);

void getRect(PDFWriter* inPDFWriter, ObjectIDType modifiedAcroFormID, double& left, double& bottom, double& width, double& height)
{
	PDFDocumentCopyingContext* copyingContext = inPDFWriter->CreatePDFCopyingContextForModifiedFile();
	PDFObjectCastPtr<PDFDictionary> AcroFormFieldObject(copyingContext->GetSourceDocumentParser()->ParseNewObject(modifiedAcroFormID));	
	MapIterator<PDFNameToPDFObjectMap>  objectContentIterator = AcroFormFieldObject->GetIterator();	
	while(objectContentIterator.MoveNext())
	{
		if(objectContentIterator.GetKey()->GetValue() == "Rect")
						{
							{	
								PDFObjectCastPtr<PDFArray> Rect;
								Rect =(objectContentIterator.GetValue());
								SingleValueContainerIterator<PDFObjectVector> rectContentIterator= Rect->GetIterator();
									if(rectContentIterator.MoveNext())
										
									left = ParsedPrimitiveHelper(rectContentIterator.GetItem()).GetAsDouble(); 
								if(rectContentIterator.MoveNext())
								{	
									bottom = ParsedPrimitiveHelper(rectContentIterator.GetItem()).GetAsDouble(); 
								}
								if(rectContentIterator.MoveNext())
								{
									width = ParsedPrimitiveHelper(rectContentIterator.GetItem()).GetAsDouble(); 
								}
								if(rectContentIterator.MoveNext())
								{
									height = ParsedPrimitiveHelper(rectContentIterator.GetItem()).GetAsDouble(); 
								}
							}

						}
	}
}

void testingFuntion(PDFWriter* inPDFWriter, PDFFormXObject*& xObject)
{
	

	
	PDFDocumentCopyingContext* copyingContext = inPDFWriter->CreatePDFCopyingContextForModifiedFile();
	ObjectIDType modifiedAcroFormID; 
	EStatusCode status = copyingContext->GetSourceDocumentParser()->FindAcroFormID("Tekst2", modifiedAcroFormID);
	//EStatusCode status = copyingContext->GetSourceDocumentParser()->FindAcroFormID("text_1", modifiedAcroFormID);

	double l, b, w ,h;
	getRect(inPDFWriter, modifiedAcroFormID, l, b, w, h);
	xObject = inPDFWriter->GetDocumentContext().StartFormXObject(PDFRectangle(l, b, w, h));
	XObjectContentContext* xobjectContentContext = xObject->GetContentContext();
	PDFObjectCastPtr<PDFDictionary> AcroFormFieldObject(copyingContext->GetSourceDocumentParser()->ParseNewObject(modifiedAcroFormID));	
	MapIterator<PDFNameToPDFObjectMap>  objectContentIterator = AcroFormFieldObject->GetIterator();	
		{
			inPDFWriter->GetObjectsContext().StartModifiedIndirectObject(modifiedAcroFormID);
			
			{
				DictionaryContext* modifiedPageObject = inPDFWriter->GetObjectsContext().StartDictionary();
				{
					while(objectContentIterator.MoveNext())
					{
						if(objectContentIterator.GetKey()->GetValue() == "FT")
						{
							//xobjectContentContext->DA("asdf");
							//modifiedPageObject->WriteKey("FT");
							//modifiedPageObject->write
						}
						if(objectContentIterator.GetKey()->GetValue() == "DR")
						{
							{
								
								PDFObjectCastPtr<PDFLiteralString> DefaultAppearance;
								DefaultAppearance =(objectContentIterator.GetValue());
								
								xobjectContentContext->DA(DefaultAppearance->GetValue());
							}
						}
						
						if(objectContentIterator.GetKey()->GetValue() == "V")
						{
							modifiedPageObject->WriteKey("V");
							modifiedPageObject->WriteLiteralStringValue("asdf");
						}
						if (objectContentIterator.GetKey()->GetValue() == "AP")
						{
							modifiedPageObject->WriteKey("AP");
							{
								DictionaryContext* APDictionary = inPDFWriter->GetObjectsContext().StartDictionary();
								APDictionary->WriteKey("N");
								APDictionary->WriteNewObjectReferenceValue(xObject->GetObjectID());
				
								inPDFWriter->GetObjectsContext().EndDictionary(APDictionary);
							}
						}
						// just copy other keys
						if(objectContentIterator.GetKey()->GetValue() != "V" && 
						objectContentIterator.GetKey()->GetValue() != "AP")
					{
				

						modifiedPageObject->WriteKey(objectContentIterator.GetKey()->GetValue());
						copyingContext->CopyDirectObjectAsIs(objectContentIterator.GetValue());
					}
				}//while
				inPDFWriter->GetObjectsContext().EndDictionary(modifiedPageObject);		
				}
			
			inPDFWriter->GetObjectsContext().EndIndirectObject();
			}
	}		
}

int main()

{ 
	const std::string materials_dir = MATERIALS_DIR;
	//const std::string fileName = "text_fields.pdf";
	const std::string fileName = "A1 PL.pdf";
	//const std::string fileName = "text_fields.pdf";

	const std::string testFileName = materials_dir  + fileName;
	
	PDFWriter pdfWriter;
	EStatusCode status = eSuccess;
	std::string modFileName = materials_dir +  std::string("mod") + fileName;
	status = pdfWriter.ModifyPDF(testFileName, ePDFVersion17, modFileName);
	PDFParser* inParser;
	inParser =&pdfWriter.GetModifiedFileParser();
	 PDFDocumentCopyingContext* copyingContext = pdfWriter.CreatePDFCopyingContextForModifiedFile();
	
	 pdfWriter.GetObjectsContext().SetCompressStreams(false);
	{

		PDFFormXObject* formXObject;
		testingFuntion(&pdfWriter, formXObject);
	
		const std::string font_file = FONT_fILE;
		PDFUsedFont* font = pdfWriter.GetFontForFile(font_file);
		if(!font)
		{
			status = PDFHummus::eFailure;
		}
		
		XObjectContentContext* xobjectContentContext = formXObject->GetContentContext();
		xobjectContentContext->BMC();
		xobjectContentContext->q();
		xobjectContentContext->W();
		xobjectContentContext->n();
		xobjectContentContext->g(0);
		xobjectContentContext->BT();
		xobjectContentContext->Tf(font,10);
		xobjectContentContext->Td(2, 3.165);

			xobjectContentContext->Tm(0,0, 0, 1, 100, 100);
		//xobjectContentContext->Tm(0,0,7.1,7.2);
		//xobjectContentContext->Tm(20,0,0,20,40,822);
		//xobjectContentContext->Tm(30,0,0,30,78.4252,662.8997);
		//xobjectContentContext->Td(124.885, 4.7014);
			//xobjectContentContext->Td(124.885, 4.7014);
		

		wchar_t* asdf  = L"editedbéęą";
		std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
		conv.to_bytes(asdf);
		//std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> utf16conv;
		//std::wstring_convert<std::codecvt<char16_t,char,std::mbstate_t>,char16_t> convert;

		//PDFTextString pdftext = conv.to_bytes(asdf);
		//EStatusCode encodingStatus = xobjectContentContext->Tj(conv.to_bytes(asdf));
		EStatusCode encodingStatus = xobjectContentContext->Tj(conv.to_bytes(asdf));
		xobjectContentContext->ET();
		xobjectContentContext->Q();
		xobjectContentContext->EMC();
	
		pdfWriter.EndFormXObjectAndRelease(formXObject);
	}
	   pdfWriter.EndPDF();
return 0;
}
