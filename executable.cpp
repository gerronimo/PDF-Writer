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
	PDFDocumentCopyingContext* copyingContext = inPDFWriter->CreatePDFCopyingContextForModifiedFile();
	ObjectIDType modifiedAcroFormID = copyingContext->GetSourceDocumentParser()->FindAcroFormID("text_1");
	PDFObjectCastPtr<PDFDictionary> AcroFormFieldObject(copyingContext->GetSourceDocumentParser()->ParseNewObject(modifiedAcroFormID));	
	MapIterator<PDFNameToPDFObjectMap>  objectContentIterator = AcroFormFieldObject->GetIterator();	
		{
			inPDFWriter->GetObjectsContext().StartModifiedIndirectObject(modifiedAcroFormID);
			
			{
				DictionaryContext* modifiedPageObject = inPDFWriter->GetObjectsContext().StartDictionary();
				{
					while(objectContentIterator.MoveNext())
					{
						if(objectContentIterator.GetKey()->GetValue() == "V")
						{
							modifiedPageObject->WriteKey("V");
							modifiedPageObject->WriteLiteralStringValue("asdf");
						}
						if (objectContentIterator.GetKey()->GetValue() == "AP")
						{
							modifiedPageObject->WriteKey("AP");
							copyResources(objectContentIterator.GetValue(), copyingContext->GetSourceDocumentParser(), xObject);
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
void copyResources(PDFObject* inObject, PDFParser* inParser, PDFFormXObject* xObject)
{
	{
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
{ std::string testFileName = "text_fields.pdf";
	std::string text = "dupa";
	PDFWriter pdfWriter;
	EStatusCode status = eSuccess;
	status = pdfWriter.ModifyPDF(testFileName, ePDFVersion17, std::string("mod") + testFileName);
	PDFParser* inParser;
	inParser =&pdfWriter.GetModifiedFileParser();
	 PDFDocumentCopyingContext* copyingContext = pdfWriter.CreatePDFCopyingContextForModifiedFile();
	
	 pdfWriter.GetObjectsContext().SetCompressStreams(false);
	{
			// basic text placement, with positioning (tm and td). and setting the font
PDFUsedFont* arialTTF = pdfWriter.GetFontForFile("C:/TestMaterials/fonts/arial.ttf");


if(!arialTTF)
{
status = eFailure;

}

		PDFFormXObject* formXObject = pdfWriter.GetDocumentContext().StartFormXObject(PDFRectangle(0, 0, 278.93, 16.0));
	
		PDFUsedFont* font = pdfWriter.GetFontForFile(
                                "C:/TestMaterials/fonts/HLB_____.PFB",
                                "C:/TestMaterials/fonts/HLB_____.PFM");
		if(!font)
		{
			status = PDFHummus::eFailure;
		}
		
		//formXObject->GetResourcesDictionary().AddFontMapping(
		XObjectContentContext* xobjectContentContext = formXObject->GetContentContext();
		//mNotDefGlyphName = GetGlyphName(gindex);
		
		/*xobjectContentContext->g(0.75);
		xobjectContentContext->re(0, 0, 278.93, 16);
		xobjectContentContext->f();
		xobjectContentContext->g(1);
		xobjectContentContext->m(1, 1);
		xobjectContentContext->l(1, 15);
		xobjectContentContext->l(277.93, 15);
		xobjectContentContext->l(276.93, 14);
		xobjectContentContext->l(2, 14);
		xobjectContentContext->l(2, 2);
		xobjectContentContext->f();
		xobjectContentContext->g(0.75293);
		xobjectContentContext->m(277.93, 15);
		xobjectContentContext->l(277.93, 1);
		xobjectContentContext->l(1, 1);
		xobjectContentContext->l(2, 2);
		xobjectContentContext->l(276.93, 2);
		xobjectContentContext->l(276.93, 14);
		xobjectContentContext->f();*/
		xobjectContentContext->BMC();
		xobjectContentContext->q();
		xobjectContentContext->re(2, 2, 274.93, 12);
		xobjectContentContext->W();
		xobjectContentContext->n();
		xobjectContentContext->g(0);
		xobjectContentContext->BT();
		xobjectContentContext->Tf(font,20);
		//xobjectContentContext->Tm(20,0,0,20,40,822);
		//xobjectContentContext->Tm(30,0,0,30,78.4252,662.8997);
		xobjectContentContext->Td(124.885, 4.7014);
		//EStatusCode encodingStatus = xobjectContentContext->Tj("abcd");
		//xobjectContentContext->TjHexLow("hexlow");
		//xobjectContentContext->Tj("asdfęąść");
			StringOrDoubleList stringsAndWidthsB;
//(A) 120 (W) 120 (A) 95 (Y again)
stringsAndWidthsB.push_back(StringOrDouble("Ą"));
stringsAndWidthsB.push_back(StringOrDouble(120));
stringsAndWidthsB.push_back(StringOrDouble("W"));
stringsAndWidthsB.push_back(StringOrDouble(120));
stringsAndWidthsB.push_back(StringOrDouble("A"));
stringsAndWidthsB.push_back(StringOrDouble(95));
stringsAndWidthsB.push_back(StringOrDouble("Ą"));
//xobjectContentContext->TJ(stringsAndWidthsB);

//GlyphUnicodeMappingList glyphs;
//glyphs.push_back(GlyphUnicodeMapping(34,'Ą'));
//
//xobjectContentContext->Tj(glyphs);	

		/*if(encodingStatus != PDFHummus::eSuccess)
			std::cout<<"Could not find some of the glyphs for this font";*/
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
