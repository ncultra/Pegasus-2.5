/*****************************************************************************
 *  Description: scripting shell for slp_client - generates an slp QUERY 
 *
 *  Originated: September 16, 2002
 *	Original Author: Mike Day md@soft-hackle.net
 *                       mdday@us.ibm.com
 *
 *  $Header: /cvs/MSB/pegasus/src/Unsupported/slp_client/src/cmd-utils/slp_query/slp_query.c,v 1.2 2003/11/18 23:59:20 tony Exp $ 	                                                            
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
 
int8 *type, *addr, *scopes, *iface, *predicate, *spi;
int16 port = 427, converge;
BOOL dir_agent = FALSE;
BOOL test = FALSE;

void free_globals(void)
{
  if(type != NULL)
    free(type);
  if(addr != NULL)
    free(addr);
  if(scopes != NULL)
    free(scopes);
  if(iface != NULL)
    free(iface);
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
  printf("\n");
  printf("All parameters must be a single string containing no spaces.\n");
  printf("Always use the format of <parameter>=<value>.\n");
  printf("Parameters enclosed in brackets are not optional.\n");
    
   
}

BOOL get_options(int argc, char *argv[])
{
  int i;
  int8 *bptr;
  
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
	if(iface != NULL)
	  free(iface);
	iface = strdup(bptr);
      } else if(TRUE == lslp_pattern_match(argv[i] + 2, "use_da=true*", FALSE)) {
	dir_agent = TRUE;
      } else if(TRUE == lslp_pattern_match(argv[i] + 2, "test*", FALSE)) {
	test = TRUE;
      } else if(TRUE == lslp_pattern_match(argv[i] + 2, "converge=*", FALSE)) {
	bptr = argv[i] + 2;
	while(*bptr != '=') bptr++;
	bptr++;
	converge = (int16)strtoul(bptr, NULL, 10);
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
					   iface, 
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
	if(addr != NULL)
	  address.sin_addr.s_addr = inet_addr(addr);
	else
	  address.sin_addr.s_addr = _LSLP_MCAST;
	client->unicast_srv_req(client, type, predicate, scopes, &address);
      }
      
      responses.isHead = TRUE;
      responses.next = responses.prev = &responses;
      
      client->get_response(client, &responses);
      while( ! _LSLP_IS_EMPTY(&responses)) {
	temp = responses.next;
	if(temp->type == srvRply) 
	  lslp_print_srv_rply(temp);
	_LSLP_UNLINK(temp);
	lslpDestroySLPMsg(temp, LSLP_DESTRUCTOR_DYNAMIC);
      }
      destroy_slp_client(client);
    }
  }
  
  free_globals();
  return(0);

}
