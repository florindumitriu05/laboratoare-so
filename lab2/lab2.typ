#set page(paper: "a4", numbering: "1", margin: 3em, flipped: true)
#set heading(numbering: "1.")
#show heading: set block(below: 2em)

#let compare(img_linux, body_linux, img_windows, body_windows, img_size: 55%) = {
  set align(center)
  set par(justify: true)
  set heading(numbering: none)

  grid(columns: 2, gutter: 2em,
  [
    == Linux
    #image(img_linux, height: img_size) \
    #body_linux
  ],
  [
    == Windows
    #image(img_windows, height: img_size) \
    #body_windows
  ])
}

#show title: set align(center)
#show title: set block(below: 3em)

#title()[
  Sisteme de operare \
  Laborator 2
]

= Vizualizarea listei de procese in executie
#compare("images/1-linux.png", [
  In Linux, vizualizarea listei de procese se face folosind programul _top_, iar terminarea unui proces se face prin apasarea tastei 'k' si introducerea process id-ului (PID) specific procesului pe care dorim sa il terminam.
],
"images/1-windows.png", [
  In Windows, vizualizarea listei de procese se face folosind _Task Manager_, iar terminarea unui proces se face apasand click dreapta pe acel proces si selectand "End task".
])

= Vizualizarea listei/dezinstalarea/instalarea de programe
#compare("images/2-linux.png", [
  In Linux, vizualizarea listei de packages instalate se face folosind "apt-get list --installed" (pe Ubuntu), iar instalarea/dezinstalarea unui program se face folosind "sudo apt-get install \<numele pachetului\>" si "sudo apt-get remove \<numele pachetului\>".
], "images/2-windows.png", [
  In Windows, vizualizarea listei de programe instalate se face folosind programul _Apps & features_, de unde se pot dezinstala programe apasand pe butonul "Uninstall" al acestora. Instalarea programelor se face din Windows Store sau 
])

= Vizualizarea specificatiilor hardware ale masinii curente
#compare("images/3-linux.png", [
  In Linux, vizualizarea specificatiilor hardware ale masinii curente se poate face folosind comanda "sudo lshw"
], "images/3-windows.png", [
  In Windows, vizualizarea specificatiilor hardware ale masinii curente se poate face in meniul "About" din setari.
])

= Vizualizarea porturilor USB/PCI libere si in uz
#compare("images/4-linux.png", [
  In Linux, vizualizarea porturilor USB/PCI se face folosind comenzile "lsusb"/"lspci".
], "images/4-windows.png", [
  In Windows, vizualizarea porturilor USB/PCI se face folosind programul _Device Manager_.
])
