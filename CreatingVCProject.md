ENGLISH
1) Download and install wxWidgets

http://sourceforge.net/projects/wxwindows/files/wxMSW/2.8.10/wxMSW-2.8.10-Setup.exe/download

2) Open the project wxWidgets in the VC++.

For exemple in: C:\wxWidgets-2.8.10\build\msw\wx.dsw.

And compile this project. You can compile only for Debugs and Releases, using the option  "Build->Batch Build..." and selecting only Debug and Release in Configuration. Click "Build" after that.

3) Now you have to configure the properties of your project:

In C/C++ -> General:

Additional Include Directories:
.\..\..\lib\vc\_lib\mswd; .\..\..\include; .; .\..\..\samples
Assuming your wxWidgets is in C:\wxWidgets-2.8.10 you will put:
"C:\wxWidgets-2.8.10\lib\vc\_lib\mswd"; "C:\wxWidgets-2.8.10\include"; "C:\wxWidgets-2.8.10"; "C:\wxWidgets-2.8.10\samples"
In Linker -> General:

Additional Libraries Directories:
.\..\..\lib\vc\_lib
you will put:
"C:\wxWidgets-2.8.10\lib\vc\_lib"
In Linker -> Input:

Additional Dependencies:
wxmsw28d\_richtext.lib; wxbase28d\_xml.lib; wxmsw28d\_adv.lib; wxmsw28d\_html.lib; wxmsw28d\_core.lib; wxbase28d.lib; wxtiffd.lib; wxjpegd.lib; wxpngd.lib; wxzlibd.lib; wxregexd.lib; wxexpatd.lib; winmm.lib; comctl32.lib; rpcrt4.lib; wsock32.lib; odbc32.lib

4) In General:

Character Set:
change to "Not Set"


PORTUGUES


Para criar um novo projeto do ICPD no Visual C++ no Windows, √© necess√°rio incluir as bibliotecas do wxWidgets, seguindo esse passo a passo:

1) Instalar a wxWidgets, executando o aplicativo wxMSW-2.8.10-Setup.exe, localizado em http://sourceforge.net/projects/wxwindows/files/wxMSW/2.8.10/wxMSW-2.8.10-Setup.exe/download

2) Abrir o projeto do wxWidgets no VC++. No exemplo, em C:\wxWidgets-2.8.10\build\msw\wx.dsw. Compilar esse projeto. Uma op√ß√£o √© compilar apenas Debugs e Releases, entrando em "Build->Batch Build..." e selecionando apenas os Debug e Release do item Configuration, ap√≥s, clicar em "Build".

3) Tendo como base o projeto pronto da amostra da wxWidgets, no exemplo, localizado em C:\wxWidgets-2.8.10\samples\widgets\widgets.dsp, deve-se configurar, nas propriedades do projeto que se deseja criar no Visual C++, os seguintes diret√≥rios:

Em C/C++ -> General:
> Additional Include Directories:
> > .\..\..\lib\vc\_lib\mswd;
> > .\..\..\include;
> > .;
> > .\..\..\samples
> > No exemplo fica:
> > > "C:\wxWidgets-2.8.10\lib\vc\_lib\mswd";
> > > "C:\wxWidgets-2.8.10\include";
> > > "C:\wxWidgets-2.8.10";
> > > "C:\wxWidgets-2.8.10\samples"

Em Linker -> General:

> Additional Libraries Directories:
> > .\..\..\lib\vc\_lib
> > No exemplo fica:
> > > "C:\wxWidgets-2.8.10\lib\vc\_lib"

Em Linker -> Input:

> Additional Dependencies:
> > wxmsw28d\_richtext.lib;
> > wxbase28d\_xml.lib;
> > wxmsw28d\_adv.lib;
> > wxmsw28d\_html.lib;
> > wxmsw28d\_core.lib;
> > wxbase28d.lib;
> > wxtiffd.lib;
> > wxjpegd.lib;
> > wxpngd.lib;
> > wxzlibd.lib;
> > wxregexd.lib;
> > wxexpatd.lib;
> > winmm.lib;
> > comctl32.lib;
> > rpcrt4.lib;
> > wsock32.lib;
> > odbc32.lib

4) Ainda nas propriedades do projeto:

Em General:

> Character Set:
> > trocar para "Not Set";


> Por padr√£o, o Visual C++ o define como "Use Unicode Character Set", mas o projeto do ICPD n√£o compila com essa op√ß√£o.

As demais configura√ß√µes das propriedades, como Debug e Output File Directory, ficam a crit√©rio do programador.