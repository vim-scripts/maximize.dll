Maximizing tool for Win32 gVim

Made by kAtremer <katremer@yandex.ru>
Last changed: 2007-10-16

http://vim.sf.net/scripts/script.php?script_id=1302



INSTALLATION

Put maximize.vim and maximize.dll in $VIM\vimfiles\plugin.


For Cream, you will also have to write:
  let g:CREAM_WINPOS=0
in your cream-conf.vim


To temporarily disable the plugin, use:
  let g:loaded_maximize=1



COMPILATION

Use MS Visual C++ (tested with 6.0 and 2005 aka 8.0)
Build with
  cl /LD maximize.cpp user32.lib kernel32.lib



USAGE

You may call the DLL from your own scripts.

To maximize:
  libcallnr('maximize', 'Maximize', 1)

To restore:
  libcallnr('maximize', 'Maximize', 0)

Returns 1 on success, 0 on fault.


HISTORY

version 1.1a [2007-10-16]: 4.5 KB DLL
- tidied up the files
- made maximize.vim for installation as a plugin
  (mainly to support Cream)

version 1.1 [2005-07-03]: 4.5 KB DLL
- revision of the pragmas, got smaller DLL
- now the class name of the supposed-Vim-window is checked too,
  because Vim may have several top-level windows
- thanks to Ilya Bobir <ilya -at- po4ta.com>

version 1.0 [2005-06-15]: 6.0 KB DLL
- initial release



ON THE SUBJECT OF SMALL DLLs

- For an English intro , see "Techniques for reducing Executable size"
  originally on http://freespace.virgin.net/james.brown7/tuts/minexe.htm
  archived at http://web.archive.org/web/20040210073930/http://freespace.virgin.net/james.brown7/tuts/minexe.htm

- Russians may also read the 28th nabla on dklab.ru
  http://dklab.ru/chicken/nablas/28.html
