/*
  This code is part of FreeWeb - an FCP-based client for Freenet

  Designed and implemented by David McNab, david@rebirthing.co.nz
  CopyLeft (c) 2001 by David McNab

  The FreeWeb website is at http://freeweb.sourceforge.net
  The website for Freenet is at http://freenet.sourceforge.net

  This code is distributed under the GNU Public Licence (GPL) version 2.
  See http://www.gnu.org/ for further details of the GPL.
*/

#ifndef _EZFCPLIB_H
#define _EZFCPLIB_H 

/*
  Necessary includes

  These includes are useful for all of the FCPtools code.  Any module-specific
  include should be placed within said module.

*/
#include "compat.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h> 

/*
  Threshold levels for the user-provided fcpLogCallback() function
  fcpLogCallback will be called with a verbosity argument, which will
  be one of these values. This allows the client program to screen log
  messages according to importance
*/
#define FCP_LOG_CRITICAL      0
#define FCP_LOG_NORMAL        1
#define FCP_LOG_VERBOSE       2
#define FCP_LOG_DEBUG         3
#define FCP_LOG_MESSAGE_SIZE  4096   /* Was 65K */

/*
  Lengths of allocated strings/arrays.
*/
#define L_URI         256
#define L_HOST        128
#define L_FILENAME    128
#define L_KEY         128
#define L_KEYINDEX    128
#define L_KSK         32768


/*
  Splitfiles handling definitions

  For keeping track of splitfile insert threads - used by fcpputsplit
  will discontinue when splitfiles mgr is working
*/
typedef struct {
  char *buffer;
  char *threadSlot;
  int blocksize;
  char **key;          /* key URI, if inserting metadata */
} fcpPutJob;


#define SPLIT_BLOCK_SIZE       262144    /* default split part size (256*1024) */
#define FCP_MAX_SPLIT_THREADS  8

#define SPLIT_INSSTAT_IDLE     0        /* no splitfile insert requested */
#define SPLIT_INSSTAT_WAITING  1        /* waiting for mgr thread to pick up */
#define SPLIT_INSSTAT_INPROG   2        /* in progress */
#define SPLIT_INSSTAT_BADNEWS  3        /* failure - awaiting cleanup */
#define SPLIT_INSSTAT_MANIFEST 4        /* inserting splitfile manifest */
#define SPLIT_INSSTAT_SUCCESS  5        /* full insert completed successfully */
#define SPLIT_INSSTAT_FAILED   6        /* insert failed somewhere */

/*
  Splitfile Insert Control Blocks
*/
typedef struct _splitChunk {
  char key[L_URI]; /* CHK key of inserted chunk */
  int status;            /* insert status of this chunk */
  int index;             /* index num of this chunk */
  char *chunk;           /* byte-image of chunk to insert - malloc()'ed */
  int size;              /* SIZE of this chunk */
} splitChunkIns;


typedef struct _splitJob {
  char key[L_URI];

  char status;               /* status as advised by splitmgr thread */
  int fd;                    /* fd of file we are inserting from, if applicable */
  int numChunks;             /* total number of chunks to insert */
  int doneChunks;            /* number of chunks successfully inserted */
  int totalSize;             /* total number of bytes to insert */
  char *fileName;            /* path of file being inserted */
  char *mimeType;
  splitChunkIns *chunk;      /* malloc'ed array of split chunk control blocks */
  struct _splitJob *next;  /* next on linked list */
} splitJobIns;

/*
  General FCP definitions
*/
#define FCP_ID_REQUIRED

#define RECV_BUFSIZE            2048

#define EZFCP_DEFAULT_HOST      "localhost"
#define EZFCP_DEFAULT_PORT      8481
#define EZFCP_DEFAULT_HTL       3
#define EZFCP_DEFAULT_REGRESS   3
#define EZFCP_DEFAULT_RAWMODE   0

/*
  flags for fcpOpenKey()
*/
#define _FCP_O_READ         0x100
#define _FCP_O_WRITE        0x200
#define _FCP_O_RAW          0x400   /* disable automatic metadata handling */


/*
  defs for metadata structure
*/
#define KEY_TYPE_MSK        'M'
#define KEY_TYPE_SSK        'S'
#define KEY_TYPE_KSK        'K'
#define KEY_TYPE_CHK        'C'

#define META_TYPE_REDIRECT  'R'
#define META_TYPE_DBR       'D'
#define META_TYPE_CONTENT   'C'
#define META_TYPE_MAPFILE   'M'

#define META_TYPE_04        '4'

/*
  cdoc type fields
*/
#define META_TYPE_04_NONE   'n'
#define META_TYPE_04_REDIR  'r'
#define META_TYPE_04_DBR    'd'
#define META_TYPE_04_SPLIT  's'


/*
  Define structures for incoming data from node

  Tokens for response types
*/
#define FCPRESP_TYPE_HELLO          1
#define FCPRESP_TYPE_SUCCESS        2
#define FCPRESP_TYPE_DATAFOUND      3
#define FCPRESP_TYPE_DATACHUNK      4
#define FCPRESP_TYPE_FORMATERROR    5
#define FCPRESP_TYPE_URIERROR       6
#define FCPRESP_TYPE_DATANOTFOUND   7
#define FCPRESP_TYPE_ROUTENOTFOUND  8
#define FCPRESP_TYPE_KEYCOLLISION   9
#define FCPRESP_TYPE_SIZEERROR      10
#define FCPRESP_TYPE_FAILED         11


/*
  Tokens for receive states
*/
#define RECV_STATE_WAITING      0
#define RECV_STATE_GOTHEADER    1

/*
  first - basic node hello response
*/
typedef struct {
  char *protocol;  /* malloc - protocol ID */
  char *node;      /* malloc - node ID */
} FCPRESP_HELLO;

/*
  Failed response
*/
typedef struct {
  char *reason;    /* node explanation of failure */
} FCPRESP_FAILED;

/*
  SVK keypair response
*/
typedef struct {
  char *pubkey;    /* SSK public key */
  char *privkey;   /* SSK private key */
  char *uristr;    /* generated URI */
} FCPRESP_SVKKEYPAIR;


/*
  Received data header response
*/
typedef struct {
  int dataLength;             /* DataLength=<number: number of bytes of
											metadata+data> */
  int metaLength;             /* MetadataLength=<number: default=0: number of
											bytes of metadata> */
  char *metaData;             /* malloc metadata */
  char *metaPtr;              /* current pointer into metadata */
} FCPRESP_DATAFOUND;


/*
  Received data chunk response
*/
typedef struct {
  int length;     /* Length=<number: number of bytes in trailing field> */
  char *data;     /* MetadataLength=<number: default=0: number of bytes of	metadata> */
  char *dataptr;  /* points into data buf for partial reads */
  char *dataend;  /* points just after last byte of data */
} FCPRESP_DATACHUNK;


typedef struct {
  char *text;
} FCPRESP_FORMATERROR;


typedef struct {
  char *text;
} FCPRESP_URIERROR;


typedef struct {
  char *text;
} FCPRESP_KEYCOLLISION;


/*
  Now bundle all these together
*/
typedef struct {
  int type;

  struct {
	 FCPRESP_HELLO           hello;
	 FCPRESP_FAILED          failed;
	 FCPRESP_SVKKEYPAIR      keypair;
	 FCPRESP_DATAFOUND       datafound;
	 FCPRESP_DATACHUNK       datachunk;
	 FCPRESP_FORMATERROR     fmterror;
	 FCPRESP_URIERROR        urierror;
	 FCPRESP_KEYCOLLISION    keycollision;
  } body;
} FCPRESP;


/*
  Universal tokens for metadata parsing

  Freenet URI split up into its parts
*/
typedef struct {
  char type;
  char is_msk;        /* set if key is an MSK */
  char keyid[128];
  char path[128];     /* only used with SSKs */
  char uri_str[256];  /* raw uri string */
  char subpath[128];  /* only used for MSKs - the part after the '//' */
  int  numdocs;       /* number of MSK document names in 'docname[]' array */
  char **docname;     /* array of docname pointers -
								 'SSK@blah/path//name1//name2//...//namen' */
} FCP_URI;


/*
  Container for a splitfile chunk
*/
typedef struct _04CHUNK {
  char uri[L_URI]; /* URI of this chunk */

  /* parser internal use only */
  struct _04CHUNK *_next;
}
META04SPLITCHUNK;


/*
  Container for splitfile check piece
*/
typedef struct _04PIECE {
  char uri[L_URI];   /* uri of check piece */
  int graphLen;            /* size of graph elementa array */
  int *graph;              /* array of graph elements */

  /* parser internal use only */
  struct _04PIECE *_next;
} META04SPLITPIECE;


/*
  Container for a splitfile check level
*/
typedef struct _04LEVEL {
  int numPieces;              /* number of check pieces */
  META04SPLITPIECE *piece;    /* array of check piece specs */

  /* parser internal use only */
  struct _04LEVEL *_next;
} META04SPLITLEVEL;


typedef struct {
  char *name;              /* key name */
  char *value;             /* value if any, or NULL */
} KEYVALPAIR;


typedef struct {
  int numFields;           /* number of fields in this cdoc */
  int type;
  KEYVALPAIR *key;        /* array of key/value pairs */
} FLDSET;

/*
  Main 0.4 metadata structure
*/
typedef struct {
  char vers[16];
  int numDocs;
  char *trailingInfo;         /* optional */
  FLDSET **cdoc;              /* new - array of cdocs */
} META04;

/*
  Definitions for key index access
*/
typedef struct {
  char name[L_KEYINDEX];     /* name of key index */
  int next_keynum;                 /* the next key we are retrieving */
  char basedate[9];                /* basedate of key, if using basedates */
} FCP_KEYINDEX;


/*
  Splitfile control structure
*/
#define CHUNK_STATUS_WAITING 0
#define CHUNK_STATUS_INPROG  1
#define CHUNK_STATUS_DONE    2

typedef struct {
  int chunkSize;
  int chunkTotal;
  int chunksInserted;
  char isfile;           /* TRUE if splitting a file, FALSE if memory */
  int keyfd;             /* fd of open file being inserted, if a file */
  char *keymem;          /* ptr to block of mem being inserted, if mem */
  char *chunkStatus;     /* array of status bytes for chunks */
} FCP_SPLIT;

/*
  Basic control block for FCP connections
*/
#define _FCPCONN struct _fcpconn
typedef _FCPCONN {
  int socket;
  int Status;
  /*
	 int recvState;
	 unsigned char rawBuf[RECV_BUFSIZE]; * ring buffer for incoming node response data *
	 unsigned char *rawBufStart;         * start ptr into rawBuf *
	 unsigned char *rawbufEnd;           * end ptr into rawBuf - points JUSTAFTER last char *
  */
  FCPRESP response;
} FCPCONN;

/*
  Now put these together into the main HFCP structure
*/
typedef struct {
  int malloced;  /* set if this block was created via malloc */
  int htl;       /* hops to live - defaults to 25 */
  int regress;   /* days to regress when retrying failed date-redirects */
  int raw;       /* set to disable auto metadata handling */
  int verbose;   /* set to enable status printfs to stdout */
  int keysize;   /* with requests, this is the size of key data */
  int bytesread; /* num bytes read from key so far */
  int openmode;
  char protocol[64];
  char node[256];
  
  char *rawMetadata;   /* raw metadata read from file when in raw mode */
  META04 *meta;        /* structure containing parsed metadata */
  FLDSET *fields;
  char mimeType[80];

  struct {
	 FCP_URI *uri;        /* uri of key being inserted */
	 int fd_data;         /* fd for writing key data to temp file */
	 int num_data_wr;     /* num bytes of normal data written */
	 char data_temp_file[L_tmpnam];   /* temporary file full path */
	 int fd_meta;         /* fd for writing key metadata to temp file */
	 int num_meta_wr;     /* num bytes of metadata written */
	 char meta_temp_file[L_tmpnam];   /* temporary file full path */
  } wr_info;

  FCPCONN conn;

  FCP_KEYINDEX keyindex;
  char created_uri[L_KEY];  /* filled in by library after writing key */
  char pubkey[L_KEY];       /* filled in after writing a key */
  char privkey[L_KEY];      /* filled in after writing a key */
  char failReason[256];           /* reason sent back with failure msg */

  splitJobIns split;              /* control structure for insert split job */
} HFCP;

/*
  Function prototypes
 */
#ifdef __cplusplus
#define _C_ "C"
#else
#define _C_
#endif

extern _C_ int      fcpStartup(char *host, int port, int defaultHtl, int raw, int maxSplitThreads);
extern _C_ HFCP    *fcpCreateHandle();
extern _C_ void     fcpInitHandle(HFCP *hfcp);
extern _C_ int      fcpMakeSvkKeypair(HFCP *hfcp, char *pubkey, char *privkey);
extern _C_ void     fcpSetHtl(HFCP *hfcp, int htl);
extern _C_ void     fcpSetVerbose(HFCP *hfcp, int verbose);
extern _C_ void     fcpSetRegress(HFCP *hfcp, int regress);
extern _C_ void     fcpSetSplitSize(int chunkSize);
extern _C_ void     fcpSetSplitThreads(int threads);
extern _C_ int      fcpRawMode(HFCP *hfcp, int flag);
extern _C_ void     fcpDestroyHandle(HFCP *hfcp);
extern _C_ int      fcpGetKeyToMem(HFCP *hfcp, char *keyname, char **pdata, char **metadata);
extern _C_ int      fcpGetKeyToFile(HFCP *hfcp, char *key, char *file, char **pMetadata);
extern _C_ int      fcpPutKeyFromFile(HFCP *hfcp, char *key, char *file, char *metadata);
extern _C_ int      fcpPutKeyFromMem(HFCP *hfcp, char *name, char *data, char *metadata, int datalen);
extern _C_ int      fcpOpenKey(HFCP *hfcp, char *key, int mode);
extern _C_ int      fcpReadKey(HFCP *hfcp, char *buf, int len);
extern _C_ int      fcpCloseKey(HFCP *hfcp);
extern _C_ int      fcpWriteKey(HFCP *hfcp, char *buf, int len);
extern _C_ int      fcpWriteKeyMeta(HFCP *hfcp, char *buf, int len);

extern _C_ int      fcpInsSplitFile(HFCP *hfcp, char *key, char *fileName, char *metadata);

extern _C_ int      fcpOpenKeyIndex(HFCP *hfcp, char *name, char *date, int start);
extern _C_ int      fcpReadKeyIndex(HFCP *hfcp, char **pdata, int keynum);
extern _C_ int      fcpWriteKeyIndex(HFCP *hfcp, char *data);

extern _C_ int      _fcpSockInit();
extern _C_ int      _fcpSockConnect(HFCP *hfcp);
extern _C_ void     _fcpSockDisconnect(HFCP *hfcp);
extern _C_ int      _fcpSockReceive(HFCP *hfcp, char *buf, int len);
extern _C_ int      _fcpSockSend(HFCP *hfcp, char *buf, int len);
extern _C_ void     _fcpClose(HFCP *hfcp);
extern _C_ int      _fcpRecvResponse(HFCP *hfcp);
extern _C_ int      _fcpReadBlk(HFCP *hfcp, char *buf, int len);
extern _C_ FCP_URI *_fcpParseUri(char *key);
extern _C_ void     _fcpFreeUri(FCP_URI *uri);
extern _C_ void     _fcpLog(int level, char *format,...);
extern _C_ void     _fcpInitSplit(int maxThreads);

extern _C_ META04  *parseMeta(char *buf);
extern _C_ void     freeMeta(META04 *meta);

/*
  Utility functions defined here (not part of the protocol itself).
 */
extern _C_ void    *safeMalloc(int nbytes);
extern _C_ long     cdocIntVal(META04 *meta, char *cdocName, char *keyName, long defVal);
extern _C_ long     cdocHexVal(META04 *meta, char *cdocName, char *keyName, long defVal);
extern _C_ char    *cdocStrVal(META04 *meta, char *cdocName, char *keyName, char *defVal);
extern _C_ FLDSET  *cdocFindDoc(META04 *meta, char *cdocName);
extern _C_ char    *cdocLookupKey(FLDSET *fldset, char *keyName);

#endif