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
 *                          2019/03/23 V0.3   [Heyn] New add uint2bit\bit2uint\table.
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

static unsigned int inline __ascii2bin( const char *pasc, unsigned int length, unsigned char *pbin )
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


static PyObject * _hamming_table(PyObject *self, PyObject *args)
{
    unsigned char bits   = 4;
    unsigned int  length = 0x00000000L;
    unsigned char table[HEXIN_MAX_BUFFER_SIZE] = { 0x00 };

    if ( !PyArg_ParseTuple( args, "|I", &bits ) ) {
        return NULL;
    }

    length = hexinGrayCode( bits, table );
    return Py_BuildValue( "y#", table, length );
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


static PyObject * _hamming_bit2byte(PyObject *self, PyObject *args)
{
    const char    *data  = NULL;
             char align  = 'R';
    unsigned char compressbits = 4;
    unsigned int  length = 0x00000000L;
    unsigned char rawbits[HEXIN_MAX_BUFFER_SIZE] = { 0x00 };
    unsigned char rawbyte[HEXIN_MAX_BUFFER_SIZE] = { 0x00 };

    if ( !PyArg_ParseTuple( args, "y#|bc", &data, &length, &compressbits, &align ) ) {
        return NULL;
    }

    if ( length > HEXIN_MAX_BUFFER_SIZE ) {
        return NULL;
    }

    length = __ascii2bin( data, length , rawbits );
    if ( align == 'R' ) {
        length = bitsTobytes_align_right( rawbits, length, rawbyte, compressbits );
    } else {
        length = bitsTobytes_align_left ( rawbits, length, rawbyte, compressbits );
    }
    
    return Py_BuildValue( "y#", rawbyte, length );
}

static PyObject * _hamming_byte2bit(PyObject *self, PyObject *args)
{
    const char    *data  = NULL;
    unsigned char compressbits = 4;
    unsigned int  length = 0x00000000L;
    unsigned char rawbits[HEXIN_MAX_BUFFER_SIZE] = { 0x00 };
    unsigned char rawbyte[HEXIN_MAX_BUFFER_SIZE] = { 0x00 };

    if ( !PyArg_ParseTuple( args, "y#|b", &data, &length, &compressbits ) ) {
        return NULL;
    }

    if ( length > HEXIN_MAX_BUFFER_SIZE ) {
        return NULL;
    }

    length = bytesTobits( (unsigned char *)data, length, rawbits, compressbits );
    length = __bin2ascii( rawbits, length );
    return Py_BuildValue( "y#", rawbits, length );
}

static PyObject * _hamming_uint2bit(PyObject *self, PyObject *args)
{
    unsigned int  data = 0x00000000L;
    unsigned int  bitsize = 0x00000000L;
    unsigned char rawbits[HEXIN_MAX_BUFFER_SIZE] = { 0x00 };

    if ( !PyArg_ParseTuple( args, "|II", &data, &bitsize ) ) {
        return NULL;
    }

    bitsize = uintTobits( data, bitsize, rawbits );

    return Py_BuildValue( "y#", rawbits, bitsize );
}

static PyObject * _hamming_bit2uint(PyObject *self, PyObject *args)
{
    const char  *data = NULL;
    unsigned int  length = 0x00000000L;
    unsigned int  number = 0x00000000L;
    unsigned char rawbits[HEXIN_MAX_BUFFER_SIZE] = { 0x00 };

    if ( !PyArg_ParseTuple( args, "y#", &data, &length ) ) {
        return NULL;
    }

    length = __ascii2bin( data, length , rawbits );
    number = bitsTouint( rawbits, length );

    return Py_BuildValue( "I", number );
}

/* method table */
static PyMethodDef hammingMethods[] = {
    {"table",       _hamming_table,     METH_VARARGS, "Hexin table."   },
    {"encode",      _hamming_encode,    METH_VARARGS, "Hamming encode."},
    {"decode",      _hamming_decode,    METH_VARARGS, "Hamming decode."},
    {"bit2byte",    _hamming_bit2byte,  METH_VARARGS, "Bits to bytes." },
    {"byte2bit",    _hamming_byte2bit,  METH_VARARGS, "Bytes to bits." },
    {"uint2bit",    _hamming_uint2bit,  METH_VARARGS, "Uint to bits."  },
    {"bit2uint",    _hamming_bit2uint,  METH_VARARGS, "Bits to uint."  },
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
"\n"
"hamming.bit2byte(b'0111111100')\n"
"hamming.bit2byte(b'0111111100', 4, b'L')\n"
"hamming.bit2byte(b'0111111100', 4, b'R') # default\n"
"\n"
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


    PyModule_AddStringConstant( mod, "__version__",     "0.2"   );
    PyModule_AddStringConstant( mod, "__author__",      "Heyn"  );

    return mod;
}
