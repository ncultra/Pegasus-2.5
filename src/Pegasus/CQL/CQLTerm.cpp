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
// Authors: David Rosckes (rosckes@us.ibm.com)
//          Bert Rivero (hurivero@us.ibm.com)
//          Chuck Carmack (carmack@us.ibm.com)
//          Brian Lucier (lucier@us.ibm.com) 
//
// Modified By:
//
//%/////////////////////////////////////////////////////////////////////////////

#include <Pegasus/CQL/CQLTerm.h>  
#include <Pegasus/CQL/CQLTermRep.h>
#include <Pegasus/CQL/CQLFactory.h>
#include <Pegasus/Query/QueryCommon/QueryContext.h>
PEGASUS_NAMESPACE_BEGIN

/*
#define PEGASUS_ARRAY_T FactorOpType
#include <Pegasus/Common/ArrayImpl.h>
#undef PEGASUS_ARRAY_T

#define PEGASUS_ARRAY_T CQLTerm
#include <Pegasus/Common/ArrayImpl.h>
#undef PEGASUS_ARRAY_T
*/


CQLTerm::CQLTerm()
{
  _rep = new CQLTermRep();
}

CQLTerm::CQLTerm(const CQLTerm& inTerm)
{
  _rep = new CQLTermRep(*inTerm._rep);
}

CQLTerm::CQLTerm(const CQLFactor& theFactor)
{
  _rep = new CQLTermRep(theFactor);
}

CQLTerm::~CQLTerm()
{
  if(_rep)
    delete _rep;
}

CQLValue CQLTerm::resolveValue(const CIMInstance& CI, const QueryContext& QueryCtx)
{
  return _rep->resolveValue(CI,QueryCtx);
}

void CQLTerm::appendOperation(FactorOpType inFactorOpType, CQLFactor inFactor)
{
  _rep->appendOperation(inFactorOpType,inFactor);
}

String CQLTerm::toString()const
{
  return _rep->toString();
}

Boolean CQLTerm::isSimple()const
{
  return _rep->isSimple();
}

Boolean CQLTerm::isSimpleValue()const
{
  return _rep->isSimpleValue();
}

Array<CQLFactor> CQLTerm::getFactors()const
{
  return _rep->getFactors();
}

Array<FactorOpType> CQLTerm::getOperators()const
{
  return _rep->getOperators();
}

void CQLTerm::applyContext(const QueryContext& inContext,
                           const CQLChainedIdentifier& inCid)
{
  _rep->applyContext(inContext,inCid);
}

CQLTerm& CQLTerm::operator=(const CQLTerm& rhs)
{
  if(&rhs != this){
    if(_rep) delete _rep;
    _rep = new CQLTermRep(*rhs._rep);
  }
  return *this;
}
/*
Boolean CQLTerm::operator==(const CQLTerm& rhs)const
{
  return (*_rep == *(rhs._rep));
}
Boolean CQLTerm::operator!=(const CQLTerm& rhs)const
{
  return (!operator==(rhs));                                                                                
}
*/
PEGASUS_NAMESPACE_END
