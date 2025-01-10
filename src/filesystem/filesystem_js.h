#pragma once
#include "filesystem.h"

// This is some fucked shit, but hey, it works
EM_JS(void, emsc_js_init, (void),
{
    Module['emsc_js_onload'] = emsc_js_onload;
});

EM_JS(void, emsc_js_download_string, (const char* c_filename, const char* c_content),
{
    var filename = UTF8ToString(c_filename);
    var content = UTF8ToString(c_content);
    var elm = document.createElement('a');
    elm.setAttribute('href', 'data:text/plain;charset=utf-8,'+encodeURIComponent(content));
    elm.setAttribute('download', filename);
    elm.style.display='none';
    document.body.appendChild(elm);
    elm.click();
    document.body.removeChild(elm);
});

EM_JS(void, emsc_js_download_binary, (const char* c_filename, const uint8_t* ptr, int num_bytes),
{
    var filename = UTF8ToString(c_filename);
    var binary = "";
    for (var i = 0; i < num_bytes; i++)
    {
        binary += String.fromCharCode(HEAPU8[ptr+i]);
    }
    console.log(btoa(binary));
    var elm = document.createElement('a');
    elm.setAttribute('href', 'data:application/octet-stream;base64,'+btoa(binary));
    elm.setAttribute('download', filename);
    elm.style.display='none';
    document.body.appendChild(elm);
    elm.click();
    document.body.removeChild(elm);
});

EM_JS(void, emsc_js_load, (void),
{
    document.getElementById('picker').click();
});

EM_JS(void, emsc_js_onload, (void),
{
    var picker = document.getElementById('picker');
    // reset the file picker
    var file = picker.files[0];
    picker.value = null;
    console.log('--- load file:');
    console.log('  name: ' + file.name);
    console.log('  type: ' + file.type);
    console.log('  size: ' + file.size);
    if (file.size < 256000)
    {
        var reader = new FileReader();
        reader.onload = function(loadEvent) 
        {
            var content = loadEvent.target.result;
            if (content)
            {
                var uint8Array = new Uint8Array(content);
                var res = ccall('emsc_loadfile',  // C function name
                    'int',
                    ['string', 'array', 'number'],  // name, data, size
                    [file.name, uint8Array, uint8Array.length]);
                if (res != 0)
                {
                    console.warn('loadfile() failed!');
                } 
            }
            else
            {
                console.warn('load result empty!');
            }
        };
        reader.readAsArrayBuffer(file);
    }
    else
    {
        console.warn('ignoring file because it is too big')
    }
});