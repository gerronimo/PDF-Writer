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
void copyResources(PDFObject* inObject, PDFParser* inParser, PDFFormXObject* xObject);

void testingFuntion(PDFWriter* inPDFWriter, PDFFormXObject* xObject)
{
	XObjectContentContext* xobjectContentContext = xObject->GetContentContext();
	PDFDocumentCopyingContext* copyingContext = inPDFWriter->CreatePDFCopyingContextForModifiedFile();
	ObjectIDType modifiedAcroFormID; 
	EStatusCode status = copyingContext->GetSourceDocumentParser()->FindAcroFormID("Tekst1", modifiedAcroFormID);
	//ObjectIDType modifiedAcroFormID = copyingContext->GetSourceDocumentParser()->FindAcroFormID("text_1");
	PDFObjectCastPtr<PDFDictionary> AcroFormFieldObject(copyingContext->GetSourceDocumentParser()->ParseNewObject(modifiedAcroFormID));	
	MapIterator<PDFNameToPDFObjectMap>  objectContentIterator = AcroFormFieldObject->GetIterator();	
		{
			inPDFWriter->GetObjectsContext().StartModifiedIndirectObject(modifiedAcroFormID);
			
			{
				DictionaryContext* modifiedPageObject = inPDFWriter->GetObjectsContext().StartDictionary();
				{
					while(objectContentIterator.MoveNext())
					{
						if(objectContentIterator.GetKey()->GetValue() == "DR")
						{
							//xobjectContentContext->DA("asdf");
						}
						if(objectContentIterator.GetKey()->GetValue() == "DA")
						{
							{
								
								PDFObjectCastPtr<PDFLiteralString> DefaultAppearance;
								DefaultAppearance =(objectContentIterator.GetValue());
								
								xobjectContentContext->DA(DefaultAppearance->GetValue());
							}
						}
						if(objectContentIterator.GetKey()->GetValue() == "Rect")
						{
							{
								
								PDFObjectCastPtr<PDFArray> Rect;
								Rect =(objectContentIterator.GetValue());
								
								double left; 
								double bottom; 
								double width; 
								double height; 

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
								//2, 2, 274.93, 12
								//xobjectContentContext->re(left,bottom, width , height);
								//xobjectContentContext->re(2,2, 274.93 , 12);
							}
						}
						if(objectContentIterator.GetKey()->GetValue() == "V")
						{
							modifiedPageObject->WriteKey("V");
							modifiedPageObject->WriteLiteralStringValue("asdf");
						}
						if (objectContentIterator.GetKey()->GetValue() == "AP")
						{

							//objectContentIterator.GetValue();
							modifiedPageObject->WriteKey("AP");
							copyResources(objectContentIterator.GetValue(), copyingContext->GetSourceDocumentParser(), xObject);
							{
								DictionaryContext* APDictionary = inPDFWriter->GetObjectsContext().StartDictionary();
								APDictionary->WriteKey("R");
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
void copyResources(PDFObject* inObject, PDFParser* inParser, PDFFormXObject* xObject)
{
	{
		/*PDFObjectCastPtr<PDFDictionary> AcroFormFieldObject(copyingContext->GetSourceDocumentParser()->ParseNewObject(modifiedAcroFormID));	
	MapIterator<PDFNameToPDFObjectMap>  objectContentIterator = AcroFormFieldObject->GetIterator();	*/
		
		//MapIterator<PDFNameToPDFObjectMap>  objectContentIterator = AcroFormFieldObject->GetIterator();	

				/*PDFObjectCastPtr<PDFDictionary> OldAPDictionary = inObject;
				PDFObjectCastPtr<PDFIndirectObjectReference> olAP(OldAPDictionary->QueryDirectObject("N"));
				PDFObjectCastPtr<PDFStreamInput> olapObject(inParser->ParseNewObject( olAP->mObjectID));
				PDFObjectCastPtr<PDFDictionary> streamDictionary(olapObject->QueryStreamDictionary());
				PDFObjectCastPtr<PDFDictionary> Resources(streamDictionary->QueryDirectObject("Resources"));
				PDFObjectCastPtr<PDFDictionary> Font(Resources->QueryDirectObject("Font"));
				PDFObjectCastPtr<PDFIndirectObjectReference> Helv(Font->QueryDirectObject("Helv"));*/
				//xObject->GetResourcesDictionary().AddFontMapping(Helv->mObjectID);
				//xObject->GetResourcesDictionary().AddFontMapping(31);
				//ZapfDingbats
	}
}
int main()

{ 
	const std::string materials_dir = MATERIALS_DIR;
	//const std::string fileName = "text_fields.pdf";
	const std::string fileName = "A1 PL.pdf";

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

		//PDFFormXObject* formXObject = pdfWriter.GetDocumentContext().StartFormXObject(PDFRectangle(0, 0, 0, 0));
		PDFFormXObject* formXObject = pdfWriter.GetDocumentContext().StartFormXObject(PDFRectangle(0.0, 0.0, 7.2, 7.03));

		
		//PDFFormXObject* formXObject = pdfWriter.GetDocumentContext().StartFormXObject(PDFRectangle(0, 0, 278.93, 16.0));
	
		const std::string font_file = FONT_fILE;
		PDFUsedFont* font = pdfWriter.GetFontForFile(font_file);
		if(!font)
		{
			status = PDFHummus::eFailure;
		}
		
		XObjectContentContext* xobjectContentContext = formXObject->GetContentContext();
		xobjectContentContext->BMC();
		xobjectContentContext->q();
		xobjectContentContext->re(1, 1, 374.851, 9.613);

		//xobjectContentContext->re(2, 2, 274.93, 12);
		//xobjectContentContext->re(0, 2, 0, 0);
		xobjectContentContext->W();
		xobjectContentContext->n();
		xobjectContentContext->g(0);
		xobjectContentContext->BT();
		xobjectContentContext->Tf(font,10);
		
			//xobjectContentContext->Tm(6668, 0.0, 0.0, 1.0, 0.0, 0.0);
		//xobjectContentContext->Tm(0,0,7.1,7.2);
		//xobjectContentContext->Tm(20,0,0,20,40,822);
		//xobjectContentContext->Tm(30,0,0,30,78.4252,662.8997);
		//xobjectContentContext->Td(124.885, 4.7014);
			//xobjectContentContext->Td(124.885, 4.7014);
		xobjectContentContext->Td(2, 3.165);

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
	

		ObjectIDType appearanceObjectID = formXObject->GetObjectID();
		testingFuntion(&pdfWriter, formXObject);
		pdfWriter.EndFormXObjectAndRelease(formXObject);
	}
		   
	   pdfWriter.EndPDF();
return 0;
}
