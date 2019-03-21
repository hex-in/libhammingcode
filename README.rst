Hamming code. 
=========================
 
    Library for hamming encode or decode.


Installation
------------

* Compile and install the library::

    pip3 install libhamming
    
* After installation you can run unit tests to make sure that the library works fine.  Execute::

    python -m libhamming.test

Usage
-----

  **In Python 3::**
    import libhamming as hamming

Example
-------

    import libhamming as hamming
    
    hamming.encode(b'0111111100')

    hamming.encode(b'\x00\x01\x01\x01\x01\x01\x01\x01\x00\x00')

    hamming.decode(b'00001110111100')
    
    hamming.decode(b'\x00\x00\x00\x00\x01\x01\x01\x00\x01\x01\x01\x01\x00\x00')



V0.2 (2019-03-21)
+++++++++++++++++++
* Release ver0.2

