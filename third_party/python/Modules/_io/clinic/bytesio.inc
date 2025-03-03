/* clang-format off */
/*[clinic input]
preserve
[clinic start generated code]*/

PyDoc_STRVAR(_io_BytesIO_readable__doc__,
"readable($self, /)\n"
"--\n"
"\n"
"Returns True if the IO object can be read.");

#define _IO_BYTESIO_READABLE_METHODDEF    \
    {"readable", (PyCFunction)_io_BytesIO_readable, METH_NOARGS, _io_BytesIO_readable__doc__},

static PyObject *
_io_BytesIO_readable_impl(bytesio *self);

static PyObject *
_io_BytesIO_readable(bytesio *self, PyObject *Py_UNUSED(ignored))
{
    return _io_BytesIO_readable_impl(self);
}

PyDoc_STRVAR(_io_BytesIO_writable__doc__,
"writable($self, /)\n"
"--\n"
"\n"
"Returns True if the IO object can be written.");

#define _IO_BYTESIO_WRITABLE_METHODDEF    \
    {"writable", (PyCFunction)_io_BytesIO_writable, METH_NOARGS, _io_BytesIO_writable__doc__},

static PyObject *
_io_BytesIO_writable_impl(bytesio *self);

static PyObject *
_io_BytesIO_writable(bytesio *self, PyObject *Py_UNUSED(ignored))
{
    return _io_BytesIO_writable_impl(self);
}

PyDoc_STRVAR(_io_BytesIO_seekable__doc__,
"seekable($self, /)\n"
"--\n"
"\n"
"Returns True if the IO object can be seeked.");

#define _IO_BYTESIO_SEEKABLE_METHODDEF    \
    {"seekable", (PyCFunction)_io_BytesIO_seekable, METH_NOARGS, _io_BytesIO_seekable__doc__},

static PyObject *
_io_BytesIO_seekable_impl(bytesio *self);

static PyObject *
_io_BytesIO_seekable(bytesio *self, PyObject *Py_UNUSED(ignored))
{
    return _io_BytesIO_seekable_impl(self);
}

PyDoc_STRVAR(_io_BytesIO_flush__doc__,
"flush($self, /)\n"
"--\n"
"\n"
"Does nothing.");

#define _IO_BYTESIO_FLUSH_METHODDEF    \
    {"flush", (PyCFunction)_io_BytesIO_flush, METH_NOARGS, _io_BytesIO_flush__doc__},

static PyObject *
_io_BytesIO_flush_impl(bytesio *self);

static PyObject *
_io_BytesIO_flush(bytesio *self, PyObject *Py_UNUSED(ignored))
{
    return _io_BytesIO_flush_impl(self);
}

PyDoc_STRVAR(_io_BytesIO_getbuffer__doc__,
"getbuffer($self, /)\n"
"--\n"
"\n"
"Get a read-write view over the contents of the BytesIO object.");

#define _IO_BYTESIO_GETBUFFER_METHODDEF    \
    {"getbuffer", (PyCFunction)_io_BytesIO_getbuffer, METH_NOARGS, _io_BytesIO_getbuffer__doc__},

static PyObject *
_io_BytesIO_getbuffer_impl(bytesio *self);

static PyObject *
_io_BytesIO_getbuffer(bytesio *self, PyObject *Py_UNUSED(ignored))
{
    return _io_BytesIO_getbuffer_impl(self);
}

PyDoc_STRVAR(_io_BytesIO_getvalue__doc__,
"getvalue($self, /)\n"
"--\n"
"\n"
"Retrieve the entire contents of the BytesIO object.");

#define _IO_BYTESIO_GETVALUE_METHODDEF    \
    {"getvalue", (PyCFunction)_io_BytesIO_getvalue, METH_NOARGS, _io_BytesIO_getvalue__doc__},

static PyObject *
_io_BytesIO_getvalue_impl(bytesio *self);

static PyObject *
_io_BytesIO_getvalue(bytesio *self, PyObject *Py_UNUSED(ignored))
{
    return _io_BytesIO_getvalue_impl(self);
}

PyDoc_STRVAR(_io_BytesIO_isatty__doc__,
"isatty($self, /)\n"
"--\n"
"\n"
"Always returns False.\n"
"\n"
"BytesIO objects are not connected to a TTY-like device.");

#define _IO_BYTESIO_ISATTY_METHODDEF    \
    {"isatty", (PyCFunction)_io_BytesIO_isatty, METH_NOARGS, _io_BytesIO_isatty__doc__},

static PyObject *
_io_BytesIO_isatty_impl(bytesio *self);

static PyObject *
_io_BytesIO_isatty(bytesio *self, PyObject *Py_UNUSED(ignored))
{
    return _io_BytesIO_isatty_impl(self);
}

PyDoc_STRVAR(_io_BytesIO_tell__doc__,
"tell($self, /)\n"
"--\n"
"\n"
"Current file position, an integer.");

#define _IO_BYTESIO_TELL_METHODDEF    \
    {"tell", (PyCFunction)_io_BytesIO_tell, METH_NOARGS, _io_BytesIO_tell__doc__},

static PyObject *
_io_BytesIO_tell_impl(bytesio *self);

static PyObject *
_io_BytesIO_tell(bytesio *self, PyObject *Py_UNUSED(ignored))
{
    return _io_BytesIO_tell_impl(self);
}

PyDoc_STRVAR(_io_BytesIO_read__doc__,
"read($self, size=None, /)\n"
"--\n"
"\n"
"Read at most size bytes, returned as a bytes object.\n"
"\n"
"If the size argument is negative, read until EOF is reached.\n"
"Return an empty bytes object at EOF.");

#define _IO_BYTESIO_READ_METHODDEF    \
    {"read", (PyCFunction)_io_BytesIO_read, METH_FASTCALL, _io_BytesIO_read__doc__},

static PyObject *
_io_BytesIO_read_impl(bytesio *self, PyObject *arg);

static PyObject *
_io_BytesIO_read(bytesio *self, PyObject **args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    PyObject *arg = Py_None;

    if (!_PyArg_UnpackStack(args, nargs, "read",
        0, 1,
        &arg)) {
        goto exit;
    }

    if (!_PyArg_NoStackKeywords("read", kwnames)) {
        goto exit;
    }
    return_value = _io_BytesIO_read_impl(self, arg);

exit:
    return return_value;
}

PyDoc_STRVAR(_io_BytesIO_read1__doc__,
"read1($self, size, /)\n"
"--\n"
"\n"
"Read at most size bytes, returned as a bytes object.\n"
"\n"
"If the size argument is negative or omitted, read until EOF is reached.\n"
"Return an empty bytes object at EOF.");

#define _IO_BYTESIO_READ1_METHODDEF    \
    {"read1", (PyCFunction)_io_BytesIO_read1, METH_O, _io_BytesIO_read1__doc__},

PyDoc_STRVAR(_io_BytesIO_readline__doc__,
"readline($self, size=None, /)\n"
"--\n"
"\n"
"Next line from the file, as a bytes object.\n"
"\n"
"Retain newline.  A non-negative size argument limits the maximum\n"
"number of bytes to return (an incomplete line may be returned then).\n"
"Return an empty bytes object at EOF.");

#define _IO_BYTESIO_READLINE_METHODDEF    \
    {"readline", (PyCFunction)_io_BytesIO_readline, METH_FASTCALL, _io_BytesIO_readline__doc__},

static PyObject *
_io_BytesIO_readline_impl(bytesio *self, PyObject *arg);

static PyObject *
_io_BytesIO_readline(bytesio *self, PyObject **args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    PyObject *arg = Py_None;

    if (!_PyArg_UnpackStack(args, nargs, "readline",
        0, 1,
        &arg)) {
        goto exit;
    }

    if (!_PyArg_NoStackKeywords("readline", kwnames)) {
        goto exit;
    }
    return_value = _io_BytesIO_readline_impl(self, arg);

exit:
    return return_value;
}

PyDoc_STRVAR(_io_BytesIO_readlines__doc__,
"readlines($self, size=None, /)\n"
"--\n"
"\n"
"List of bytes objects, each a line from the file.\n"
"\n"
"Call readline() repeatedly and return a list of the lines so read.\n"
"The optional size argument, if given, is an approximate bound on the\n"
"total number of bytes in the lines returned.");

#define _IO_BYTESIO_READLINES_METHODDEF    \
    {"readlines", (PyCFunction)_io_BytesIO_readlines, METH_FASTCALL, _io_BytesIO_readlines__doc__},

static PyObject *
_io_BytesIO_readlines_impl(bytesio *self, PyObject *arg);

static PyObject *
_io_BytesIO_readlines(bytesio *self, PyObject **args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    PyObject *arg = Py_None;

    if (!_PyArg_UnpackStack(args, nargs, "readlines",
        0, 1,
        &arg)) {
        goto exit;
    }

    if (!_PyArg_NoStackKeywords("readlines", kwnames)) {
        goto exit;
    }
    return_value = _io_BytesIO_readlines_impl(self, arg);

exit:
    return return_value;
}

PyDoc_STRVAR(_io_BytesIO_readinto__doc__,
"readinto($self, buffer, /)\n"
"--\n"
"\n"
"Read bytes into buffer.\n"
"\n"
"Returns number of bytes read (0 for EOF), or None if the object\n"
"is set not to block and has no data to read.");

#define _IO_BYTESIO_READINTO_METHODDEF    \
    {"readinto", (PyCFunction)_io_BytesIO_readinto, METH_O, _io_BytesIO_readinto__doc__},

static PyObject *
_io_BytesIO_readinto_impl(bytesio *self, Py_buffer *buffer);

static PyObject *
_io_BytesIO_readinto(bytesio *self, PyObject *arg)
{
    PyObject *return_value = NULL;
    Py_buffer buffer = {NULL, NULL};

    if (!PyArg_Parse(arg, "w*:readinto", &buffer)) {
        goto exit;
    }
    return_value = _io_BytesIO_readinto_impl(self, &buffer);

exit:
    /* Cleanup for buffer */
    if (buffer.obj) {
       PyBuffer_Release(&buffer);
    }

    return return_value;
}

PyDoc_STRVAR(_io_BytesIO_truncate__doc__,
"truncate($self, size=None, /)\n"
"--\n"
"\n"
"Truncate the file to at most size bytes.\n"
"\n"
"Size defaults to the current file position, as returned by tell().\n"
"The current file position is unchanged.  Returns the new size.");

#define _IO_BYTESIO_TRUNCATE_METHODDEF    \
    {"truncate", (PyCFunction)_io_BytesIO_truncate, METH_FASTCALL, _io_BytesIO_truncate__doc__},

static PyObject *
_io_BytesIO_truncate_impl(bytesio *self, PyObject *arg);

static PyObject *
_io_BytesIO_truncate(bytesio *self, PyObject **args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    PyObject *arg = Py_None;

    if (!_PyArg_UnpackStack(args, nargs, "truncate",
        0, 1,
        &arg)) {
        goto exit;
    }

    if (!_PyArg_NoStackKeywords("truncate", kwnames)) {
        goto exit;
    }
    return_value = _io_BytesIO_truncate_impl(self, arg);

exit:
    return return_value;
}

PyDoc_STRVAR(_io_BytesIO_seek__doc__,
"seek($self, pos, whence=0, /)\n"
"--\n"
"\n"
"Change stream position.\n"
"\n"
"Seek to byte offset pos relative to position indicated by whence:\n"
"     0  Start of stream (the default).  pos should be >= 0;\n"
"     1  Current position - pos may be negative;\n"
"     2  End of stream - pos usually negative.\n"
"Returns the new absolute position.");

#define _IO_BYTESIO_SEEK_METHODDEF    \
    {"seek", (PyCFunction)_io_BytesIO_seek, METH_FASTCALL, _io_BytesIO_seek__doc__},

static PyObject *
_io_BytesIO_seek_impl(bytesio *self, Py_ssize_t pos, int whence);

static PyObject *
_io_BytesIO_seek(bytesio *self, PyObject **args, Py_ssize_t nargs, PyObject *kwnames)
{
    PyObject *return_value = NULL;
    Py_ssize_t pos;
    int whence = 0;

    if (!_PyArg_ParseStack(args, nargs, "n|i:seek",
        &pos, &whence)) {
        goto exit;
    }

    if (!_PyArg_NoStackKeywords("seek", kwnames)) {
        goto exit;
    }
    return_value = _io_BytesIO_seek_impl(self, pos, whence);

exit:
    return return_value;
}

PyDoc_STRVAR(_io_BytesIO_write__doc__,
"write($self, b, /)\n"
"--\n"
"\n"
"Write bytes to file.\n"
"\n"
"Return the number of bytes written.");

#define _IO_BYTESIO_WRITE_METHODDEF    \
    {"write", (PyCFunction)_io_BytesIO_write, METH_O, _io_BytesIO_write__doc__},

PyDoc_STRVAR(_io_BytesIO_writelines__doc__,
"writelines($self, lines, /)\n"
"--\n"
"\n"
"Write lines to the file.\n"
"\n"
"Note that newlines are not added.  lines can be any iterable object\n"
"producing bytes-like objects. This is equivalent to calling write() for\n"
"each element.");

#define _IO_BYTESIO_WRITELINES_METHODDEF    \
    {"writelines", (PyCFunction)_io_BytesIO_writelines, METH_O, _io_BytesIO_writelines__doc__},

PyDoc_STRVAR(_io_BytesIO_close__doc__,
"close($self, /)\n"
"--\n"
"\n"
"Disable all I/O operations.");

#define _IO_BYTESIO_CLOSE_METHODDEF    \
    {"close", (PyCFunction)_io_BytesIO_close, METH_NOARGS, _io_BytesIO_close__doc__},

static PyObject *
_io_BytesIO_close_impl(bytesio *self);

static PyObject *
_io_BytesIO_close(bytesio *self, PyObject *Py_UNUSED(ignored))
{
    return _io_BytesIO_close_impl(self);
}

PyDoc_STRVAR(_io_BytesIO___init____doc__,
"BytesIO(initial_bytes=b\'\')\n"
"--\n"
"\n"
"Buffered I/O implementation using an in-memory bytes buffer.");

static int
_io_BytesIO___init___impl(bytesio *self, PyObject *initvalue);

static int
_io_BytesIO___init__(PyObject *self, PyObject *args, PyObject *kwargs)
{
    int return_value = -1;
    static const char * const _keywords[] = {"initial_bytes", NULL};
    static _PyArg_Parser _parser = {"|O:BytesIO", _keywords, 0};
    PyObject *initvalue = NULL;

    if (!_PyArg_ParseTupleAndKeywordsFast(args, kwargs, &_parser,
        &initvalue)) {
        goto exit;
    }
    return_value = _io_BytesIO___init___impl((bytesio *)self, initvalue);

exit:
    return return_value;
}
/*[clinic end generated code: output=08139186b009ec47 input=a9049054013a1b77]*/
