//%2005////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2000, 2001, 2002 BMC Software; Hewlett-Packard Development
// Company, L.P.; IBM Corp.; The Open Group; Tivoli Systems.
// Copyright (c) 2003 BMC Software; Hewlett-Packard Development Company, L.P.;
// IBM Corp.; EMC Corporation, The Open Group.
// Copyright (c) 2004 BMC Software; Hewlett-Packard Development Company, L.P.;
// IBM Corp.; EMC Corporation; VERITAS Software Corporation; The Open Group.
// Copyright (c) 2005 Hewlett-Packard Development Company, L.P.; IBM Corp.;
// EMC Corporation; VERITAS Software Corporation; The Open Group.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// THE ABOVE COPYRIGHT NOTICE AND THIS PERMISSION NOTICE SHALL BE INCLUDED IN
// ALL COPIES OR SUBSTANTIAL PORTIONS OF THE SOFTWARE. THE SOFTWARE IS PROVIDED
// "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
// LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
// PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
// ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//==============================================================================
//
//%/////////////////////////////////////////////////////////////////////////////

#ifndef Pegasus_ContentLanguages_h
#define Pegasus_ContentLanguages_h
#include <Pegasus/Common/Config.h>
#include <Pegasus/Common/LanguageElementContainer.h>
#include <Pegasus/Common/ContentLanguageElement.h>
#ifdef PEGASUS_USE_EXPERIMENTAL_INTERFACES

PEGASUS_NAMESPACE_BEGIN

class ContentLanguagesRep;

//////////////////////////////////////////////////////////////
//
// ContentLanguages::
//
//////////////////////////////////////////////////////////////

/** <I><B>Experimental Interface</B></I><BR>
 */
class PEGASUS_COMMON_LINKAGE ContentLanguages : public LanguageElementContainer{

public:

	/**	This member is used to represent an empty ContentLanguages. Using this 
        member avoids construction of an empty ContentLanguages 
        (e.g., ContentLanguages()).
    */
    static const ContentLanguages EMPTY;

	/**
	 * Constructor
	 */
	ContentLanguages();

	/**
	 * Constructor
	 * @param hdr String - ContentLanguage header
	 */
	ContentLanguages(String hdr); 

	/**
	 * Constructor
	 * @param container Array<LanguageElement> - elements to construct the object with
	 */
	ContentLanguages(const Array<LanguageElement> &container);
	
	/**
	 * Constructor
	 * @param container Array<ContentLanguageElement> - elements to construct the object with
	 */
	ContentLanguages(const Array<ContentLanguageElement> &container);

	/**
	 * Copy Constructor
	 * @param rhs ContentLanguages - element to construct the object with
	 */
	ContentLanguages(const ContentLanguages &rhs);
	
	/**
	 * Destructor
	 */
	~ContentLanguages();
	
	/**
	 * Random access into the container
	 * @param index int - position of element to get
	 * @return ContentLanguageElement element at postion index
	 * @throws IndexOutOfBounds Exception
	 */
	ContentLanguageElement getLanguageElement(Uint32 index) const;
	
	/**
	 * Fills in the parameter array with all this objects elements
	 * @param elements Array<ContentLanguageElement> & - filled with the contents of the ContentLanguages container 
	 */
	void getAllLanguageElements(Array<ContentLanguageElement> & elements) const;

	Array<ContentLanguageElement> getAllLanguageElements()const;


	
	/**
	 * Appends the element to the container
	 * @param element ContentLanguageElement - element to append to container
	 */
	void append(ContentLanguageElement element);
	
	/**
	 * Returns the next element in the container
	 * Callers should call itrStart() ONCE before making calls to this function and
	 * anytime the callers want the iterator reset to the beginning of the container.
	 * @return ContentLanguageElement - the next element the container or ContentLanguageElement::EMPTY_REF
	 * if the end of the container has been reached.
	 */
	ContentLanguageElement itrNext();
	
	/**
	 * Finds the element in the container that matches the language_tag.
	 * @param language_tag String - case insensitive match
	 * @return int index of element if found, otherwise -1
	 */
	Sint32 find(String language_tag)const;

	
	/**
	 * @return ostream - Returns a representation of this object in ContentLanguage header format
	 * according to the RFC
	 */
	PEGASUS_COMMON_LINKAGE friend PEGASUS_STD(ostream) & operator<<(PEGASUS_STD(ostream) &stream, const ContentLanguages &cl);

	/**
	 * Assignment 
	 * @param rhs ContentLanguages
	 */
	ContentLanguages& operator=(const ContentLanguages &rhs);

	void buildLanguageElements(Array<String> values);

}; // end ContentLanguages

PEGASUS_NAMESPACE_END

#endif  // PEGASUS_USE_EXPERIMENTAL_INTERFACES

#endif
