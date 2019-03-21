/*****************************************************************************************************************
 * Copyright (c) 2018-2019, Hexin (Dalian) Technology Co. Ltd All rights reserved.
 * Author  : Heyn
 * Version : V0.1
 * 
 * LICENSING TERMS:
 * -----------------
 * 
 * HISTORIC VERSION:
 * -----------------
 *          New Create at   2019/03/21 V0.2   [Heyn] Initialization.
 * 
*****************************************************************************************************************/

#include <Python.h>
#include <structmember.h>
#include "hxhamming.h"

static unsigned int inline __bin2ascii( unsigned char *pbin, unsigned int length )
{
    for ( unsigned int i=0; i<length; i++ ) {
        pbin[i] = pbin[i] + '0';
    }

    return length;
}

static unsigned int inline __ascii2bin( unsigned char *pasc, unsigned int length, unsigned char *pbin )
{
    for ( unsigned int i = 0; i<length; i++ ) {
        if ( (pasc[i]==0x00) || (pasc[i]==0x01) ) {
            pbin[i] = pasc[i];
        } else {
            pbin[i] = ( (pasc[i] - '0') == 0x00 ? 0x00 : 0x01 );
        }
    }
    return length;
}

static PyObject * _hamming_encode(PyObject *self, PyObject *args)
{
    const char    *data  = NULL;
    unsigned int  length = 0x00000000L;
    unsigned char hambits[HEXIN_MAX_BUFFER_SIZE] = { 0x00 };
    unsigned char rawbits[HEXIN_MAX_BUFFER_SIZE] = { 0x00 };

    if ( !PyArg_ParseTuple( args, "y#", &data, &length ) ) {
        return NULL;
    }

    if ( length > HEXIN_MAX_BUFFER_SIZE ) {
        return NULL;
    }

    length = __ascii2bin( data, length , rawbits );
    length = hexinHammingEncode( rawbits, length, hambits );
    length = __bin2ascii( hambits, length );

    return Py_BuildValue( "y#", hambits, length );
}

static PyObject * _hamming_decode(PyObject *self, PyObject *args)
{
    const char    *data  = NULL;
      signed int  error  = -1;
    unsigned int  length = 0x00000000L;
    unsigned char hambits[HEXIN_MAX_BUFFER_SIZE] = { 0x00 };
    unsigned char rawbits[HEXIN_MAX_BUFFER_SIZE] = { 0x00 };

    if ( !PyArg_ParseTuple( args, "y#", &data, &length ) ) {
        return NULL;
    }

    if ( length > HEXIN_MAX_BUFFER_SIZE ) {
        return NULL;
    }

    length = __ascii2bin( data, length , hambits );
    error  = hexinHammingDecode( hambits, length, rawbits, &length );
    length = __bin2ascii( rawbits, length );

    return Py_BuildValue( "(iy#)", error, rawbits, length );
}

/* method table */
static PyMethodDef hammingMethods[] = {
    {"encode",      _hamming_encode,    METH_VARARGS, "Hamming encode."},
    {"decode",      _hamming_decode,    METH_VARARGS, "Hamming decode."},    
    {NULL, NULL, 0, NULL}        /* Sentinel */
};

/* module documentation */
PyDoc_STRVAR( hamming_doc,
"import libhamming as hamming\n"
"\n"
"hamming.encode(b'0111111100')\n"
"hamming.encode(b'\x00\x01\x01\x01\x01\x01\x01\x01\x00\x00')\n"
"hamming.decode(b'00001110111100')\n"
"hamming.decode(b'\x00\x00\x00\x00\x01\x01\x01\x00\x01\x01\x01\x01\x00\x00')\n"
"(0, b'0111111100')\n"
"\n");

/* module definition structure */
static struct PyModuleDef hammingmodule = {
    PyModuleDef_HEAD_INIT,
    "hamming",              /* name of module */
    hamming_doc,            /* module documentation, may be NULL */
    -1,                     /* size of per-interpreter state of the module */
    hammingMethods
};

/* initialization function for Python 3 */
PyMODINIT_FUNC PyInit_hamming(void)
{

    PyObject *mod;

    mod = PyModule_Create( &hammingmodule );
    if ( NULL == mod ) {
        return NULL;
    }


    PyModule_AddStringConstant( mod, "__version__",     "0.1"   );
    PyModule_AddStringConstant( mod, "__author__",      "Heyn"  );

    return mod;
}
