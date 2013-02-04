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

int main()
{ std::string testFileName = "text_fields - Copy - Copy.pdf";
	PDFWriter pdfWriter;
	EStatusCode status = eSuccess;
	status = pdfWriter.ModifyPDF(testFileName, ePDFVersion17, std::string("mod") + testFileName);
	PDFParser* inParser;
	inParser =&pdfWriter.GetModifiedFileParser();
	 PDFDocumentCopyingContext* copyingContext = pdfWriter.CreatePDFCopyingContextForModifiedFile();
	
	 pdfWriter.GetObjectsContext().SetCompressStreams(false);
	PDFFormXObject* formXObject = pdfWriter.GetDocumentContext().StartFormXObject(PDFRectangle(0, 0, 278.93, 16.0));
	//[227.77 790 506.7 806]
	
	//formXObject = mDocumentContext->StartFormXObject(PDFRectangle(0,0,dimensions.first,dimensions.second),inFormXObjectID);
	XObjectContentContext* xobjectContentContext = formXObject->GetContentContext();
	

	formXObject->GetContentStream()->IsStreamCompressed();
	xobjectContentContext->g(0.75);
	xobjectContentContext->re(0, 0, 278.93, 16);
	xobjectContentContext->f();
	xobjectContentContext->g(1);
	xobjectContentContext->m(1, 1);
	xobjectContentContext->l(1, 15);
	xobjectContentContext->l(277.93, 15);
	xobjectContentContext->l(276.93, 14);
	xobjectContentContext->f();
	xobjectContentContext->g(0.75293);
	xobjectContentContext->m(277.93, 15);
	xobjectContentContext->l(277.93, 1);
	xobjectContentContext->l(1, 1);
	xobjectContentContext->l(2, 2);
	xobjectContentContext->l(276.93, 2);
	xobjectContentContext->l(276.93, 14);
	xobjectContentContext->BMC();
	xobjectContentContext->q();
	xobjectContentContext->re(2, 2, 274.93, 12);
	
	xobjectContentContext->W();
	xobjectContentContext->n();
	xobjectContentContext->g(0);
	xobjectContentContext->BT();
	
	
	

	xobjectContentContext->Tf();
	
	//xobjectContentContext->Tf(0, 0);
	//xobjectContentContext->g(0);
	xobjectContentContext->Td(124.885, 4.7014);
	xobjectContentContext->TjLow("ąęćżźńół");
	xobjectContentContext->ET();
	xobjectContentContext->Q();
	xobjectContentContext->EMC();
	

	 //Get ID of a AcroForm by the label "text_1"
	 ObjectIDType modifiedAcroFormID = inParser->FindAcroFormID("text_1");
		
	PDFObjectCastPtr<PDFDictionary> AcroFormFieldObject(inParser->ParseNewObject(modifiedAcroFormID));
	
	
		ObjectIDType appearanceObjectID = formXObject->GetObjectID();
		pdfWriter.EndFormXObjectAndRelease(formXObject);

		MapIterator<PDFNameToPDFObjectMap>  objectContentIterator = AcroFormFieldObject->GetIterator();
	pdfWriter.GetObjectsContext().StartModifiedIndirectObject(modifiedAcroFormID);
        DictionaryContext* modifiedPageObject = pdfWriter.GetObjectsContext().StartDictionary();

        while(objectContentIterator.MoveNext())
        {
			if(objectContentIterator.GetKey()->GetValue() == "V")
			{
				modifiedPageObject->WriteKey("V");
				modifiedPageObject->WriteLiteralStringValue("ęąśćżźńół");
			}
			 if (objectContentIterator.GetKey()->GetValue() == "AP")
			 {
				 modifiedPageObject->WriteKey("AP");
				DictionaryContext* APDictionary = pdfWriter.GetObjectsContext().StartDictionary();
				APDictionary->WriteKey("N");
				APDictionary->WriteNewObjectReferenceValue(appearanceObjectID);
				
				pdfWriter.GetObjectsContext().EndDictionary(APDictionary);
			 }
			 //just copy other keys
            if(objectContentIterator.GetKey()->GetValue() != "V" && 
				objectContentIterator.GetKey()->GetValue() != "AP")
            {
                modifiedPageObject->WriteKey(objectContentIterator.GetKey()->GetValue());
                copyingContext->CopyDirectObjectAsIs(objectContentIterator.GetValue());
            }
        }
		pdfWriter.GetObjectsContext().EndDictionary(modifiedPageObject);
		
       pdfWriter.GetObjectsContext().EndIndirectObject();
	   pdfWriter.EndPDF();
return 0;
}
