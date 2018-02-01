/***************************************************************************
 *   Copyright (C) 2008 by Dominik Seichter                                *
 *   domseichter@web.de                                                    *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "TextExtractor.h"
#include "extension.h"
#include <stack>
#include <fstream>
#include <string>
using namespace std;


string txt_content;
TextExtractor::TextExtractor()
{

}

TextExtractor::~TextExtractor()
{
}
const char* file_name;
void TextExtractor::Init( const char* pszInput )
{
    file_name=pszInput;
    if( !pszInput )
    {
        PODOFO_RAISE_ERROR( ePdfError_InvalidHandle );
    }

    PdfMemDocument document;

    document.Load( pszInput );

    int nCount = document.GetPageCount();
    for( int i=0; i<nCount; i++ ) 
    {
        PdfPage* pPage = document.GetPage( i );
        
        this->ExtractText( &document, pPage );
    }
}

void TextExtractor::ExtractText( PdfMemDocument* pDocument, PdfPage* pPage ) 
{
    const char*      pszToken = NULL;
    PdfVariant       var;
    EPdfContentsType eType;

    PdfContentsTokenizer tokenizer( pPage );

    bool   bTextBlock   = false;
    PdfFont* pCurFont   = NULL;

    std::stack<PdfVariant> stack;

    while( tokenizer.ReadNext( eType, pszToken, var ) )
    {
        if( eType == ePdfContentsType_Keyword )
        {
            // support 'l' and 'm' tokens
            if( strcmp( pszToken, "l" ) == 0 || 
                strcmp( pszToken, "m" ) == 0 )
            {
                if( stack.size() == 2 )
                {
                    stack.top().GetReal();
                    stack.pop();
                    stack.top().GetReal();
                    stack.pop();
                }
                else
                {
                    fprintf( stderr, "WARNING: Token '%s' expects two arguments, but %" PDF_FORMAT_INT64 " given; ignoring\n",
                        pszToken, static_cast<pdf_int64>( stack.size() ) );

                    while( !stack.empty() )
                        stack.pop();
                }
            }
            else if( strcmp( pszToken, "BT" ) == 0 ) 
            {
                bTextBlock   = true;     
                // BT does not reset font
                // pCurFont     = NULL;
            }
            else if( strcmp( pszToken, "ET" ) == 0 ) 
            {
                if( !bTextBlock ) 
                    fprintf( stderr, "WARNING: Found ET without BT!\n" );
            }

            if( bTextBlock ) 
            {
                if( strcmp( pszToken, "Tf" ) == 0 ) 
                {
                    if( stack.size() < 2 )
                    {
                        fprintf( stderr, "WARNING: Expects two arguments for 'Tf', ignoring\n" );
                        pCurFont = NULL;
                        continue;
                    }

                    stack.pop();
                    PdfName fontName = stack.top().GetName();
                    PdfObject* pFont = pPage->GetFromResources( PdfName("Font"), fontName );
                    if( !pFont ) 
                    {
                        PODOFO_RAISE_ERROR_INFO( ePdfError_InvalidHandle, "Cannot create font!" );
                    }

                    pCurFont = pDocument->GetFont( pFont );
                    if( !pCurFont )
                    {
                        fprintf( stderr, "WARNING: Unable to create font for object %" PDF_FORMAT_INT64 " %" PDF_FORMAT_INT64 " R\n",
                                 static_cast<pdf_int64>( pFont->Reference().ObjectNumber() ),
                                 static_cast<pdf_int64>( pFont->Reference().GenerationNumber() ) );
                    }
                }
                else if( strcmp( pszToken, "Tj" ) == 0 ||
                         strcmp( pszToken, "'" ) == 0 ) 
                {
                    if( stack.size() < 1 )
                    {
                        fprintf( stderr, "WARNING: Expects one argument for '%s', ignoring\n", pszToken );
                        continue;
                    }

                    AddTextElement(pCurFont, stack.top().GetString() );
                    stack.pop();
                }
                else if( strcmp( pszToken, "\"" ) == 0 )
                {
                    if( stack.size() < 3 )
                    {
                        fprintf( stderr, "WARNING: Expects three arguments for '%s', ignoring\n", pszToken );

                        while( !stack.empty() )
                            stack.pop();

                        continue;
                    }

                    AddTextElement(pCurFont, stack.top().GetString() );
                    stack.pop();
                    stack.pop(); // remove char spacing from stack
                    stack.pop(); // remove word spacing from stack
                }
                else if( strcmp( pszToken, "TJ" ) == 0 ) 
                {
                    if( stack.size() < 3 )
                    {
                        fprintf( stderr, "WARNING: Expects one argument for '%s', ignoring\n", pszToken );
                        continue;
                    }

                    PdfArray array = stack.top().GetArray();
                    stack.pop();
                    
                    for( int i=0; i<static_cast<int>(array.GetSize()); i++ ) 
                    {
                        if( array[i].IsString() || array[i].IsHexString() )
                            AddTextElement(pCurFont, array[i].GetString() );
                    }
                }
            }
        }
        else if ( eType == ePdfContentsType_Variant )
        {
            stack.push( var );
        }
        else
        {
            // Impossible; type must be keyword or variant
            PODOFO_RAISE_ERROR( ePdfError_InternalLogic );
        }
    }
}

void TextExtractor::AddTextElement(PdfFont* pCurFont, const PdfString & rString )
{
    if( !pCurFont ) 
    {
        fprintf( stderr, "WARNING: Found text but do not have a current font: %s\n", rString.GetString() );
        return;
    }

    if( !pCurFont->GetEncoding() ) 
    {
        fprintf( stderr, "WARNING: Found text but do not have a current encoding: %s\n", rString.GetString() );
        return;
    }

    // For now just write to console
    PdfString unicode = pCurFont->GetEncoding()->ConvertToUnicode( rString, pCurFont );
    const char* pszData = unicode.GetStringUtf8().c_str();
    while( *pszData ) {
        //printf("%02x", static_cast<unsigned char>(*pszData) );
        ++pszData;
    }
    ofstream file;
    txt_content += unicode.GetStringUtf8().c_str();
    //file.open(ext_switch((string)file_name,"txt"), std::ofstream::app);
    //file << unicode.GetStringUtf8().c_str()+'\n';
    //file.close();
    //printf("%s",unicode.GetStringUtf8().c_str() );
}
