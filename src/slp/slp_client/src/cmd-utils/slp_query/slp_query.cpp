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
/*****************************************************************************
 *  Description: scripting shell for slp_client - generates an slp QUERY 
 *
 *  Originated: September 16, 2002
 *	Original Author: Mike Day md@soft-hackle.net
 *                       mdday@us.ibm.com
 *
 *               					                    
 *  Copyright (c) 2001 - 2003  IBM                                          
 *  Copyright (c) 2000 - 2003 Michael Day                                    
 *                                                                           
 *  Permission is hereby granted, free of charge, to any person obtaining a  
 *  copy of this software and associated documentation files (the "Software"),
 *  to deal in the Software without restriction, including without limitation 
 *  the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 *  and/or sell copies of the Software, and to permit persons to whom the     
 *  Software is furnished to do so, subject to the following conditions:       
 * 
 *  The above copyright notice and this permission notice shall be included in 
 *  all copies or substantial portions of the Software.
 * 
 * 
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 *  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 *  DEALINGS IN THE SOFTWARE.
 *
 *****************************************************************************/





/**** define the following token to import labels from the slp_client library *****/
#define SLP_LIB_IMPORT
#include "../slp_client/slp_client.h"


static char *type;
static char *addr;
static char *scopes;
static char *_interface;
static char *predicate;
static char *spi;
static int16 port = 427, converge;
static BOOL dir_agent = FALSE;
static BOOL test = FALSE;
static BOOL parsable= FALSE;
static char fs='\t', rs='\n';


void free_globals(void)
{
  if(type != NULL)
    free(type);
  if(addr != NULL)
    free(addr);
  if(scopes != NULL)
    free(scopes);
  if(_interface != NULL)
    free(_interface);
  if(predicate != NULL)
    free(predicate);
  if(spi != NULL)
    free(spi);
}


void usage(void)
{
  printf("\nslp_query -- transmit an SLP Service Request and print the results.\n");
  printf("-------------------------------------------------------------------------\n");
  printf("slp_query --type=service-type-string\n");
  printf("         [--predicate=predicate-string]\n"); 
  printf("         [--scopes=scope-string]\n");
  printf("         [--address=target-IP]\n");
  printf("         [--port=target-port]\n");
  printf("         [--interface=host-IP]\n");
  printf("         [--use_da=true]\n");
  printf("         [--converge=convergence-cycles]\n");
  printf("         [--spi=security-parameters-index] (not used)\n");
  printf("         [--test] (tests parameters)\n");
  printf("         [--fs=field-separator]\n");
  printf("         [--rs=record-separator]\n");
  printf("\n");
  printf("All parameters must be a single string containing no spaces.\n");
  printf("Always use the format of <parameter>=<value>.\n");
  printf("Parameters enclosed in brackets are optional.\n");
    
   
}

BOOL get_options(int argc, char *argv[])
{
  int i;
  char *bptr;
  
  for(i = 1; i < argc; i++){
    if((*argv[i] == '-') && (*(argv[i] + 1) == '-')) {
      if(TRUE == lslp_pattern_match(argv[i] + 2, "type=*", FALSE)) {
	bptr = argv[i] + 2;
	while(*bptr != '=') bptr++;
	bptr++;
	/* handle case where type is specified twice on the cmd line */
	if(type != NULL)
	  free(type);
	type = strdup(bptr);
      } else if(TRUE == lslp_pattern_match(argv[i] + 2, "predicate=*", FALSE)) {
	bptr = argv[i] + 2;
	while(*bptr != '=') bptr++;
	bptr++;
	if(predicate != NULL)
	  free(predicate);
	predicate = strdup(bptr);
      } else if(TRUE == lslp_pattern_match(argv[i] + 2, "scopes*", FALSE)) {
	bptr = argv[i] + 2;
	while(*bptr != '=') bptr++;
	bptr++;
	if(scopes != NULL)
	  free(scopes);
	scopes = strdup(bptr);
	
      } else if(TRUE == lslp_pattern_match(argv[i] + 2, "address*", FALSE)) {
	bptr = argv[i] + 2;
	while(*bptr != '=') bptr++;
	bptr++;
	if(addr != NULL)
	  free(addr);
	addr = strdup(bptr);

      } else if(TRUE == lslp_pattern_match(argv[i] + 2, "port*", FALSE)) {
	bptr = argv[i] + 2;
	while(*bptr != '=') bptr++;
	bptr++;
	port = (int16)strtoul(bptr, NULL, 10);
      } else if(TRUE == lslp_pattern_match(argv[i] + 2, "interface*", FALSE)) {
	bptr = argv[i] + 2;
	while(*bptr != '=') bptr++;
	bptr++;
	if(_interface != NULL)
	  free(_interface);
	_interface = strdup(bptr);
      } else if(TRUE == lslp_pattern_match(argv[i] + 2, "use_da=true*", FALSE)) {
	dir_agent = TRUE;
      } else if(TRUE == lslp_pattern_match(argv[i] + 2, "test*", FALSE)) {
	test = TRUE;
      } else if(TRUE == lslp_pattern_match(argv[i] + 2, "converge=*", FALSE)) {
	bptr = argv[i] + 2;
	while(*bptr != '=') bptr++;
	bptr++;
	converge = (int16)strtoul(bptr, NULL, 10);
      } else if(TRUE == lslp_pattern_match(argv[i] + 2, "fs=*", FALSE)){
	bptr = argv[i] + 2;
	while(*bptr != '=') bptr++;
	bptr++;
	fs = *bptr;
	parsable=TRUE;
      } else if(TRUE == lslp_pattern_match(argv[i] + 2, "rs=*", FALSE)){
	bptr = argv[i] + 2;
	while(*bptr != '=') bptr++;
	bptr++;
	rs = *bptr;
	parsable=TRUE;
      }
    }
  }
  
  if( type == NULL )
    return FALSE;
  return TRUE;
}


int main(int argc, char **argv)
{
  struct slp_client *client;
  lslpMsg responses, *temp;
  
  if(FALSE == get_options(argc, argv)) {
    usage();
  } else {
    if(scopes == NULL)
      scopes = strdup("DEFAULT");
    if(test == TRUE){
      return test_query(type, predicate, scopes);
    }
    
    if(NULL != (client = create_slp_client(addr, 
					   _interface, 
					   port, 
					   "DSA", 
					   scopes, 
					   FALSE, 
					   dir_agent))) {
					     

      if( addr != NULL && inet_addr(addr) == inet_addr("127.0.0.1")) {
	client->local_srv_req(client, type, predicate, scopes);
      } else if(converge) {
	client->_convergence = converge ;
	client->converge_srv_req(client, type, predicate, scopes);
      } else {
	SOCKADDR_IN address;
	address.sin_port = htons(port);
	address.sin_family = AF_INET;
	if(addr != NULL) {
	  address.sin_addr.s_addr = inet_addr(addr);
	  client->unicast_srv_req(client, type, predicate, scopes, &address);
	} else {
	  address.sin_addr.s_addr = _LSLP_MCAST;
	  client->converge_srv_req(client, type, predicate, scopes);
	}
	
      }
      
      responses.isHead = TRUE;
      responses.next = responses.prev = &responses;
      
      client->get_response(client, &responses);
      while( ! _LSLP_IS_EMPTY(&responses)) {
	temp = responses.next;
	if(temp->type == srvRply) {
	  if(parsable == TRUE )
	    lslp_print_srv_rply_parse(temp, fs, rs);
	  else
	    lslp_print_srv_rply(temp);
	}
	_LSLP_UNLINK(temp);
	lslpDestroySLPMsg(temp, LSLP_DESTRUCTOR_DYNAMIC);
      }
      destroy_slp_client(client);
    }
  }
  
  free_globals();
  return 1;

}
